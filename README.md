minimal reproducible example of header include order (of stddef.h and stdint.h) causing different results in alignment of max_align_t;

first noticed on MinGW (mingw-builds "personal build" i686-8.1.0-release-posix-dwarf-rt_v6-rev0 from mingw-w64 SourceForge), it seems to only affect MinGW's setup with two separate include directories (therefore two distinct <stddef.h> files), and not be a GCC-wide (trunk) bug.

"check" scripts contain commands for building and running the test (reports result whether the bug was found to stdout)

The following is my current understanding of where the issue comes from, and what to be done about it:

	mingw(-w64) (the "windows port of GCC") has two separate standard library include directories (as might other GCC distributions): One in "lib/gcc/<platform string>/<version string>/include" (hereafter "first path/directory") and one in "<platform string>/include" (hereafter "second path/directory").
	Apparently the first one is selected when including via system include syntax (<header.h>), but many of those files (including <stddef.h> and <stdint.h>) contain an #include_next directive (often near their beginning) referring to the file of the same name at the second path.
	The two variants of <stddef.h> don't agree on max_align_t:
		The second one (lib/gcc/<ps>/<vs>/include/stddef.h) has these two members:
			long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
			long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
		leading on my platform to an alignment of 8, while the first one (<ps>/include/stddef.h) has this additional member:
			#ifdef __i386__
			  __float128 __max_align_f128 __attribute__((__aligned__(__alignof(__float128))));
			#endif
		(with a trustworthy explanatory comment before it) leading on my platform to an alignment of 16.
	When included normally, the macro _GCC_MAX_ALIGN_T guards these definitions so the one from the second path (alignment 8) wins out.
	<stdint.h> and <inttypes.h> access <stddef.h> differently however, by first defining the macros __need_wint_t and __need_wchar_t. Both <stddef.h> have code to detect this, skip/ignore the definition of max_align_t and other types, and undefine the macros.
	Because this is done sequentially however, only one of the two headers (the one in the second include directory) ever gets to see the __need-macros, and now the other header (in directory 1) takes its turn in "normal operation", defining max_align_t.

	As a temporary workaround, you can make sure to always include <stddef.h> before <stdint.h> and <inttypes.h> (and possibly any other files that do this), because that results in alignment 8, which coincides with the (your? my) malloc implementation.
	Overall though alignment 16 is probably the "correct" value (to have __float128 support on i386), and it's actually malloc that's wrong. Therefore the max_align_t definition in directory two should probably be updated to additionally include the third member. It should further be investigated where the current malloc-alignment comes from, and fixed on the basis of MinGW wrapping malloc differently.
	
	So far I have only found these __need macros being used in <stdint.h> and <inttypes.h>. <stdarg.h> seems to support a similar mechanism, but I haven't found it used anywhere.
	
	I've further found that the change to 16 bytes was relatively recent, in GCC 7: https://gcc.gnu.org/git/?p=gcc.git;a=commitdiff;h=9b5c49ef97e63cc63f1ffa13baf771368105ebe2
	
	This elicited a bug report and change from glibc to make their malloc 16 byte alligned as well: https://sourceware.org/bugzilla/show_bug.cgi?id=21120
	
	It really seems like this change just went unnoticed by mingw, so it really seems both stddef.h and the malloc implementation "just" need to be updated by someone who knows where and how to do so.