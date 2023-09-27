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

#ifndef __CMACROS_H__
#define __CMACROS_H__
#define CHR_dirseplinux '/'
#define CHR_dirsepwindows '\\'

#ifdef _WIN32
#define CHR_dirsep CHR_dirsepwindows
#else
#define CHR_dirsep CHR_dirseplinux
#endif

#ifndef SWAP16
#define SWAP16(x) __builtin_bswap32((x))
#endif
#ifndef SWAP32
#define SWAP32(x) __builtin_bswap32((x))
#endif

#define STR_TOSTR(s) #s
#define STR_ISNULL(s) (!(s) || !*(s))

#ifndef INLINE
#define INLINE static inline __attribute__((always_inline))
#endif
#ifndef ALIGNED
#define ALIGNED __attribute__((packed, aligned(1)))
#endif
#ifndef PACKED
#define PACKED __attribute__((packed))
#endif

#ifdef __STDEXT_CMACROS_H_DEPDEFS__
#ifdef _WIN32
#ifdef __STDEXT_CMACROS_H_DEPINCL__
#include <string.h>
#endif
#define cstrcmpi stricmp
#else
#ifdef __STDEXT_CMACROS_H_DEPINCL__
#include <strings.h>
#endif
#define cstrcmpi strcasecmp
#endif
#endif
#endif
