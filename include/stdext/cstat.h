/*
 * MIT License
 * 
 * Copyright (c) 2023 Yonder
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __CSTAT_H__
#define __CSTAT_H__
#include <stdio.h>

#include <stdext/cmacros.h>

typedef enum __CstatError {
    CSE_ERR_SUCCESS = 0,
    CSE_ERR_NULLPTR,
    CSE_ERR_EACCES,
    CSE_ERR_EFAULT,
    CSE_ERR_ELOOP,
    CSE_ERR_ENAMETOOLONG,
    CSE_ERR_ENOENT,
    CSE_ERR_ENOMEM,
    CSE_ERR_ENOTDIR,
    CSE_ERR_EOVERFLOW,
    CSE_ERR_UNKNOWN
} CstatError;

typedef enum __CfopenError {
    COE_ERR_SUCCESS = 0,
    COE_ERR_NULLPTR,
    COE_ERR_EACCES,
    COE_ERR_EINTR,
    COE_ERR_EISDIR,
    COE_ERR_ELOOP,
    COE_ERR_EMFILE,
    COE_ERR_ENAMETOOLONG,
    COE_ERR_ENFILE,
    COE_ERR_ENOENT,
    COE_ERR_ENOSPC,
    COE_ERR_ENOTDIR,
    COE_ERR_ENXIO,
    COE_ERR_EOVERFLOW,
    COE_ERR_EROFS,
    COE_ERR_EINVAL,
    COE_ERR_ENOMEM,
    COE_ERR_ETXTBSY,
    COE_ERR_UNKNOWN
} CfopenError;

CstatError cfexists(char *path, int *isDir);

CfopenError cfopen(char *path, char *mode, FILE **file);

INLINE char *CstatError_ToStr(CstatError cstatError) {
    switch (cstatError) {
        case CSE_ERR_SUCCESS:
            return "CSE_ERR_SUCCESS"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Operation was successful."
#endif
            ;
        case CSE_ERR_NULLPTR:
            return "CSE_ERR_NULLPTR"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Input buffer and/or output int is NULL."
#endif
            ;
        case CSE_ERR_EACCES:
            return "CSE_ERR_EACCES"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Search permission is denied for one of the directories in the path prefix of pathname."
#endif
            ;
        case CSE_ERR_EFAULT:
            return "CSE_ERR_EFAULT"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Bad address."
#endif
            ;
        case CSE_ERR_ELOOP:
            return "CSE_ERR_ELOOP"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Too many symbolic links encountered while traversing the path."
#endif
            ;
        case CSE_ERR_ENAMETOOLONG:
            return "CSE_ERR_ENAMETOOLONG"
#ifdef __STDEXT_INCLERRSTRGS__
                ": pathname is too long."
#endif
            ;
        case CSE_ERR_ENOENT:
            return "CSE_ERR_ENOENT"
#ifdef __STDEXT_INCLERRSTRGS__
                ": A component of pathname does not exist or is a dangling symbolic link."
#endif
            ;
        case CSE_ERR_ENOMEM:
            return "CSE_ERR_ENOMEM"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Out of memory (i.e., kernel memory)."
#endif
            ;
        case CSE_ERR_ENOTDIR:
            return "CSE_ERR_ENOTDIR"
#ifdef __STDEXT_INCLERRSTRGS__
                ": A component of the path prefix of pathname is not a directory."
#endif
            ;
        case CSE_ERR_EOVERFLOW:
            return "CSE_ERR_EOVERFLOW"
#ifdef __STDEXT_INCLERRSTRGS__
                ": pathname or fd refers to a file whose size, inode number, or number of blocks cannot be represented"
                " in, respectively, the types off_t, ino_t, or blkcnt_t.  This error can occur when, for example, an "
                "application compiled on a 32-bit platform without -D_FILE_OFFSET_BITS=64 calls stat() on a file whose"
                " size exceeds (1<<31)-1 bytes."
#endif
            ;
        case CSE_ERR_UNKNOWN:
            return "CSE_ERR_UNKNOWN"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Uknown error code."
#endif
            ;
        default:
            return "UNKNOWN"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Invalid error code."
#endif
            ;
    }
}

INLINE char *CfopenError_ToStr(CfopenError cfopenError) {
    switch (cfopenError) {
        case COE_ERR_SUCCESS:
            return "COE_ERR_SUCCESS"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Operation was successful."
#endif
            ;
        case COE_ERR_NULLPTR:
            return "COE_ERR_NULLPTR"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Input pathname, input mode, and/or output FILE is null."
#endif
            ;
        case COE_ERR_EACCES:
            return "COE_ERR_EACCES"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Search permission is denied on a component of the path prefix, or the file exists and the "
                "permissions specified by mode are denied, or the file does not exist and write permission is denied "
                "for the parent directory of the file to be created."
#endif
            ;
        case COE_ERR_EINTR:
            return "COE_ERR_EINTR"
#ifdef __STDEXT_INCLERRSTRGS__
                ": A signal was caught during fopen()."
#endif
            ;
        case COE_ERR_EISDIR:
            return "COE_ERR_EISDIR"
#ifdef __STDEXT_INCLERRSTRGS__
                ": The named file is a directory and mode requires write access."
#endif
            ;
        case COE_ERR_ELOOP:
            return "COE_ERR_ELOOP"
#ifdef __STDEXT_INCLERRSTRGS__
                ": A loop exists in symbolic links encountered during resolution of the path argument or more than "
                "{SYMLOOP_MAX} symbolic links were encountered during resolution of the path argument."
#endif
            ;
        case COE_ERR_EMFILE:
            return "COE_ERR_EMFILE"
#ifdef __STDEXT_INCLERRSTRGS__
                ": All file descriptors available to the process are currently open or {STREAM_MAX} streams are "
                "currently open in the calling process or {FOPEN_MAX} streams are currently open in the calling "
                "process."
#endif
            ;
        case COE_ERR_ENAMETOOLONG:
            return "COE_ERR_ENAMETOOLONG"
#ifdef __STDEXT_INCLERRSTRGS__
                ": The length of a pathname exceeds {PATH_MAX}, or pathname resolution of a symbolic link produced an "
                "intermediate result with a length that exceeds {PATH_MAX} or the length of a component of a pathname "
                "is longer than {NAME_MAX}."
#endif
            ;
        case COE_ERR_ENFILE:
            return "COE_ERR_ENFILE"
#ifdef __STDEXT_INCLERRSTRGS__
                ": The maximum allowable number of files is currently open in the system."
#endif
            ;
        case COE_ERR_ENOENT:
            return "COE_ERR_ENOENT"
#ifdef __STDEXT_INCLERRSTRGS__
                ": The mode string begins with 'r' and a component of pathname does not name an existing file, or mode"
                " begins with 'w' or 'a' and a component of the path prefix of pathname does not name an existing file"
                ", or pathname is an empty string or the pathname argument contains at least one non-<slash> character"
                " and ends with one or more trailing <slash> characters. If pathname without the trailing <slash> "
                "characters would name an existing file, an [ENOENT] error shall not occur."
#endif
            ;
        case COE_ERR_ENOSPC:
            return "COE_ERR_ENOSPC"
#ifdef __STDEXT_INCLERRSTRGS__
                ": The directory or file system that would contain the new file cannot be expanded, the file does not "
                "exist, and the file was to be created."
#endif
            ;
        case COE_ERR_ENOTDIR:
            return "COE_ERR_ENOTDIR"
#ifdef __STDEXT_INCLERRSTRGS__
                ": A component of the path prefix names an existing file that is neither a directory nor a symbolic "
                "link to a directory, or the pathname argument contains at least one non-<slash> character and ends "
                "with one or more trailing <slash> characters and the last pathname component names an existing file "
                "that is neither a directory nor a symbolic link to a directory or the pathname argument contains at "
                "least one non-<slash> character and ends with one or more trailing <slash> characters. If pathname "
                "without the trailing <slash> characters would name an existing file, an [ENOENT] error shall not "
                "occur."
#endif
            ;
        case COE_ERR_ENXIO:
            return "COE_ERR_ENXIO"
#ifdef __STDEXT_INCLERRSTRGS__
                ": The named file is a character special or block special file, and the device associated with this "
                "special file does not exist."
#endif
            ;
        case COE_ERR_EOVERFLOW:
            return "COE_ERR_EOVERFLOW"
#ifdef __STDEXT_INCLERRSTRGS__
                ": The named file is a regular file and the size of the file cannot be represented correctly in an "
                "object of type off_t."
#endif
            ;
        case COE_ERR_EROFS:
            return "COE_ERR_EROFS"
#ifdef __STDEXT_INCLERRSTRGS__
                ": The named file resides on a read-only file system and mode requires write access."
#endif
            ;
        case COE_ERR_EINVAL:
            return "COE_ERR_EINVAL"
#ifdef __STDEXT_INCLERRSTRGS__
                ": The value of the mode argument is not valid."
#endif
            ;
        case COE_ERR_ENOMEM:
            return "COE_ERR_ENOMEM"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Insufficient storage space is available."
#endif
            ;
        case COE_ERR_ETXTBSY:
            return "COE_ERR_ETXTBSY"
#ifdef __STDEXT_INCLERRSTRGS__
                ": The file is a pure procedure (shared text) file that is being executed and mode requires write "
                "access."
#endif
            ;
        case COE_ERR_UNKNOWN:
            return "COE_ERR_UNKNOWN"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Uknown error code."
#endif
            ;
        default:
            return "UNKNOWN"
#ifdef __STDEXT_INCLERRSTRGS__
                ": Invalid error code."
#endif
            ;
    }
}
#endif
