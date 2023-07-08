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

#ifndef __STANDARD_H__
#define __STANDARD_H__
#ifndef CODE_NAME
#error "Please provide a code name for printing the code in the CODE_NAME macro BEFORE including standard.h"
#endif
#ifndef CODE_AUTHOR
#error "Please provide a author name for printing the code in the CODE_AUTHOR macro BEFORE including standard.h"
#endif
#ifndef CODE_DEFINITION
#error "Please provide a method name for printing the code in the CODE_DEFINITION macro BEFORE including standard.h"
#endif

#include <stdio.h>

#include <cgetchar.h>

int main(int argc, char** argv) {
    char silent = 0;
    if (argc > 1)
        silent = argv[1][0] == 's';
    
    fprintf(stdout, "$%s [%s]\n", CODE_NAME, CODE_AUTHOR);
    
    CODE_DEFINITION();
    
#ifdef CODE_DESCRIPTION
    fprintf(stdout, "%s\n", CODE_DESCRIPTION);
#endif
    
    if (!silent) {
        fprintf(stderr, "Press any key to continue . . . ");
        cgetch();
        fprintf(stderr, "\n");
    }
    return 0;
}
#endif
