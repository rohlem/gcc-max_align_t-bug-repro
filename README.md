minimal reproducible example of header include order (of stddef.h and stdint.h) causing different results in alignment of max_align_t;

first noticed on MinGW (mingw-builds "personal build" i686-8.1.0-release-posix-dwarf-rt_v6-rev0 from mingw-w64 SourceForge), currently unclear whether this might be a GCC-wide bug.

"check" scripts contain commands for building and running the test (reports result whether the bug was found to stdout)

(Note that the current malloc implementation in my MinGW distro only aligns to 8 bytes (but that seems configurable and might be a result of this mix-up))