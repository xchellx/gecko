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

#include <stdext/catexit.h>

#include <stdlib.h>
#include <signal.h>

volatile char catexitIsRegistered = 0;

volatile catexitHandler __cregsignalHandler__ = SIG_DFL;

static int catexitSigs[12] = {
#ifdef SIGHUP
    SIGHUP,
#else
    -1,
#endif
#ifdef SIGQUIT
    SIGQUIT,
#else
    -1,
#endif
#ifdef SIGTSTP
    SIGTSTP,
#else
    -1,
#endif
#ifdef SIGABRT
    SIGABRT,
#else
    -1,
#endif
#ifdef SIGIOT
    SIGIOT,
#else
    -1,
#endif
#ifdef SIGKILL
    SIGKILL,
#else
    -1,
#endif
#ifdef SIGSTOP
    SIGSTOP,
#else
    -1,
#endif
#ifdef SIGINT
    SIGINT,
#else
    -1,
#endif
#ifdef SIGSEGV
    SIGSEGV,
#else
    -1,
#endif
#ifdef SIGTERM
    SIGTERM,
#else
    -1,
#endif
#ifdef SIGBREAK
    SIGBREAK,
#else
    -1,
#endif
#ifdef SIGABRT_COMPAT
    SIGABRT_COMPAT,
#else
    -1,
#endif
};

void __catexitHandler__(void) {
    __cregsignalHandler__(-1);
}

void __csignalHandler__(int sig) {
    __cregsignalHandler__(sig);
}

int catexit(catexitHandler handler, char remove) {
    int success = 0;
    // FIXME: Giving volatile-ness is valid?
    __cregsignalHandler__ = (volatile catexitHandler) (remove ? SIG_DFL : handler);
    if (!remove)
        success += !atexit(__catexitHandler__);
    for (int i = 0; i < 12; i++) {
        if (catexitSigs[i] != -1) {
#ifdef _WIN32
            success += signal(catexitSigs[i], remove ? SIG_DFL : __csignalHandler__) != SIG_ERR;
#else
            struct sigaction act;
            act.sa_handler = remove ? SIG_DFL : __csignalHandler__;
            success += sigaction(catexitSigs[i], &act, NULL) != -1;
#endif
        }
    }
    catexitIsRegistered = remove ? !!0 : !!success;
    return success;
}
