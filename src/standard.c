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

#include <standard.h>

#include <getopt.h>
#include <string.h>

#ifndef _WIN32
#include <strings.h>
#endif

#include <stdext.h>

static volatile char standardLoopSafety = 1;

static char *shortOpts = "-:hyo:c:";
static struct option longOpts[5] = {
    { "help",    no_argument,       NULL, 'h' },
    { "yes",     no_argument,       NULL, 'y' },
    { "outfile", required_argument, NULL, 'o' },
    { "codefmt", required_argument, NULL, 'c' },
    { NULL,      0,                 NULL, 0   }
};

void sigonexit(int sig) {
    if (ctermIsModified)
        crestoreTerm();
    if (catexitIsRegistered)
        standardLoopSafety = 0;
}

int main(int argc, char **argv) {
    if (catexit(sigonexit, 0) < 2) {
        fprintf(stderr, "ERROR: Failed to set atexit handlers\n");
        return 1;
    }
    
    char help = 0, yes = 0;
    char *outfName = NULL;
    CLFFmt listFmt = CLF_NONE;
    int argi = 1;
    uint8_t ignoreOpts = 0;
    optind = 1;
    while (standardLoopSafety) {
        opterr = 0;
        int opti = 0;
        char optc = getopt_long(argc, argv, shortOpts, longOpts, &opti);
        if (ignoreOpts && optc != -1)
            optc = '\1';
        
        // This hack/workaround was done for dash-dash because getopt for some reason considers arguments after dash-
        // dash to be separate than '\1' arguments in the while loop (for which the '-' at the start of a short option
        // string makes arguments pass as '\1' in the while loop). This is to ensure that both arguments before and
        // after dash-dash are passed into the while loop at '\1'.
        uint8_t dashDash = !strcmp(argv[optind - 1], "--");
        if (!ignoreOpts)
            ignoreOpts = dashDash;
        
        if (dashDash)
            continue;
        else if (optc == -1)
            break;
        
        switch (optc) {
            case '\1':
                break;
            case 'o':
                if (outfName) {
                    fprintf(stderr, "ERROR: Cannot specify 'o' option multiple times\n");
                    return 1;
                } else if (STR_ISNULL(optarg)) {
                    fprintf(stderr, "ERROR: Missing value for 'o' option\n");
                    return 1;
                } else
                    outfName = optarg;
                break;
            case 'c':
                if (listFmt != CLF_NONE) {
                    fprintf(stderr, "ERROR: Cannot specify 'c' option multiple times\n");
                    return 1;
                } else if (STR_ISNULL(optarg)) {
                    fprintf(stderr, "ERROR: Missing value for 'c' option\n");
                    return 1;
                } else {
                    if (!cstrcmpi(optarg, "dolphin"))
                        listFmt = CLF_DOLPHIN;
                    else if (!cstrcmpi(optarg, "gct"))
                        listFmt = CLF_GCT;
                    else if (!cstrcmpi(optarg, "ocarina"))
                        listFmt = CLF_OCARINA;
                    else if (!cstrcmpi(optarg, "raw"))
                        listFmt = CLF_RAW;
                    else if (!cstrcmpi(optarg, "rawtext"))
                        listFmt = CLF_RAWTEXT;
                    else {
                        fprintf(stderr, "ERROR: Invalid value for 'c' option\n");
                        return 1;
                    }
                }
                break;
            case 'y':
                yes = 1;
                break;
            case 'h':
                help = 1;
                break;
            case '?':
                if (optopt)
                    fprintf(stderr, "ERROR: Unknown option '%c'\n", optopt);
                else
                    fprintf(stderr, "ERROR: Unknown option \"%s\"\n", argv[optind - 1]);
                return 1;
            case ':':
                if (optopt)
                    fprintf(stderr, "ERROR: Missing value for option '%c'\n", optopt);
                else
                    fprintf(stderr, "ERROR: Missing value for option \"%s\"\n", argv[optind - 1]);
                return 1;
            default:
                fprintf(stderr, "ERROR: ?? getopt returned character code %i ??\n", optc);
                return 1;
        }
    }
    
    if (listFmt == CLF_NONE)
        listFmt = CLF_DOLPHIN;
    
    if (help) {
        fprintf(stderr, (
            __STANDARD_USAGE_INTRO__ " [-h/--help] [-y/--yes] [-o/--outfile <path>] [-c/--codefmt <fmt>]\n"
            "  h/help: Display this message\n"
            "  y/yes: Do not ask to press any key (non-interactive)\n"
            "  o/outfile: The file to output to (instead of stdout)\n"
            "    <path>:\n"
            "      A path to a file\n"
            "  c/codefmt: The code list format to output to\n"
            "    <fmt>:\n"
            "      dolphin: INI code list format; what Dolphin Emulator supports\n"
            "      gct: Gecko code list format; what real hardware loaders (Nintendont and the like) support\n"
            "      ocarina: Ocarina code list format; what many code managers support\n"
            "      raw: Raw binary output; no loader support but may be used by other applications\n"
            "      rawtext: Raw text output; no loader support but may be used by other applications\n"
        ));
        return 1;
    }
    
    G_IsOutputBin = (listFmt == CLF_GCT || listFmt == CLF_RAW);
    FILE *outf = NULL;
    if (outfName) {
        CfopenError outfErr = cfopen(outfName, G_IsOutputBin ? "wb" : "wt", &outf);
        if (outfErr) {
            fprintf(stderr, "ERROR: Couldn't open file \"%s\": %s\n", outfName, CfopenError_ToStr(outfErr));
            return 1;
        }
        G_OutputHandle = outf;
    } else
        G_OutputHandle = stdout;
    
    printclf(listFmt);
    
    if (outf) {
        fclose(outf);
        outf = NULL;
        G_OutputHandle = NULL;
    }
    
    if (!yes) {
        fprintf(stderr, "Press any key to continue . . . ");
        cgetch();
        fprintf(stderr, "\n");
    }
    return 0;
}
