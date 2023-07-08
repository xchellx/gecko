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

#ifndef _GECKO_H_
#define _GECKO_H_
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef INLINE
#define INLINE static inline __attribute__((always_inline))
#endif

/*
 * References used:
 * https://web.archive.org/web/20230620145237/https://smashboards.com/threads/melee-gecko-codes-guide-and-discussion.327311/page-16
 * https://web.archive.org/web/20221202224849/https://gamehacking.org/faqs/wiicodetypes.html
 * https://web.archive.org/web/20230606090655/https://mariokartwii.com/archive/index.php?thread-545.html
 * https://web.archive.org/web/20200608171913/https://geckocodes.org/index.php?arsenal=1
 * https://web.archive.org/web/20230422150103/https://forums.dolphin-emu.org/archive/index.php?thread-23096-2.html
 * https://web.archive.org/web/20221128121054/http://wiigeckocodes.github.io/codetypedocumentation.html
 */

/* ********************************************************************************************************************
 * Common Addresses
 ******************************************************************************************************************* */

#define G_ADDR_BA 0x80000000
#define G_ADDR_STACK 0x81000000

#define G_ADDR_RNG_BA 0x8000
#define G_ADDR_RNG_STACK 0x8100

#define G_ADDR_GR0 0x00001808
#define G_ADDR_GR1 0x0000180C
#define G_ADDR_GR2 0x00001810
#define G_ADDR_GR3 0x00001814
#define G_ADDR_GR4 0x00001818
#define G_ADDR_GR5 0x0000181C
#define G_ADDR_GR6 0x00001820
#define G_ADDR_GR7 0x00001824
#define G_ADDR_GR8 0x00001828
#define G_ADDR_GR9 0x0000182C
#define G_ADDR_GRA 0x00001830
#define G_ADDR_GRB 0x00001834
#define G_ADDR_GRC 0x00001838
#define G_ADDR_GRD 0x0000183C
#define G_ADDR_GRE 0x00001840
#define G_ADDR_GRF 0x00001844

#define G_ADDR_BA_GR0 0x80001808
#define G_ADDR_BA_GR1 0x8000180C
#define G_ADDR_BA_GR2 0x80001810
#define G_ADDR_BA_GR3 0x80001814
#define G_ADDR_BA_GR4 0x80001818
#define G_ADDR_BA_GR5 0x8000181C
#define G_ADDR_BA_GR6 0x80001820
#define G_ADDR_BA_GR7 0x80001824
#define G_ADDR_BA_GR8 0x80001828
#define G_ADDR_BA_GR9 0x8000182C
#define G_ADDR_BA_GRA 0x80001830
#define G_ADDR_BA_GRB 0x80001834
#define G_ADDR_BA_GRC 0x80001838
#define G_ADDR_BA_GRD 0x8000183C
#define G_ADDR_BA_GRE 0x80001840
#define G_ADDR_BA_GRF 0x80001844

/* ********************************************************************************************************************
 * gecko.h Types
 ******************************************************************************************************************* */

typedef enum __GCodeType {
    GCT_WRITE =    0,
    GCT_REGIF =   (0x1 << 29),
    GCT_BAORPO =  (0x2 << 29),
    GCT_CTRLFLW = (0x3 << 29),
    GCT_GR =      (0x4 << 29),
    GCT_SPECIF =  (0x5 << 29),
    GCT_MISC =    (0x6 << 29),
    GCT_END =     (0x7 << 29)
} GCodeType;

typedef enum __GCodeSubType {
    // GCT_WRITE
    GCST_WRITE8 =   0,
    GCST_WRITE16 =  (0x1 << 25),
    GCST_WRITE32 =  (0x2 << 25),
    GCST_WRITESTR = (0x3 << 25),
    GCST_WRITESRL = (0x4 << 25),
    // GCT_REGIF
    GCST_IF32EQU = 0,
    GCST_IF32NEQ = (0x1 << 25),
    GCST_IF32GTR = (0x2 << 25),
    GCST_IF32LSS = (0x3 << 25),
    GCST_IF16EQU = (0x4 << 25),
    GCST_IF16NEQ = (0x5 << 25),
    GCST_IF16GTR = (0x6 << 25),
    GCST_IF16LSS = (0x7 << 25),
    // GCT_BAORPO
    GCST_BAREAD =    0,
    GCST_BASET =     (0x1 << 25),
    GCST_BAWRITE =   (0x2 << 25),
    GCST_BASETCODE = (0x3 << 25),
    GCST_POREAD =    (0x4 << 25),
    GCST_POSET =     (0x5 << 25),
    GCST_POWRITE =   (0x6 << 25),
    GCST_POSETCODE = (0x7 << 25),
    // GCT_CTRLFLW
    GCST_REPEATSET =  0,
    GCST_REPEATEXEC = (0x1 << 25),
    GCST_RETURN =     (0x2 << 25),
    GCST_GOTO =       (0x3 << 25),
    GCST_GOSUB =      (0x4 << 25),
    // GCT_GR
    GCST_GRSET =         0,
    GCST_GRREAD =       (0x1 << 25),
    GCST_GRWRITE =      (0x2 << 25),
    GCST_GRDIRECTOP =   (0x3 << 25),
    GCST_GROP =         (0x4 << 25),
    GCST_MEMCPYFROMGR = (0x5 << 25),
    GCST_MEMCPYTOGR =   (0x6 << 25),
    // GCT_SPECIF
    GCST_IFGR16EQU =   0,
    GCST_IFGR16NEQ =   (0x1 << 25),
    GCST_IFGR16GTR =   (0x2 << 25),
    GCST_IFGR16LSS =   (0x3 << 25),
    GCST_IFCNTR16EQU = (0x4 << 25),
    GCST_IFCNTR16NEQ = (0x5 << 25),
    GCST_IFCNTR16GTR = (0x6 << 25),
    GCST_IFCNTR16LSS = (0x7 << 25),
    // GCT_MISC
    GCST_ASMEXEC = 0,
    GCST_ASMINST = (0x1 << 25),
    GCST_ASMBRCH = (0x3 << 25),
    GCST_SWITCH =  (0x6 << 25),
    GCST_RNGCHCK = (0x7 << 25),
    // GCT_END
    GCST_FULLTERM =  0,
    GCST_ENDIFELSE = (0x1 << 25),
    GCST_ENDOFCODE = (0x8 << 25),
} GCodeSubType;

typedef enum __GCodeFlags {
    GCF_NONE = 0,
    GCF_USEPOINTER = (1 << 28),
    GCF_ADDRISSTACK = (1 << 24)
} GCodeFlags;

typedef enum __GOffsetFlags {
    GOF_NONE = 0,
    GOF_GECKOREG = (1 << 12),
    GOF_PTRORBASEADDR = (1 << 16),
    GOF_ADDTO = (1 << 20)
} GOffsetFlags;

typedef enum __GExecStat {
    GES_TRUE =   0,
    GES_FALSE =  (0x1 << 20),
    GES_EITHER = (0x2 << 20),
    GES_NONE = 0xFF
} GExecStat;

typedef enum __GDataType {
    GDT_8 =    0,
    GDT_16 =   (0x1 << 20),
    GDT_32 =   (0x2 << 20)
} GDataType;

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
    GR_0 =  0,
    GR_1 =  0x1,
    GR_2 =  0x2,
    GR_3 =  0x3,
    GR_4 =  0x4,
    GR_5 =  0x5,
    GR_6 =  0x6,
    GR_7 =  0x7,
    GR_8 =  0x8,
    GR_9 =  0x9,
    GR_10 = 0xA,
    GR_11 = 0xB,
    GR_12 = 0xC,
    GR_13 = 0xD,
    GR_14 = 0xE,
    GR_15 = 0xF,
    GR_NONE = 0xFF
} GRegister;

typedef enum __GBlock {
    GB_0 =  0,
    GB_1 =  0x1,
    GB_2 =  0x2,
    GB_3 =  0x3,
    GB_4 =  0x4,
    GB_5 =  0x5,
    GB_6 =  0x6,
    GB_7 =  0x7,
    GB_8 =  0x8,
    GB_9 =  0x9,
    GB_10 = 0xA,
    GB_11 = 0xB,
    GB_12 = 0xC,
    GB_13 = 0xD,
    GB_14 = 0xE,
    GB_15 = 0xF, // TODO: At least, I'm pretty sure it is 0xF max like gr
    GB_NONE = 0xFF
} GBlock;

/* ********************************************************************************************************************
 * Label Functionality
 ******************************************************************************************************************* */

static int16_t G_CurProcLine = 0;
static uint8_t G_IsProcLabels = 0;

// Gecko can only hold up to 231 code lines
// Must be somewhere between a G_BeginCode and G_EndCode
#define __G_CheckIfExec__() \
G_CurProcLine += 1; \
if (G_IsProcLabels || G_CurProcLine > 231) \
    return

// Must be somewhere before a G_BeginCode
#define G_DeclareLabel(l) int16_t l = 0

// Must be somewhere before a G_EndCode
#define G_BeginCode() \
for (uint8_t __G_ProcessPass__ = 0; __G_ProcessPass__ < 2; __G_ProcessPass__++) { \
    G_CurProcLine = 0; \
    G_IsProcLabels = (__G_ProcessPass__ == 0)

// Must be somewhere between a G_BeginCode and G_EndCode
#define G_DefineLabel(l) \
if (G_IsProcLabels) \
    l = G_CurProcLine

// Must be somewhere after a G_BeginCode
#define G_EndCode() }

// Must be somewhere between a G_BeginCode and G_EndCode
int16_t G_GetLabel(int16_t label) {
    if (G_IsProcLabels)
        return 0;
    
    int16_t offs = (int16_t) abs(label - G_CurProcLine);
    if (G_CurProcLine > label)
        offs = -offs;
    return offs;
}

/* ********************************************************************************************************************
 * CT0: Write
 ******************************************************************************************************************* */

void __G_Write__(GCodeSubType typ, uint32_t addr, uint32_t val, uint32_t extraCount, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_WRITE | typ | flg | addr;
    uint32_t geckoVal = (extraCount << 16) | val;
    fprintf(stdout, "%08X %08X\n", gecko, geckoVal);
}

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

// TODO: Implement GCST_WRITESTR

// TODO: Implement GCST_WRITESRL

/* ********************************************************************************************************************
 * CT1: Regular If
 ******************************************************************************************************************* */

/*
 * Notes:
 * All forms of Regular If are unsigned checks
 * For mask, it is applied at runtime as: *addr & ~mask ==/!=/>/< val
 */

void __G_If__(GCodeSubType typ, uint32_t addr, uint32_t val, uint32_t mask, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_REGIF | typ | flg | (addr + 1);
    uint32_t geckoVal = (mask << 16) | val;
    fprintf(stdout, "%08X %08X\n", gecko, geckoVal);
}

INLINE void G_If32Equal(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_If__(GCST_IF32EQU, addr, val, 0, flg);
}

INLINE void G_If32NotEqual(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_If__(GCST_IF32NEQ, addr, val, 0, flg);
}

INLINE void G_If32GreaterThan(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_If__(GCST_IF32GTR, addr, val, 0, flg);
}

INLINE void G_If32LessThan(uint32_t addr, uint32_t val, GCodeFlags flg) {
    __G_If__(GCST_IF32LSS, addr, val, 0, flg);
}

INLINE void G_If16Equal(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_If__(GCST_IF16EQU, addr, val, 0, flg);
}

INLINE void G_If16EqualMask(uint32_t addr, uint16_t val, uint16_t mask, GCodeFlags flg) {
    __G_If__(GCST_IF16EQU, addr, val, mask, flg);
}

INLINE void G_If16NotEqual(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_If__(GCST_IF16NEQ, addr, val, 0, flg);
}

INLINE void G_If16NotEqualMask(uint32_t addr, uint16_t val, uint16_t mask, GCodeFlags flg) {
    __G_If__(GCST_IF16NEQ, addr, val, mask, flg);
}

INLINE void G_If16GreaterThan(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_If__(GCST_IF16GTR, addr, val, 0, flg);
}

INLINE void G_If16GreaterThanMask(uint32_t addr, uint16_t val, uint16_t mask, GCodeFlags flg) {
    __G_If__(GCST_IF16GTR, addr, val, mask, flg);
}

INLINE void G_If16LessThan(uint32_t addr, uint16_t val, GCodeFlags flg) {
    __G_If__(GCST_IF16LSS, addr, val, 0, flg);
}

INLINE void G_If16LessThanMask(uint32_t addr, uint16_t val, uint16_t mask, GCodeFlags flg) {
    __G_If__(GCST_IF16LSS, addr, val, mask, flg);
}

/* ********************************************************************************************************************
 * CT2: Base Address (ba) or Pointer Address (po)
 ******************************************************************************************************************* */

void __G_BAOrPO__(GCodeSubType typ, uint32_t addr, GRegister gr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    if (typ == GCST_BAWRITE || typ == GCST_POWRITE)
        oFlg &= ~GOF_ADDTO;
    if (gr == GR_NONE) {
        gr = GR_0;
        oFlg &= ~GOF_GECKOREG;
    }
    if ((flg & GCF_USEPOINTER) == GCF_USEPOINTER)
        oFlg |= GOF_PTRORBASEADDR;
    flg &= ~GCF_ADDRISSTACK;
    
    uint32_t gecko = GCT_BAORPO | typ | flg | oFlg | gr;
    fprintf(stdout, "%08X %08X\n", gecko, addr);
}

void __G_SetBAOrPOToCodeAddress__(GCodeSubType typ, int16_t offs) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_BAORPO | typ | (offs & 0xFFFF);
    fprintf(stdout, "%08X %08X\n", gecko, 0);
}

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

INLINE void G_SetBAToCodeAddress(int16_t offs) {
    __G_SetBAOrPOToCodeAddress__(GCST_BASETCODE, offs);
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

INLINE void G_SetPOToCodeAddress(int16_t offs) {
    __G_SetBAOrPOToCodeAddress__(GCST_POSETCODE, offs);
}

/* ********************************************************************************************************************
 * CT3: Control Flow (Repeat, bN)
 ******************************************************************************************************************* */

void __G_ControlFlow__(GCodeSubType typ, uint32_t cnt, int16_t offs, GBlock block, GExecStat exec) {
    __G_CheckIfExec__();
    
    if (block == GB_NONE)
        block = GB_0;
    if (exec == GES_NONE)
        exec = GES_TRUE;
    
    if (offs == 0)
        offs = 1;
    else if (offs < 0)
        offs += 2;
    
    uint32_t gecko = GCT_CTRLFLW | typ | exec | cnt | ((offs - 1) & 0xFFFF);
    fprintf(stdout, "%08X %08X\n", gecko, block);
}

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

void __G_GR__(GCodeSubType typ, GRegister gr, GDataType dtype, uint32_t addrOrVal, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    if (gr == GR_NONE)
        gr = GR_0;
    if (typ != GCST_GRSET)
        oFlg &= ~GOF_ADDTO;
    if ((flg & GCF_USEPOINTER) == GCF_USEPOINTER)
        oFlg |= GOF_PTRORBASEADDR;
    oFlg &= ~GOF_GECKOREG;
    flg &= ~GCF_ADDRISSTACK;
    
    uint32_t gecko = GCT_GR | typ | flg | oFlg | dtype | gr;
    fprintf(stdout, "%08X %08X\n", gecko, addrOrVal);
}

INLINE void G_SetGR(GRegister gr, uint32_t val, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRSET, gr, GDT_8, val, oFlg, flg);
}

INLINE void G_Read8GR(GRegister gr, uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRREAD, gr, GDT_8, addr, oFlg, flg);
}

INLINE void G_Read16GR(GRegister gr, uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRREAD, gr, GDT_16, addr, oFlg, flg);
}

INLINE void G_Read32GR(GRegister gr, uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRREAD, gr, GDT_32, addr, oFlg, flg);
}

INLINE void G_Write8GR(GRegister gr, uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRWRITE, gr, GDT_8, addr, oFlg, flg);
}

INLINE void G_Write16GR(GRegister gr, uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRWRITE, gr, GDT_16, addr, oFlg, flg);
}

INLINE void G_Write32GR(GRegister gr, uint32_t addr, GOffsetFlags oFlg, GCodeFlags flg) {
    __G_GR__(GCST_GRWRITE, gr, GDT_32, addr, oFlg, flg);
}

void __G_GROperation__(GCodeSubType typ, GRegister grn, GRegister grk, GRegisterOp op, GRegisterOpType ref,
uint32_t addrOrVal) {
    __G_CheckIfExec__();
    
    if (grn == GR_NONE)
        grn = GR_0;
    if (grk == GR_NONE)
        grk = GR_0;
    
    uint32_t gecko = GCT_GR | typ | grn | op | ref;
    uint32_t geckoVal = addrOrVal;
    if (typ == GCST_GROP)
        geckoVal = (uint32_t) grk;
    fprintf(stdout, "%08X %08X\n", gecko, geckoVal);
}

INLINE void G_GRDirectAdd(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_ADD, ref, val);
}

INLINE void G_GRDirectMultiply(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_MULTIPLY, ref, val);
}

INLINE void G_GRDirectOR(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_OR, ref, val);
}

INLINE void G_GRDirectAND(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_AND, ref, val);
}

INLINE void G_GRDirectXOR(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_XOR, ref, val);
}

INLINE void G_GRDirectShiftLeft(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_SHIFTLEFT, ref, val);
}

INLINE void G_GRDirectShiftRight(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_SHIFTRIGHT, ref, val);
}

INLINE void G_GRDirectRotateLeft(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_ROTATELEFT, ref, val);
}

INLINE void G_GRDirectSignedShiftRight(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_SIGNEDSHIFTRIGHT, ref, val);
}

INLINE void G_GRDirectFloatAdd(GRegister gr, GRegisterOpType ref, uint32_t val) {
    __G_GROperation__(GCST_GRDIRECTOP, gr, GR_NONE, GRO_FLOATADD, ref, val);
}

INLINE void G_GRDirectFloatMultiply(GRegister gr, GRegisterOpType ref, uint32_t val) {
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

void __G_CopyMem__(GCodeSubType typ, GRegister grn, GRegister grk, uint32_t addr, uint32_t cnt, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    flg &= ~GCF_ADDRISSTACK;
    
    if (typ == GCST_MEMCPYFROMGR) {
        if (grn == GR_NONE)
            grn = GR_0;
        
        if (grk == GR_15)
            grk = GR_14;
        
        if (grk == GR_NONE || (flg & GCF_USEPOINTER) == GCF_USEPOINTER)
            grk = GR_15;
    } else {
        if (grk == GR_NONE)
            grk = GR_0;
        
        if (grn == GR_15)
            grn = GR_14;
        
        if (grn == GR_NONE || (flg & GCF_USEPOINTER) == GCF_USEPOINTER)
            grn = GR_15;
    }
    
    uint32_t gecko = GCT_GR | typ | flg | (cnt << 8) | (grn << 4) | grk;
    fprintf(stdout, "%08X %08X\n", gecko, addr);
}

// *grn -> mem
INLINE void G_CopyGRDerefToMem(GRegister gr, uint32_t addr, uint16_t cnt, GCodeFlags flg) {
    __G_CopyMem__(GCST_MEMCPYFROMGR, gr, GR_NONE, addr, cnt, flg);
}

// *grn -> mem + *grk
INLINE void G_CopyGRDerefToGRDerefPlusMem(GRegister grn, GRegister grk, uint32_t addr, uint16_t cnt, GCodeFlags flg) {
    __G_CopyMem__(GCST_MEMCPYFROMGR, grn, grk, addr, cnt, flg);
}

// mem -> *grk
INLINE void G_CopyPOToGRDeref(GRegister gr, uint32_t addr, uint16_t cnt, GCodeFlags flg) {
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
 * All forms of Special If are unsigned checks
 * For mask, it is applied at runtime as: *grn & ~mask ==/!=/>/< *grk & ~mask
 */

void __G_SpecIf__(GCodeSubType typ, GRegister grn, GRegister grk, uint32_t addr, uint32_t mask, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    if (grn != GR_NONE && grk != GR_NONE) {
        addr = 0;
        flg &= ~GCF_USEPOINTER;
        flg &= ~GCF_ADDRISSTACK;
    }
    
    if (grn == GR_15)
        grn = GR_14;
    if (grk == GR_15)
        grk = GR_14;
    
    if (grn == GR_NONE || (flg & GCF_USEPOINTER) == GCF_USEPOINTER)
        grn = GR_15;
    if (grk == GR_NONE || (flg & GCF_USEPOINTER) == GCF_USEPOINTER)
        grk = GR_15;
    
    uint32_t gecko = GCT_SPECIF | typ | flg | (addr + 1);
    uint32_t geckoVal = (grk << 28) | (grn << 24) | mask;
    fprintf(stdout, "%08X %08X\n", gecko, geckoVal);
}

INLINE void G_If16GREqualDirect(GRegister gr, uint32_t addr, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16EQU, gr, GR_NONE, addr, 0, flg);
}

INLINE void G_If16GREqual(GRegister grn, GRegister grk) {
    __G_SpecIf__(GCST_IFGR16EQU, grn, grk, 0, 0, GCF_NONE);
}

INLINE void G_If16GREqualDirectMask(GRegister gr, uint32_t addr, uint16_t mask, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16EQU, gr, GR_NONE, addr, mask, flg);
}

INLINE void G_If16GREqualMask(GRegister grn, GRegister grk, uint16_t mask) {
    __G_SpecIf__(GCST_IFGR16EQU, grn, grk, 0, mask, GCF_NONE);
}

INLINE void G_If16GRNotEqualDirect(GRegister gr, uint32_t addr, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16NEQ, gr, GR_NONE, addr, 0, flg);
}

INLINE void G_If16GRNotEqual(GRegister grn, GRegister grk) {
    __G_SpecIf__(GCST_IFGR16NEQ, grn, grk, 0, 0, GCF_NONE);
}

INLINE void G_If16GRNotEqualDirectMask(GRegister gr, uint32_t addr, uint16_t mask, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16NEQ, gr, GR_NONE, addr, mask, flg);
}

INLINE void G_If16GRNotEqualMask(GRegister grn, GRegister grk, uint16_t mask) {
    __G_SpecIf__(GCST_IFGR16NEQ, grn, grk, 0, mask, GCF_NONE);
}

INLINE void G_If16GRGreaterThanDirect(GRegister gr, uint32_t addr, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16GTR, gr, GR_NONE, addr, 0, flg);
}

INLINE void G_If16GRGreaterThan(GRegister grn, GRegister grk) {
    __G_SpecIf__(GCST_IFGR16GTR, grn, grk, 0, 0, GCF_NONE);
}

INLINE void G_If16GRGreaterThanDirectMask(GRegister gr, uint32_t addr, uint16_t mask, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16GTR, gr, GR_NONE, addr, mask, flg);
}

INLINE void G_If16GRGreaterThanMask(GRegister grn, GRegister grk, uint16_t mask) {
    __G_SpecIf__(GCST_IFGR16GTR, grn, grk, 0, mask, GCF_NONE);
}

INLINE void G_If16GRLessThanDirect(GRegister gr, uint32_t addr, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16LSS, gr, GR_NONE, addr, 0, flg);
}

INLINE void G_If16GRLessThan(GRegister grn, GRegister grk) {
    __G_SpecIf__(GCST_IFGR16LSS, grn, grk, 0, 0, GCF_NONE);
}

INLINE void G_If16GRLessThanDirectMask(GRegister gr, uint32_t addr, uint16_t mask, GCodeFlags flg) {
    __G_SpecIf__(GCST_IFGR16LSS, gr, GR_NONE, addr, mask, flg);
}

INLINE void G_If16GRLessThanMask(GRegister grn, GRegister grk, uint16_t mask) {
    __G_SpecIf__(GCST_IFGR16LSS, grn, grk, 0, mask, GCF_NONE);
}

// TODO: Implement GCST_IFCNTR16EQU

// TODO: Implement GCST_IFCNTR16NEQ

// TODO: Implement GCST_IFCNTR16GTR

// TODO: Implement GCST_IFCNTR16LSS

/* ********************************************************************************************************************
 * CT6: Misc
 ******************************************************************************************************************* */

// TODO: Implement GCST_ASMEXEC

// TODO: Implement GCST_ASMINST

// TODO: Implement GCST_ASMBRCH

void G_Switch() {
    __G_CheckIfExec__();
    
    fprintf(stdout, "%08X %08X\n", GCT_MISC | GCST_SWITCH, 0);
}

void __G_RangeCheck__(uint32_t startAddr, uint32_t endAddr, uint32_t endif, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    flg &= ~GCF_ADDRISSTACK;
    uint32_t gecko = GCT_MISC | GCST_RNGCHCK | flg | endif;
    uint32_t geckoVal = (startAddr << 16) | endAddr;
    fprintf(stdout, "%08X %08X\n", gecko, geckoVal);
}

INLINE void G_RangeCheck(uint16_t startAddr, uint16_t endAddr, GCodeFlags flg) {
    __G_RangeCheck__(startAddr, endAddr, 0, flg);
}

INLINE void G_RangeCheckEndif(uint16_t startAddr, uint16_t endAddr, GCodeFlags flg) {
    __G_RangeCheck__(startAddr, endAddr, 1, flg);
}

/* ********************************************************************************************************************
 * CT7: End
 ******************************************************************************************************************* */

void __G_End__(GCodeSubType typ, uint32_t ba, uint32_t po, uint32_t endifCount, uint32_t doelse) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_END | typ | (doelse << 20) | endifCount;
    uint32_t geckoVal = (ba << 16) | po;
    fprintf(stdout, "%08X %08X\n", gecko, geckoVal);
}

INLINE void G_FullTerminator() {
    __G_End__(GCST_FULLTERM, 0, 0, 0, 0);
}

INLINE void G_FullTerminatorBAPO(uint16_t ba, uint16_t po) {
    __G_End__(GCST_FULLTERM, ba, po, 0, 0);
}

INLINE void G_Endif() {
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

INLINE void G_Endif_Else() {
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
// Use G_FullTerminator instead (it endifs all ifs and clears execution status).
void G_EndOfCodeList() {
    __G_CheckIfExec__();
    
    fprintf(stdout, "%08X %08X\n", GCT_END | GCST_ENDOFCODE, 0);
}

/* ********************************************************************************************************************
 * Special Extensions
 ******************************************************************************************************************* */
INLINE void G_If8Equal(uint32_t addr, uint8_t val, GCodeFlags flg) {
    G_If16EqualMask(addr, val, 0xFF, flg);
}

INLINE void G_If8NotEqual(uint32_t addr, uint32_t val, GCodeFlags flg) {
    G_If16NotEqualMask(addr, val, 0xFF, flg);
}

INLINE void G_If8GreaterThan(uint32_t addr, uint32_t val, GCodeFlags flg) {
    G_If16GreaterThanMask(addr, val, 0xFF, flg);
}

INLINE void G_If8LessThan(uint32_t addr, uint32_t val, GCodeFlags flg) {
    G_If16LessThanMask(addr, val, 0xFF, flg);
}
#endif
