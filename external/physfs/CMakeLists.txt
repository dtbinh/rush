project(physfs)

include_directories(. zlib123)

file(GLOB PHYSFS_FILES "*.h" "*.c")
source_group(physfs FILES ${PHYSFS_FILES})

SET(ZLIB_FILES
    zlib123/adler32.c
    zlib123/compress.c
    zlib123/crc32.c
    zlib123/deflate.c
    zlib123/gzio.c
    zlib123/infback.c
    zlib123/inffast.c
    zlib123/inflate.c
    zlib123/inftrees.c
    zlib123/trees.c
    zlib123/uncompr.c
    zlib123/zutil.c
)
source_group(zlib FILES ${ZLIB_FILES})

set(LZMA_FILES
    lzma/7zCrc.c
    lzma/Archive/7z/7zBuffer.c
    lzma/Archive/7z/7zDecode.c
    lzma/Archive/7z/7zExtract.c
    lzma/Archive/7z/7zHeader.c
    lzma/Archive/7z/7zIn.c
    lzma/Archive/7z/7zItem.c
    lzma/Archive/7z/7zMethodID.c
    lzma/Compress/Branch/BranchX86.c
    lzma/Compress/Branch/BranchX86_2.c
    lzma/Compress/Lzma/LzmaDecode.c
)
source_group(lzma FILES ${LZMA_FILES})

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
endif(MSVC)

add_definitions(-D_REENTRANT -D_THREAD_SAFE)
add_definitions(-DPHYSFS_SUPPORTS_ZIP=1)
add_definitions(-DPHYSFS_SUPPORTS_7Z=1)
add_definitions(-DZ_PREFIX=1)

add_library(physfs STATIC ${PHYSFS_FILES} ${ZLIB_FILES} ${LZMA_FILES})
