minimal reproducible example of header include order (of stddef.h and stdint.h) causing different results in alignment of max_align_t;
first noticed on MinGW (mingw-builds "personal build" i686-8.1.0-release-posix-dwarf-rt_v6-rev0 from mingw-w64 SourceForge), currently unclear whether this might be a GCC-wide bug.
(Note that the current malloc implementation only aligns to 8 bytes (but that seems configurable and might be a result of this mix-up))