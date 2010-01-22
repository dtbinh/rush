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

include(CheckIncludeFile)
include(CheckCSourceCompiles)
if(UNIX)
    check_include_file(sys/ucred.h HAVE_UCRED_H)
    if(HAVE_UCRED_H)
        add_definitions(-DPHYSFS_HAVE_SYS_UCRED_H=1)
            set(PHYSFS_HAVE_CDROM_SUPPORT TRUE)
        endif(HAVE_UCRED_H)

        check_include_file(mntent.h HAVE_MNTENT_H)
        if(HAVE_MNTENT_H)
            add_definitions(-DPHYSFS_HAVE_MNTENT_H=1)
            set(PHYSFS_HAVE_CDROM_SUPPORT TRUE)
        endif(HAVE_MNTENT_H)

        check_c_source_compiles("
            #include <stdio.h>
            #include <sys/mnttab.h>
            int main(int argc, char **argv) { return 0; }
        " HAVE_SYS_MNTTAB_H)

        if(HAVE_SYS_MNTTAB_H)
            add_definitions(-DPHYSFS_HAVE_SYS_MNTTAB_H=1)
            set(PHYSFS_HAVE_CDROM_SUPPORT TRUE)
        endif(HAVE_SYS_MNTTAB_H)

        check_include_file(pthread.h HAVE_PTHREAD_H)
        if(HAVE_PTHREAD_H)
            set(PHYSFS_HAVE_THREAD_SUPPORT TRUE)
        else(HAVE_PTHREAD_H)
            add_definitions(-DPHYSFS_NO_PTHREADS_SUPPORT=1)
        endif(HAVE_PTHREAD_H)
endif(UNIX)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
endif(MSVC)

add_definitions(-D_REENTRANT -D_THREAD_SAFE)
add_definitions(-DPHYSFS_SUPPORTS_ZIP=1)
add_definitions(-DPHYSFS_SUPPORTS_7Z=1)
add_definitions(-DZ_PREFIX=1)

add_library(physfs STATIC ${PHYSFS_FILES} ${ZLIB_FILES} ${LZMA_FILES})
