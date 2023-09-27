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

#include <stdext/cstat.h>

#include <errno.h>
#include <sys/stat.h>

CstatError cfexists(char *pathname, int *isDir) {
    if (pathname && isDir) {
        struct stat pathnameStat;
        if (stat(pathname, &pathnameStat)) {
            switch (errno) {
                case EACCES:
                    return CSE_ERR_EACCES;
                case EFAULT:
                    return CSE_ERR_EFAULT;
                case ELOOP:
                    return CSE_ERR_ELOOP;
                case ENAMETOOLONG:
                    return CSE_ERR_ENAMETOOLONG;
                case ENOENT:
                    return CSE_ERR_ENOENT;
                case ENOMEM:
                    return CSE_ERR_ENOMEM;
                case ENOTDIR:
                    return CSE_ERR_ENOTDIR;
                case EOVERFLOW:
                    return CSE_ERR_EOVERFLOW;
                default:
                    return CSE_ERR_UNKNOWN;
            }
        } else {
            *isDir = S_ISDIR(pathnameStat.st_mode);
            return CSE_ERR_SUCCESS;
        }
    } else
        return CSE_ERR_NULLPTR;
}

CfopenError cfopen(char *pathname, char *mode, FILE **file) {
    if (pathname && mode && file) {
        *file = fopen(pathname, mode);
        if (!*file) {
            switch (errno) {
                case EACCES:
                    return COE_ERR_EACCES;
                case EINTR:
                    return COE_ERR_EINTR;
                case EISDIR:
                    return COE_ERR_EISDIR;
                case ELOOP:
                    return COE_ERR_ELOOP;
                case EMFILE:
                    return COE_ERR_EMFILE;
                case ENAMETOOLONG:
                    return COE_ERR_ENAMETOOLONG;
                case ENFILE:
                    return COE_ERR_ENFILE;
                case ENOENT:
                    return COE_ERR_ENOENT;
                case ENOSPC:
                    return COE_ERR_ENOSPC;
                case ENOTDIR:
                    return COE_ERR_ENOTDIR;
                case ENXIO:
                    return COE_ERR_ENXIO;
                case EOVERFLOW:
                    return COE_ERR_EOVERFLOW;
                case EROFS:
                    return COE_ERR_EROFS;
                case EINVAL:
                    return COE_ERR_EINVAL;
                case ENOMEM:
                    return COE_ERR_ENOMEM;
                case ETXTBSY:
                    return COE_ERR_ETXTBSY;
                default:
                    return COE_ERR_UNKNOWN;
            }
        } else
            return COE_ERR_SUCCESS;
    } else
        return COE_ERR_NULLPTR;
}
