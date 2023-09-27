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
 * copies or substantial portions of the Software 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE 
 */

#ifndef __GECKO_H__
#define __GECKO_H__
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef INLINE
#define INLINE static inline __attribute__((always_inline))
#endif

/*
 * References used:
 * https://web.archive.org/web/20230606090611/https://mariokartwii.com/archive/index.php?thread-434.html
 * https://web.archive.org/web/20221202224849/https://gamehacking.org/faqs/wiicodetypes.html
 * https://web.archive.org/web/20230606090655/https://mariokartwii.com/archive/index.php?thread-545.html
 * https://web.archive.org/web/20200608171913/https://geckocodes.org/index.php?arsenal=1
 * https://web.archive.org/web/20230422150103/https://forums.dolphin-emu.org/archive/index.php?thread-23096-2.html
 * https://web.archive.org/web/20221128121054/http://wiigeckocodes.github.io/codetypedocumentation.html
 * https://web.archive.org/web/20230620145237/https://smashboards.com/threads/melee-gecko-codes-guide-and-discussion.327311/page-16
 * https://web.archive.org/web/20230605031725/https://www.zeldacodes.org/downloads/code-manager
 * https://github.com/dolphin-emu/dolphin/blob/579ccb0710935bac3e4751e6a1671f927d81de57/docs/codehandler.s
 * https://github.com/dolphin-emu/dolphin/blob/fe8a9ac70796498e8bc5b29770e9334ed187c35d/Source/Core/Core/GeckoCode.h#L50
 * https://github.com/dolphin-emu/dolphin/blob/fe8a9ac70796498e8bc5b29770e9334ed187c35d/Source/Core/Core/GeckoCode.cpp#L168
 * https://github.com/JoshuaMKW/geckocode-libs/blob/ddbdccb2569742d1f595775001603d5933b57f3a/geckolibs/gct.py#L360
 * https://github.com/emukidid/swiss-gc/blob/0b6f971a1074e13cbc9c2fff80c987fceeac5c77/cube/swiss/source/cheats/cheats.c#L182
 * Viewing Dolphin Emulator's memory using https://github.com/aldelaro5/Dolphin-memory-engine
 */

// NOTE: gecko.h is only tested with codehandler.s

/*
 * GCT code lists are just a binary form of a list of codes which begins with:
 * 00 D0 C0 DE 00 D0 C0 DE (GCT_MAGIC, GCT_MAGIC)
 * and ends with:
 * F0 00 00 00 00 00 00 00 (GCT_END | GCST_ENDOFCODE, 0)
 *
 * Dolphin takes all enabled codes from the INI and stores them into a GCT in memory. Other loaders on real hardware
 * require loading GCT files directly.
 *
 * All codes (including magic and end line) are in big endian.
 */

/*
 * The code handler has a code limit each loader arbitrarily defines. Therefore, gecko.h cannot detect if such limit is
 * hit. Please refer to your loader's implementation for such limitation to avoid going over the limit.
 */

// TODO: Documentation for types and functions
// TODO: Contribution guide in README
// TODO: Gecko 1.8 codes?
// TODO: Make codes for 0-01, 0-02, JAP, and KOR
// TODO: Port PrimeHack codes to gecko.h and replace powerpc assembly with gecko
// TODO: Document gecko.h, make wiki, and/or update README.md
// TODO: Figure out licensing issues where an exception to MIT can be added for contributed codes to be under any license
// TODO: Gecko code dissasembler into gecko.h functions
// TODO: Auto generated macros that have addresses to the codes as they appear as in the codelist in memory
// TODO: Provide way of passing options to each code, to allow for codes configured at runtime

/* ********************************************************************************************************************
 * Common Addresses
 ******************************************************************************************************************* */

// WARNING:
// gecko.h assumes the structure of codehandler.s. Therefore, some features of gecko.h may only be available for it.
// If you want understand this and want these extra features, define the __GECKO_H_CODEHANDLERSCOMPAT__ macro.

// These addresses may be invalid if your loader hooks the codehandler at a completely different address. This assumes
// the address is where Dolphin hooks it. If you want gecko.h to work with a different address, define the
// __GECKO_H_CODEHANDLERADDR__ macro.
#ifndef __GECKO_H_CODEHANDLERADDR__
#define __GECKO_H_CODEHANDLERADDR__ 0x80001800
#endif

#ifdef __GECKO_H_CODEHANDLERSCOMPAT__
// G_GetLinePointer will work incorrectly if the size of the code handler's binary is different. This assumes the size
// of binary is of Dolphin's code handler binary. If you want gecko.h to work with a differently sized codehandlers,
// define the __GECKO_H_CODEHANDLERSIZE__ macro.
#ifndef __GECKO_H_CODEHANDLERSIZE__
#define __GECKO_H_CODEHANDLERSIZE__ 2880
#endif
#else
#ifdef __GECKO_H_CODEHANDLERSIZE__
#undef __GECKO_H_CODEHANDLERSIZE__
#endif
#define __GECKO_H_CODEHANDLERSIZE__ 0
#endif

#define G_ADDR_CODEHANDLER (__GECKO_H_CODEHANDLERADDR__ & 0x00FFFFFF)
#define G_SIZE_CODEHANDLER __GECKO_H_CODEHANDLERSIZE__

// Start address of base address (ba)
#define G_ADDR_BA 0x80000000
// Start address of the stack (GCF_ADDRISSTACK)
#define G_ADDR_STACK 0x81000000

// uint16_t range values for ORing an address or for G_RangeCheck
#define G_ADDR_RNG_BA 0x8000
#define G_ADDR_RNG_STACK 0x8100

// One uint32_t per gecko register
#define G_ADDR_GR0 (G_ADDR_CODEHANDLER + 0x00000008)
#define G_ADDR_GR1 (G_ADDR_CODEHANDLER + 0x0000000C)
#define G_ADDR_GR2 (G_ADDR_CODEHANDLER + 0x00000010)
#define G_ADDR_GR3 (G_ADDR_CODEHANDLER + 0x00000014)
#define G_ADDR_GR4 (G_ADDR_CODEHANDLER + 0x00000018)
#define G_ADDR_GR5 (G_ADDR_CODEHANDLER + 0x0000001C)
#define G_ADDR_GR6 (G_ADDR_CODEHANDLER + 0x00000020)
#define G_ADDR_GR7 (G_ADDR_CODEHANDLER + 0x00000024)
#define G_ADDR_GR8 (G_ADDR_CODEHANDLER + 0x00000028)
#define G_ADDR_GR9 (G_ADDR_CODEHANDLER + 0x0000002C)
#define G_ADDR_GRA (G_ADDR_CODEHANDLER + 0x00000030)
#define G_ADDR_GRB (G_ADDR_CODEHANDLER + 0x00000034)
#define G_ADDR_GRC (G_ADDR_CODEHANDLER + 0x00000038)
#define G_ADDR_GRD (G_ADDR_CODEHANDLER + 0x0000003C)
#define G_ADDR_GRE (G_ADDR_CODEHANDLER + 0x00000040)
#define G_ADDR_GRF (G_ADDR_CODEHANDLER + 0x00000044)

// Two uint32_t per block:
// [0] = Next code address
// [1] = Number of times to repeat
#define G_ADDR_GB0 (G_ADDR_CODEHANDLER + 0x00000048)
#define G_ADDR_GB1 (G_ADDR_CODEHANDLER + 0x00000050)
#define G_ADDR_GB2 (G_ADDR_CODEHANDLER + 0x00000058)
#define G_ADDR_GB3 (G_ADDR_CODEHANDLER + 0x00000060)
#define G_ADDR_GB4 (G_ADDR_CODEHANDLER + 0x00000068)
#define G_ADDR_GB5 (G_ADDR_CODEHANDLER + 0x00000070)
#define G_ADDR_GB6 (G_ADDR_CODEHANDLER + 0x00000078)
#define G_ADDR_GB7 (G_ADDR_CODEHANDLER + 0x00000080)
#define G_ADDR_GB8 (G_ADDR_CODEHANDLER + 0x00000088)
#define G_ADDR_GB9 (G_ADDR_CODEHANDLER + 0x00000090)
#define G_ADDR_GBA (G_ADDR_CODEHANDLER + 0x00000098)

/* ********************************************************************************************************************
 * gecko.h Types
 ******************************************************************************************************************* */

typedef enum __GCodeType {
    GCT_WRITE =   0,
    GCT_REGIF =   (0x1 << 29),
    GCT_BAORPO =  (0x2 << 29),
    GCT_CTRLFLW = (0x3 << 29),
    GCT_GR =      (0x4 << 29),
    GCT_SPECIF =  (0x5 << 29),
    GCT_MISC =    (0x6 << 29),
    GCT_END =     (0x7 << 29),
    GCT_MAGIC =   0x00D0C0DE
} GCodeType;

typedef enum __GCodeSubType {
    // GCT_WRITE
    GCST_WRITE8 =       0,
    GCST_WRITE16 =      (0x1 << 25),
    GCST_WRITE32 =      (0x2 << 25),
    GCST_WRITESTR =     (0x3 << 25),
    GCST_WRITESRL =     (0x4 << 25),
    // GCT_REGIF
    GCST_IF32EQU =      0,
    GCST_IF32NEQ =      (0x1 << 25),
    GCST_IF32GTR =      (0x2 << 25),
    GCST_IF32LSS =      (0x3 << 25),
    GCST_IF16EQU =      (0x4 << 25),
    GCST_IF16NEQ =      (0x5 << 25),
    GCST_IF16GTR =      (0x6 << 25),
    GCST_IF16LSS =      (0x7 << 25),
    // GCT_BAORPO
    GCST_BAREAD =       0,
    GCST_BASET =        (0x1 << 25),
    GCST_BAWRITE =      (0x2 << 25),
    GCST_BASETCODE =    (0x3 << 25),
    GCST_POREAD =       (0x4 << 25),
    GCST_POSET =        (0x5 << 25),
    GCST_POWRITE =      (0x6 << 25),
    GCST_POSETCODE =    (0x7 << 25),
    // GCT_CTRLFLW
    GCST_REPEATSET =    0,
    GCST_REPEATEXEC =   (0x1 << 25),
    GCST_RETURN =       (0x2 << 25),
    GCST_GOTO =         (0x3 << 25),
    GCST_GOSUB =        (0x4 << 25),
    // GCT_GR
    GCST_GRSET =        0,
    GCST_GRREAD =       (0x1 << 25),
    GCST_GRWRITE =      (0x2 << 25),
    GCST_GRDIRECTOP =   (0x3 << 25),
    GCST_GROP =         (0x4 << 25),
    GCST_MEMCPYFROMGR = (0x5 << 25),
    GCST_MEMCPYTOGR =   (0x6 << 25),
    // GCT_SPECIF
    GCST_IFGR16EQU =    0,
    GCST_IFGR16NEQ =    (0x1 << 25),
    GCST_IFGR16GTR =    (0x2 << 25),
    GCST_IFGR16LSS =    (0x3 << 25),
    GCST_IFCNTR16EQU =  (0x4 << 25),
    GCST_IFCNTR16NEQ =  (0x5 << 25),
    GCST_IFCNTR16GTR =  (0x6 << 25),
    GCST_IFCNTR16LSS =  (0x7 << 25),
    // GCT_MISC
    GCST_ASMEXEC =      0,
    GCST_ASMINST =      (0x1 << 25),
    GCST_ASMBRCH =      (0x3 << 25),
    GCST_SWITCH =       (0x6 << 25),
    GCST_RNGCHCK =      (0x7 << 25),
    // GCT_END
    GCST_FULLTERM =     0,
    GCST_ENDIFELSE =    (0x1 << 25),
    GCST_ENDOFCODE =    (0x8 << 25)
    // GCT_MAGIC
    // None
} GCodeSubType;

typedef enum __GCodeFlags {
    GCF_NONE = 0,
    GCF_ADDRISSTACK = (1 << 24),
    GCF_USEPOINTER =  (1 << 28)
} GCodeFlags;

typedef enum __GOffsetFlags {
    GOF_NONE =          0,
    GOF_GECKOREG =      (1 << 12),
    GOF_PTRORBASEADDR = (1 << 16),
    GOF_ADDTO =         (1 << 20)
} GOffsetFlags;

typedef enum __GIfCounterFlags {
    GICF_NONE =       0,
    GICF_CNDINVERSE = 1,
    GICF_ENDIF =      (1 << 3),
} GIfCounterFlags;

typedef enum __GExecStat {
    GES_TRUE =   0,
    GES_FALSE =  (0x1 << 20),
    GES_EITHER = (0x2 << 20),
    GES_NONE =   0xFF
} GExecStat;

typedef enum __GRegisterDataType {
    GRDT_8 =  0,
    GRDT_16 = (0x1 << 20),
    GRDT_32 = (0x2 << 20)
} GRegisterDataType;

typedef enum __GSerialDataType {
    GSDT_8 =  0,
    GSDT_16 = (0x1 << 28),
    GSDT_32 = (0x2 << 28)
} GSerialDataType;

typedef enum __GRegisterOp {
    GRO_ADD =              0,
    GRO_MULTIPLY =         (0x1 << 20),
    GRO_OR =               (0x2 << 20),
    GRO_AND =              (0x3 << 20),
    GRO_XOR =              (0x4 << 20),
    GRO_SHIFTLEFT =        (0x5 << 20),
    GRO_SHIFTRIGHT =       (0x6 << 20),
    GRO_ROTATELEFT =       (0x7 << 20),
    GRO_SIGNEDSHIFTRIGHT = (0x8 << 20),
    GRO_FLOATADD =         (0x9 << 20),
    GRO_FLOATMULTIPLY =    (0xA << 20)
} GRegisterOp;

typedef enum __GRegisterOpType {
    GROT_SRCVALUE_DSTVALUE = 0,
    GROT_SRCDEREF_DSTVALUE = (0x1 << 16),
    GROT_SRCVALUE_DSTDEREF = (0x2 << 16),
    GROT_SRCDEREF_DSTDEREF = (0x3 << 16)
} GRegisterOpType;

typedef enum __GRegister {
    GR_0 =    0,
    GR_1 =    0x1,
    GR_2 =    0x2,
    GR_3 =    0x3,
    GR_4 =    0x4,
    GR_5 =    0x5,
    GR_6 =    0x6,
    GR_7 =    0x7,
    GR_8 =    0x8,
    GR_9 =    0x9,
    GR_10 =   0xA,
    GR_11 =   0xB,
    GR_12 =   0xC,
    GR_13 =   0xD,
    GR_14 =   0xE,
    GR_15 =   0xF,
    GR_NONE = 0xFF
} GRegister;

typedef enum __GBlock {
    GB_0 =    0,
    GB_1 =    0x1,
    GB_2 =    0x2,
    GB_3 =    0x3,
    GB_4 =    0x4,
    GB_5 =    0x5,
    GB_6 =    0x6,
    GB_7 =    0x7,
    GB_8 =    0x8,
    GB_9 =    0x9,
    GB_10 =   0xA,
    GB_NONE = 0xFF
} GBlock;

/* ********************************************************************************************************************
 * Code Output Functionality
 ******************************************************************************************************************* */

extern FILE *G_OutputHandle;
extern uint8_t G_IsOutputBin;

/* ********************************************************************************************************************
 * Label and Line Pointer Functionality
 ******************************************************************************************************************* */

extern int16_t G_CurProcLblLine;
extern uint8_t G_IsProcLabels;

#ifdef __GECKO_H_CODEHANDLERSCOMPAT__
extern uint32_t G_CurProcPtrLine;
extern uint8_t G_IsProcLinePtrs;
#endif

// Must be somewhere before a G_BeginCode
#define G_DeclareLabel(l) int16_t l = 0

// Must be somewhere before a G_EndCode
#ifdef __GECKO_H_CODEHANDLERSCOMPAT__
#define G_BeginCode() \
for (uint8_t __G_ProcessPass__ = 0; __G_ProcessPass__ < 3; __G_ProcessPass__++) { \
    G_CurProcLblLine = 0; \
    G_CurProcPtrLine = 0; \
    G_IsProcLabels = (__G_ProcessPass__ == 0); \
    G_IsProcLinePtrs = (__G_ProcessPass__ == 1)
#else
#define G_BeginCode() \
for (uint8_t __G_ProcessPass__ = 0; __G_ProcessPass__ < 2; __G_ProcessPass__++) { \
    G_CurProcLblLine = 0; \
    G_IsProcLabels = (__G_ProcessPass__ == 0)
#endif

// Must be somewhere between a G_BeginCode and G_EndCode
#define G_DefineLabel(l) \
if (G_IsProcLabels) \
    l = G_CurProcLblLine

// Must be somewhere after a G_BeginCode
#define G_EndCode() }

// Must be somewhere between a G_BeginCode and G_EndCode
int16_t G_GetLabel(int16_t label);

#ifdef __GECKO_H_CODEHANDLERSCOMPAT__
// Must be somewhere between a G_BeginCode and G_EndCode
uint32_t G_GetLinePointer(void);
#endif

/* ********************************************************************************************************************
 * CT0: Write
 ******************************************************************************************************************* */

void __G_Write__(GCodeSubType typ, uint32_t addr, uint32_t val, uint32_t extraCount, GCodeFlags flg);

INLINE void G_Write8(uint32_t addr, uint8_t val, GCodeFlags flg) {
    __G_Write__(GCST_WRITE8, addr, val, 0, flg);
}

INLINE void G_Extra_Write8(uint32_t addr, uint8_t val, uint16_t extraCount, GCodeFlags flg) {
    __G_Write__(GCST_WRITE8, addr, val, extraCount, flg);
}

INLINE void G_Write16(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_Write__(GCST_WRITE16, addr, val, 0, flg);
}

INLINE void G_Extra_Write16(uint32_t addr, uint16_t val, uint16_t extraCount, GCodeFlags flg) {
    __G_Write__(GCST_WRITE16, addr, val, extraCount, flg);
}

INLINE void G_Write32(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_Write__(GCST_WRITE32, addr, val, 0, flg);
}

void G_WriteString(uint32_t addr, uint16_t valsSz, uint8_t *vals, GCodeFlags flg);

void __G_WriteSerial__(GSerialDataType sdType, uint32_t addr, uint32_t value, uint32_t count, uint32_t addrIncr,
uint32_t valueIncr, GCodeFlags flg);

INLINE void G_WriteSerial8(uint32_t addr, uint8_t value, uint16_t count, uint16_t addrIncr, uint32_t valueIncr,
GCodeFlags flg) {
    __G_WriteSerial__(GSDT_8, addr, value, count, addrIncr, valueIncr, flg);
}

INLINE void G_WriteSerial16(uint32_t addr, uint16_t value, uint16_t count, uint16_t addrIncr, uint32_t valueIncr,
GCodeFlags flg) {
    __G_WriteSerial__(GSDT_16, addr, value, count, addrIncr, valueIncr, flg);
}

INLINE void G_WriteSerial32(uint32_t addr, uint32_t value, uint16_t count, uint16_t addrIncr, uint32_t valueIncr,
GCodeFlags flg) {
    __G_WriteSerial__(GSDT_32, addr, value, count, addrIncr, valueIncr, flg);
}

/* ********************************************************************************************************************
 * CT1: Regular If
 ******************************************************************************************************************* */

/*
 * Notes:
 * - All forms of Regular If are unsigned checks
 * - For mask, it is applied at runtime as: *addr & ~mask ==/!=/>/< val
 */

void __G_If__(GCodeSubType typ, uint32_t addr, uint32_t val, uint32_t endif, uint32_t mask, GCodeFlags flg);

INLINE void G_If32Equal(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_If__(GCST_IF32EQU, addr, val, 0, 0, flg);
}

INLINE void G_Endif_If32Equal(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_If__(GCST_IF32EQU, addr, val, 1, 0, flg);
}

INLINE void G_If32NotEqual(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_If__(GCST_IF32NEQ, addr, val, 0, 0, flg);
}

INLINE void G_Endif_If32NotEqual(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_If__(GCST_IF32NEQ, addr, val, 1, 0, flg);
}

INLINE void G_If32GreaterThan(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_If__(GCST_IF32GTR, addr, val, 0, 0, flg);
}

INLINE void G_Endif_If32GreaterThan(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_If__(GCST_IF32GTR, addr, val, 1, 0, flg);
}

INLINE void G_If32LessThan(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_If__(GCST_IF32LSS, addr, val, 0, 0, flg);
}

INLINE void G_Endif_If32LessThan(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_If__(GCST_IF32LSS, addr, val, 1, 0, flg);
}

INLINE void G_If16Equal(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_If__(GCST_IF16EQU, addr, val, 0, 0, flg);
}

INLINE void G_Endif_If16Equal(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_If__(GCST_IF16EQU, addr, val, 1, 0, flg);
}

INLINE void G_If16EqualMask(uint32_t addr, uint16_t val, uint16_t mask, GCodeFlags flg) {
    __G_If__(GCST_IF16EQU, addr, val, 0, mask, flg);
}

INLINE void G_Endif_If16EqualMask(uint32_t addr, uint16_t val, uint16_t mask, GCodeFlags flg) {
    __G_If__(GCST_IF16EQU, addr, val, 1, mask, flg);
}

INLINE void G_If16NotEqual(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_If__(GCST_IF16NEQ, addr, val, 0, 0, flg);
}

INLINE void G_Endif_If16NotEqual(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_If__(GCST_IF16NEQ, addr, val, 1, 0, flg);
}

INLINE void G_If16NotEqualMask(uint32_t addr, uint16_t val, uint16_t mask, GCodeFlags flg) {
    __G_If__(GCST_IF16NEQ, addr, val, 0, mask, flg);
}

INLINE void G_Endif_If16NotEqualMask(uint32_t addr, uint16_t val, uint16_t mask, GCodeFlags flg) {
    __G_If__(GCST_IF16NEQ, addr, val, 1, mask, flg);
}

INLINE void G_If16GreaterThan(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_If__(GCST_IF16GTR, addr, val, 0, 0, flg);
}

INLINE void G_Endif_If16GreaterThan(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_If__(GCST_IF16GTR, addr, val, 1, 0, flg);
}

INLINE void G_If16GreaterThanMask(uint32_t addr, uint16_t val, uint16_t mask, GCodeFlags flg) {
    __G_If__(GCST_IF16GTR, addr, val, 0, mask, flg);
}

INLINE void G_Endif_If16GreaterThanMask(uint32_t addr, uint16_t val, uint16_t mask, GCodeFlags flg) {
    __G_If__(GCST_IF16GTR, addr, val, 1, mask, flg);
}

INLINE void G_If16LessThan(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_If__(GCST_IF16LSS, addr, val, 0, 0, flg);
}

INLINE void G_Endif_If16LessThan(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_If__(GCST_IF16LSS, addr, val, 1, 0, flg);
}

INLINE void G_If16LessThanMask(uint32_t addr, uint16_t val, uint16_t mask, GCodeFlags flg) {
    __G_If__(GCST_IF16LSS, addr, val, 0, mask, flg);
}

INLINE void G_Endif_If16LessThanMask(uint32_t addr, uint16_t val, uint16_t mask, GCodeFlags flg) {
    __G_If__(GCST_IF16LSS, addr, val, 1, mask, flg);
}

/* ********************************************************************************************************************
 * CT2: Base Address (ba) or Pointer Address (po)
 ******************************************************************************************************************* */

void __G_BAOrPO__(GCodeSubType typ, uint32_t addr, GRegister gr, GOffsetFlags oFlg, GCodeFlags flg);

INLINE void G_ReadBA(uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_BAOrPO__(GCST_BAREAD, addr, GR_NONE, oFlg, flg);
}

INLINE void G_ReadBAGR(uint32_t addr, GRegister gr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_BAOrPO__(GCST_BAREAD, addr, gr, oFlg, flg);
}

INLINE void G_SetBA(uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_BAOrPO__(GCST_BASET, addr, GR_NONE, oFlg, flg);
}

INLINE void G_SetBAGR(uint32_t addr, GRegister gr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_BAOrPO__(GCST_BASET, addr, gr, oFlg, flg);
}

INLINE void G_WriteBA(uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_BAOrPO__(GCST_BAWRITE, addr, GR_NONE, oFlg, flg);
}

INLINE void G_WriteBAGR(uint32_t addr, GRegister gr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_BAOrPO__(GCST_BAWRITE, addr, gr, oFlg, flg);
}

INLINE void G_ReadPO(uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_BAOrPO__(GCST_POREAD, addr, GR_NONE, oFlg, flg);
}

INLINE void G_ReadPOGR(uint32_t addr, GRegister gr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_BAOrPO__(GCST_POREAD, addr, gr, oFlg, flg);
}

INLINE void G_SetPO(uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_BAOrPO__(GCST_POSET, addr, GR_NONE, oFlg, flg);
}

INLINE void G_SetPOGR(uint32_t addr, GRegister gr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_BAOrPO__(GCST_POSET, addr, gr, oFlg, flg);
}

INLINE void G_WritePO(uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_BAOrPO__(GCST_POWRITE, addr, GR_NONE, oFlg, flg);
}

INLINE void G_WritePOGR(uint32_t addr, GRegister gr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_BAOrPO__(GCST_POWRITE, addr, gr, oFlg, flg);
}

void __G_SetBAOrPOToCodeAddress__(GCodeSubType typ, int16_t offs);

INLINE void G_SetBAToCodeAddress(int16_t offs) {
    __G_SetBAOrPOToCodeAddress__(GCST_BASETCODE, offs);
}

INLINE void G_SetPOToCodeAddress(int16_t offs) {
    __G_SetBAOrPOToCodeAddress__(GCST_POSETCODE, offs);
}

/* ********************************************************************************************************************
 * CT3: Control Flow (Repeat, bN)
 ******************************************************************************************************************* */

void __G_ControlFlow__(GCodeSubType typ, uint32_t cnt, int16_t offs, GBlock block, GExecStat exec);

INLINE void G_SetRepeat(uint16_t cnt, GBlock block) {
    __G_ControlFlow__(GCST_REPEATSET, cnt, 1, block, GES_NONE);
}

INLINE void G_ExecuteRepeat(GBlock block) {
    __G_ControlFlow__(GCST_REPEATEXEC, 0, 1, block, GES_NONE);
}

INLINE void G_ReturnIfTrue(GBlock block) {
    __G_ControlFlow__(GCST_RETURN, 0, 1, block, GES_TRUE);
}

INLINE void G_ReturnIfFalse(GBlock block) {
    __G_ControlFlow__(GCST_RETURN, 0, 1, block, GES_FALSE);
}

INLINE void G_Return(GBlock block) {
    __G_ControlFlow__(GCST_RETURN, 0, 1, block, GES_EITHER);
}

INLINE void G_GotoIfTrue(int16_t offs) {
    __G_ControlFlow__(GCST_GOTO, 0, offs, GB_NONE, GES_TRUE);
}

INLINE void G_GotoIfFalse(int16_t offs) {
    __G_ControlFlow__(GCST_GOTO, 0, offs, GB_NONE, GES_FALSE);
}

INLINE void G_Goto(int16_t offs) {
    __G_ControlFlow__(GCST_GOTO, 0, offs, GB_NONE, GES_EITHER);
}

INLINE void G_GosubIfTrue(int16_t offs, GBlock block) {
    __G_ControlFlow__(GCST_GOSUB, 0, offs, block, GES_TRUE);
}

INLINE void G_GosubIfFalse(int16_t offs, GBlock block) {
    __G_ControlFlow__(GCST_GOSUB, 0, offs, block, GES_FALSE);
}

INLINE void G_Gosub(int16_t offs, GBlock block) {
    __G_ControlFlow__(GCST_GOSUB, 0, offs, block, GES_EITHER);
}

/* ********************************************************************************************************************
 * CT4: Gecko Register (grN)
 ******************************************************************************************************************* */

void __G_GR__(GCodeSubType typ, GRegister gr, GRegisterDataType rdType, uint32_t addrOrVal, GOffsetFlags oFlg,
GCodeFlags flg);

INLINE void G_SetGR(GRegister gr, uint32_t val, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRSET, gr, GRDT_8, val, oFlg, flg);
}

INLINE void G_ReadGR8(GRegister gr, uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRREAD, gr, GRDT_8, addr, oFlg, flg);
}

INLINE void G_ReadGR16(GRegister gr, uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRREAD, gr, GRDT_16, addr, oFlg, flg);
}

INLINE void G_ReadGR32(GRegister gr, uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRREAD, gr, GRDT_32, addr, oFlg, flg);
}

INLINE void G_WriteGR8(GRegister gr, uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRWRITE, gr, GRDT_8, addr, oFlg, flg);
}

INLINE void G_WriteGR16(GRegister gr, uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRWRITE, gr, GRDT_16, addr, oFlg, flg);
}

INLINE void G_WriteGR32(GRegister gr, uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRWRITE, gr, GRDT_32, addr, oFlg, flg);
}

void __G_GROperation__(GCodeSubType typ, GRegister grn, GRegister grk, GRegisterOp op, GRegisterOpType ref,
uint32_t val);

INLINE void G_GRAddDirect(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_ADD, ref, val);
}

INLINE void G_GRMultiplyDirect(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_MULTIPLY, ref, val);
}

INLINE void G_GRORDirect(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_OR, ref, val);
}

INLINE void G_GRANDDirect(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_AND, ref, val);
}

INLINE void G_GRXORDirect(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_XOR, ref, val);
}

INLINE void G_GRShiftLeftDirect(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_SHIFTLEFT, ref, val);
}

INLINE void G_GRShiftRightDirect(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_SHIFTRIGHT, ref, val);
}

INLINE void G_GRRotateLeftDirect(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_ROTATELEFT, ref, val);
}

INLINE void G_GRSignedShiftRightDirect(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_SIGNEDSHIFTRIGHT, ref, val);
}

INLINE void G_GRFloatAddDirect(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_FLOATADD, ref, val);
}

INLINE void G_GRFloatMultiplyDirect(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_FLOATMULTIPLY, ref, val);
}

INLINE void G_GRAdd(GRegister grn, GRegister grk, GRegisterOpType ref) {
    __G_GROperation__(GCST_GROP, grn, grk, GRO_ADD, ref, 0);
}

INLINE void G_GRMultiply(GRegister grn, GRegister grk, GRegisterOpType ref) {
    __G_GROperation__(GCST_GROP, grn, grk, GRO_MULTIPLY, ref, 0);
}

INLINE void G_GROR(GRegister grn, GRegister grk, GRegisterOpType ref) {
    __G_GROperation__(GCST_GROP, grn, grk, GRO_OR, ref, 0);
}

INLINE void G_GRAND(GRegister grn, GRegister grk, GRegisterOpType ref) {
    __G_GROperation__(GCST_GROP, grn, grk, GRO_AND, ref, 0);
}

INLINE void G_GRXOR(GRegister grn, GRegister grk, GRegisterOpType ref) {
    __G_GROperation__(GCST_GROP, grn, grk, GRO_XOR, ref, 0);
}

INLINE void G_GRShiftLeft(GRegister grn, GRegister grk, GRegisterOpType ref) {
    __G_GROperation__(GCST_GROP, grn, grk, GRO_SHIFTLEFT, ref, 0);
}

INLINE void G_GRShiftRight(GRegister grn, GRegister grk, GRegisterOpType ref) {
    __G_GROperation__(GCST_GROP, grn, grk, GRO_SHIFTRIGHT, ref, 0);
}

INLINE void G_GRRotateLeft(GRegister grn, GRegister grk, GRegisterOpType ref) {
    __G_GROperation__(GCST_GROP, grn, grk, GRO_ROTATELEFT, ref, 0);
}

INLINE void G_GRSignedShiftRight(GRegister grn, GRegister grk, GRegisterOpType ref) {
    __G_GROperation__(GCST_GROP, grn, grk, GRO_SIGNEDSHIFTRIGHT, ref, 0);
}

INLINE void G_GRFloatAdd(GRegister grn, GRegister grk, GRegisterOpType ref) {
    __G_GROperation__(GCST_GROP, grn, grk, GRO_FLOATADD, ref, 0);
}

INLINE void G_GRFloatMultiply(GRegister grn, GRegister grk, GRegisterOpType ref) {
    __G_GROperation__(GCST_GROP, grn, grk, GRO_FLOATMULTIPLY, ref, 0);
}

void __G_CopyMem__(GCodeSubType typ, GRegister grn, GRegister grk, uint32_t addr, uint32_t cnt, GCodeFlags flg);

// *grn -> mem
INLINE void G_CopyGRDerefToMem(GRegister gr, uint32_t addr, uint16_t cnt, GCodeFlags flg) {
    __G_CopyMem__(GCST_MEMCPYFROMGR, gr, GR_NONE, addr, cnt, flg);
}

// *grn -> mem + *grk
INLINE void G_CopyGRDerefToGRDerefPlusMem(GRegister grn, GRegister grk, uint32_t addr, uint16_t cnt, GCodeFlags flg) {
    __G_CopyMem__(GCST_MEMCPYFROMGR, grn, grk, addr, cnt, flg);
}

// mem -> *grk
INLINE void G_CopyMemToGRDeref(GRegister gr, uint32_t addr, uint16_t cnt, GCodeFlags flg) {
    __G_CopyMem__(GCST_MEMCPYTOGR, GR_NONE, gr, addr, cnt, flg);
}

// mem + *grn -> grk
INLINE void G_CopyGRDerefPlusMemToGRDeref(GRegister grn, GRegister grk, uint32_t addr, uint16_t cnt, GCodeFlags flg) {
    __G_CopyMem__(GCST_MEMCPYTOGR, grn, grk, addr, cnt, flg);
}

/* ********************************************************************************************************************
 * CT5: Special If
 ******************************************************************************************************************* */

/*
 * Notes:
 * - All forms of Special If are unsigned checks
 * - For mask, it is applied at runtime as: *grn & ~mask ==/!=/>/< *grk & ~mask
 * - For mask in counter if, it is applied at runtime as: counter ==/!=/>/< max & ~mask
 * - For counter if, the counter is stored inside the code type itself, as it is stored in memory in the code list.
 *   XX | XXXX | XXXXXXXX
 *        ^^^^
 *        Counter
 */

void __G_SpecIf__(GCodeSubType typ, GRegister grn, GRegister grk, uint32_t addr, uint32_t endif, uint32_t mask,
GCodeFlags flg);

INLINE void G_If16GREqualDirect(GRegister gr, uint32_t addr, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16EQU, gr, GR_NONE, addr, 0, 0, flg);
}

INLINE void G_Endif_If16GREqualDirect(GRegister gr, uint32_t addr, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16EQU, gr, GR_NONE, addr, 1, 0, flg);
}

INLINE void G_If16GREqualDirectMask(GRegister gr, uint32_t addr, uint16_t mask, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16EQU, gr, GR_NONE, addr, 0, mask, flg);
}

INLINE void G_Endif_If16GREqualDirectMask(GRegister gr, uint32_t addr, uint16_t mask, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16EQU, gr, GR_NONE, addr, 1, mask, flg);
}

INLINE void G_If16GREqual(GRegister grn, GRegister grk) {
    __G_SpecIf__(GCST_IFGR16EQU, grn, grk, 0, 0, 0, GCF_NONE);
}

INLINE void G_Endif_If16GREqual(GRegister grn, GRegister grk) {
    __G_SpecIf__(GCST_IFGR16EQU, grn, grk, 0, 1, 0, GCF_NONE);
}

INLINE void G_If16GREqualMask(GRegister grn, GRegister grk, uint16_t mask) {
    __G_SpecIf__(GCST_IFGR16EQU, grn, grk, 0, 0, mask, GCF_NONE);
}

INLINE void G_Endif_If16GREqualMask(GRegister grn, GRegister grk, uint16_t mask) {
    __G_SpecIf__(GCST_IFGR16EQU, grn, grk, 0, 1, mask, GCF_NONE);
}

INLINE void G_If16GRNotEqualDirect(GRegister gr, uint32_t addr, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16NEQ, gr, GR_NONE, addr, 0, 0, flg);
}

INLINE void G_Endif_If16GRNotEqualDirect(GRegister gr, uint32_t addr, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16NEQ, gr, GR_NONE, addr, 1, 0, flg);
}

INLINE void G_If16GRNotEqualDirectMask(GRegister gr, uint32_t addr, uint16_t mask, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16NEQ, gr, GR_NONE, addr, 0, mask, flg);
}

INLINE void G_Endif_If16GRNotEqualDirectMask(GRegister gr, uint32_t addr, uint16_t mask, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16NEQ, gr, GR_NONE, addr, 1, mask, flg);
}

INLINE void G_If16GRNotEqual(GRegister grn, GRegister grk) {
    __G_SpecIf__(GCST_IFGR16NEQ, grn, grk, 0, 0, 0, GCF_NONE);
}

INLINE void G_Endif_If16GRNotEqual(GRegister grn, GRegister grk) {
    __G_SpecIf__(GCST_IFGR16NEQ, grn, grk, 0, 1, 0, GCF_NONE);
}

INLINE void G_If16GRNotEqualMask(GRegister grn, GRegister grk, uint16_t mask) {
    __G_SpecIf__(GCST_IFGR16NEQ, grn, grk, 0, 0, mask, GCF_NONE);
}

INLINE void G_Endif_If16GRNotEqualMask(GRegister grn, GRegister grk, uint16_t mask) {
    __G_SpecIf__(GCST_IFGR16NEQ, grn, grk, 0, 1, mask, GCF_NONE);
}

INLINE void G_If16GRGreaterThanDirect(GRegister gr, uint32_t addr, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16GTR, gr, GR_NONE, addr, 0, 0, flg);
}

INLINE void G_Endif_If16GRGreaterThanDirect(GRegister gr, uint32_t addr, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16GTR, gr, GR_NONE, addr, 1, 0, flg);
}

INLINE void G_If16GRGreaterThanDirectMask(GRegister gr, uint32_t addr, uint16_t mask, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16GTR, gr, GR_NONE, addr, 0, mask, flg);
}

INLINE void G_Endif_If16GRGreaterThanDirectMask(GRegister gr, uint32_t addr, uint16_t mask, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16GTR, gr, GR_NONE, addr, 1, mask, flg);
}

INLINE void G_If16GRGreaterThan(GRegister grn, GRegister grk) {
    __G_SpecIf__(GCST_IFGR16GTR, grn, grk, 0, 0, 0, GCF_NONE);
}

INLINE void G_Endif_If16GRGreaterThan(GRegister grn, GRegister grk) {
    __G_SpecIf__(GCST_IFGR16GTR, grn, grk, 0, 1, 0, GCF_NONE);
}

INLINE void G_If16GRGreaterThanMask(GRegister grn, GRegister grk, uint16_t mask) {
    __G_SpecIf__(GCST_IFGR16GTR, grn, grk, 0, 0, mask, GCF_NONE);
}

INLINE void G_Endif_If16GRGreaterThanMask(GRegister grn, GRegister grk, uint16_t mask) {
    __G_SpecIf__(GCST_IFGR16GTR, grn, grk, 0, 1, mask, GCF_NONE);
}

INLINE void G_If16GRLessThanDirect(GRegister gr, uint32_t addr, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16LSS, gr, GR_NONE, addr, 0, 0, flg);
}

INLINE void G_Endif_If16GRLessThanDirect(GRegister gr, uint32_t addr, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16LSS, gr, GR_NONE, addr, 1, 0, flg);
}

INLINE void G_If16GRLessThanDirectMask(GRegister gr, uint32_t addr, uint16_t mask, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16LSS, gr, GR_NONE, addr, 0, mask, flg);
}

INLINE void G_Endif_If16GRLessThanDirectMask(GRegister gr, uint32_t addr, uint16_t mask, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16LSS, gr, GR_NONE, addr, 1, mask, flg);
}

INLINE void G_If16GRLessThan(GRegister grn, GRegister grk) {
    __G_SpecIf__(GCST_IFGR16LSS, grn, grk, 0, 0, 0, GCF_NONE);
}

INLINE void G_Endif_If16GRLessThan(GRegister grn, GRegister grk) {
    __G_SpecIf__(GCST_IFGR16LSS, grn, grk, 0, 1, 0, GCF_NONE);
}

INLINE void G_If16GRLessThanMask(GRegister grn, GRegister grk, uint16_t mask) {
    __G_SpecIf__(GCST_IFGR16LSS, grn, grk, 0, 0, mask, GCF_NONE);
}

INLINE void G_Endif_If16GRLessThanMask(GRegister grn, GRegister grk, uint16_t mask) {
    __G_SpecIf__(GCST_IFGR16LSS, grn, grk, 0, 1, mask, GCF_NONE);
}

void __G_IfCounter__(GCodeSubType typ, uint32_t counter, uint32_t max, uint32_t mask, GIfCounterFlags cFlg);

INLINE void G_IfCounterEqual(uint16_t counter, uint16_t max, uint16_t mask, GIfCounterFlags cFlg) {
    __G_IfCounter__(GCST_IFCNTR16EQU, counter, max, 0, cFlg);
}

INLINE void G_IfCounterNotEqual(uint16_t counter, uint16_t max, uint16_t mask, GIfCounterFlags cFlg) {
    __G_IfCounter__(GCST_IFCNTR16NEQ, counter, max, 0, cFlg);
}

INLINE void G_IfCounterGreaterThan(uint16_t counter, uint16_t max, uint16_t mask, GIfCounterFlags cFlg) {
    __G_IfCounter__(GCST_IFCNTR16GTR, counter, max, 0, cFlg);
}

INLINE void G_IfCounterLessThan(uint16_t counter, uint16_t max, uint16_t mask, GIfCounterFlags cFlg) {
    __G_IfCounter__(GCST_IFCNTR16LSS, counter, max, 0, cFlg);
}

/* ********************************************************************************************************************
 * CT6: Misc
 ******************************************************************************************************************* */

// The last instruction MUST be: blr (0x4E800020)
// This is not handled automatically. Please ensure this if you are compiling your ppc!
void G_ExecuteAssembly(uint32_t valsSz, uint32_t *vals);

// The last instruction MUST be: 0x00000000, if line count even, add on 0x00000000 to code and have a nop (0x60000000)
// behind it
// This is handled automatically.
void G_InsertAssembly(uint32_t addr, uint32_t valsSz, uint32_t *vals, GCodeFlags flg);

void G_CreateBranch(uint32_t addr, uint32_t branch, GCodeFlags flg);

void G_Switch(void);

void __G_RangeCheck__(uint32_t startAddr, uint32_t endAddr, uint32_t endif, GCodeFlags flg);

INLINE void G_RangeCheck(uint16_t startAddr, uint16_t endAddr, GCodeFlags flg) {
    __G_RangeCheck__(startAddr, endAddr, 0, flg);
}

INLINE void G_Endif_RangeCheck(uint16_t startAddr, uint16_t endAddr, GCodeFlags flg) {
    __G_RangeCheck__(startAddr, endAddr, 1, flg);
}

/* ********************************************************************************************************************
 * CT7: End
 ******************************************************************************************************************* */

void __G_End__(GCodeSubType typ, uint32_t ba, uint32_t po, uint32_t endifCount, uint32_t doelse);

INLINE void G_FullTerminator(void) {
    __G_End__(GCST_FULLTERM, 0, 0, 0, 0);
}

INLINE void G_FullTerminatorBAPO(uint16_t ba, uint16_t po) {
    __G_End__(GCST_FULLTERM, ba, po, 0, 0);
}

INLINE void G_Endif(void) {
    __G_End__(GCST_ENDIFELSE, 0, 0, 1, 0);
}

INLINE void G_EndifBAPO(uint16_t ba, uint16_t po) {
    __G_End__(GCST_ENDIFELSE, ba, po, 1, 0);
}

INLINE void G_Endifs(uint16_t endifCount) {
    __G_End__(GCST_ENDIFELSE, 0, 0, endifCount, 0);
}

INLINE void G_EndifsBAPO(uint16_t ba, uint16_t po, uint16_t endifCount) {
    __G_End__(GCST_ENDIFELSE, ba, po, endifCount, 0);
}

INLINE void G_Endif_Else(void) {
    __G_End__(GCST_ENDIFELSE, 0, 0, 1, 1);
}

INLINE void G_EndifBAPO_Else(uint16_t ba, uint16_t po) {
    __G_End__(GCST_ENDIFELSE, ba, po, 1, 1);
}

INLINE void G_Endifs_Else(uint16_t endifCount) {
    __G_End__(GCST_ENDIFELSE, 0, 0, endifCount, 1);
}

INLINE void G_EndifsBAPO_Else(uint16_t ba, uint16_t po, uint16_t endifCount) {
    __G_End__(GCST_ENDIFELSE, ba, po, endifCount, 1);
}

// This should not be used directly (unless you are outputting a GCT code list).
// This is seen at the end of GCT code lists.
// If you want to mark the end of a code, use G_FullTerminator instead (it endifs all ifs and clears execution status).
void G_EndGCT(void);

/* ********************************************************************************************************************
 * Special Extensions
 ******************************************************************************************************************* */

// This should not be used directly (unless you are outputting a GCT code list).
// This is seen at the beginning of GCT code lists.
void G_BeginGCT(void);

INLINE void G_If8Equal(uint32_t addr, uint8_t val, GCodeFlags flg) {
    G_If16EqualMask(addr - 1, val, 0xFF00, flg);
}

INLINE void G_Endif_If8Equal(uint32_t addr, uint8_t val, GCodeFlags flg) {
    G_Endif_If16EqualMask(addr - 1, val, 0xFF00, flg);
}

INLINE void G_If8NotEqual(uint32_t addr, uint32_t val, GCodeFlags flg) {
    G_If16NotEqualMask(addr - 1, val, 0xFF00, flg);
}

INLINE void G_Endif_If8NotEqual(uint32_t addr, uint32_t val, GCodeFlags flg) {
    G_Endif_If16NotEqualMask(addr - 1, val, 0xFF00, flg);
}

INLINE void G_If8GreaterThan(uint32_t addr, uint32_t val, GCodeFlags flg) {
    G_If16GreaterThanMask(addr - 1, val, 0xFF00, flg);
}

INLINE void G_Endif_If8GreaterThan(uint32_t addr, uint32_t val, GCodeFlags flg) {
    G_Endif_If16GreaterThanMask(addr - 1, val, 0xFF00, flg);
}

INLINE void G_If8LessThan(uint32_t addr, uint32_t val, GCodeFlags flg) {
    G_If16LessThanMask(addr - 1, val, 0xFF00, flg);
}

INLINE void G_Endif_If8LessThan(uint32_t addr, uint32_t val, GCodeFlags flg) {
    G_Endif_If16LessThanMask(addr - 1, val, 0xFF00, flg);
}

INLINE void G_NOP() {
    G_SetPO(0, GOF_PTRORBASEADDR, GCF_USEPOINTER);
}
#endif
