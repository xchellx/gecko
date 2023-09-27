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

#include <stdext/cgetchar.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#endif

#ifndef _WIN32
volatile struct termios __cgc_oldf_direct__;
volatile struct termios *__cgc_oldf__ = &__cgc_oldf_direct__;
#endif

volatile char ctermIsModified = 0;
volatile char ctermLoopSafety = 1;

void crestoreTerm(void) {
#ifndef _WIN32
    ctermLoopSafety = 0;
    ctermIsModified = 0;
    // FIXME: Losing volatile-ness is bad!
    tcsetattr(STDIN_FILENO, TCSANOW, (struct termios *) __cgc_oldf__);
#endif
}

char cgetchar(char echoc) {
    int c = '\0';
    ctermLoopSafety = 1;
    ctermIsModified = 1;
    
#ifdef _WIN32
    while (ctermLoopSafety && (c = (echoc ? getche() : getch())) != EOF && !iscntrl(c)) {
        if (c != '\0') {
            if (echoc && c != '\n')
                printf("\n");
            break;
        }
    }
#else
    
    struct termios newf;
    tcgetattr(STDIN_FILENO, &newf);
    *__cgc_oldf__ = newf;
    
    if (!echoc && (newf.c_lflag & ECHO) == ECHO)
        newf.c_lflag &= ~ECHO;
    if ((newf.c_lflag & ECHONL) == ECHONL)
        newf.c_lflag &= ~ECHONL;
    if ((newf.c_lflag & ICANON) == ICANON)
        newf.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &newf);
    
    while (ctermLoopSafety && (c = getchar()) != EOF && !iscntrl(c)) {
        if (c != '\0') {
            if (echoc && c != '\n')
                printf("\n");
            break;
        }
    }
    
    crestoreTerm();
#endif
    
    if (c == '\r')
        c = '\n';
    return (char) c;
}
