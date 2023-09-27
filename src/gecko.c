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

#include <gecko.h>

#ifndef SWAP32
#define SWAP32(x) __builtin_bswap32((x))
#endif

// To avoid going into floating point math:
// Equivalent to: ceil(n / (2^s)) * (2^s)
#define __RoundUpToNearestS__(n, s) ((((((n) - 1) >> (s)) + 1) >> 0) << (s))
// Equivalent to: ceil(n / 8) * 8
#define __RoundUpToNearest8__(n) __RoundUpToNearestS__((n), 3)
// Equivalent to: ceil(n / 2) * 2
#define __RoundUpToNearest2__(n) __RoundUpToNearestS__((n), 1)

/* ********************************************************************************************************************
 * Code Output Functionality
 ******************************************************************************************************************* */

FILE *G_OutputHandle = NULL;
uint8_t G_IsOutputBin = 0;

INLINE void __G_Print8__(uint8_t val) {
    if (G_IsOutputBin)
        fwrite(&val, sizeof(uint8_t), 1, G_OutputHandle);
    else
        fprintf(G_OutputHandle, "%02X", val);
}

INLINE void __G_Print32__(uint32_t val) {
    if (G_IsOutputBin) {
        val =SWAP32(val);
        fwrite(&val, sizeof(uint32_t), 1, G_OutputHandle);
    } else
        fprintf(G_OutputHandle, "%08X", val);
}

INLINE void __G_PrintCodeType__(uint32_t gecko, uint32_t geckoVal) {
    if (G_IsOutputBin) {
        gecko =SWAP32(gecko);
        geckoVal =SWAP32(geckoVal);
        fwrite(&gecko, sizeof(uint32_t), 1, G_OutputHandle);
        fwrite(&geckoVal, sizeof(uint32_t), 1, G_OutputHandle);
    } else
        fprintf(G_OutputHandle, "%08X %08X\n", gecko, geckoVal);
}

/* ********************************************************************************************************************
 * Label and Line Pointer Functionality
 ******************************************************************************************************************* */

int16_t G_CurProcLblLine = 0;
uint8_t G_IsProcLabels = 0;

#ifdef __GECKO_H_CODEHANDLERSCOMPAT__
uint32_t G_CurProcPtrLine = 0;
uint8_t G_IsProcLinePtrs = 0;
#endif

#ifdef __GECKO_H_CODEHANDLERSCOMPAT__
#define __G_CheckIfExec__() \
G_CurProcLblLine += 1; \
G_CurProcPtrLine += 1; \
if (G_IsProcLabels || G_IsProcLinePtrs || !G_OutputHandle) \
    return
#else
#define __G_CheckIfExec__() \
G_CurProcLblLine += 1; \
if (G_IsProcLabels || !G_OutputHandle) \
    return
#endif

int16_t G_GetLabel(int16_t label) {
    if (G_IsProcLabels)
        return 0;
    
    int16_t offs = (int16_t) abs(label - G_CurProcLblLine);
    if (G_CurProcLblLine > label)
        offs = -offs;
    return offs;
}

#ifdef __GECKO_H_CODEHANDLERSCOMPAT__
uint32_t G_GetLinePointer(void) {
    if (G_IsProcLinePtrs)
        return 0;
    
    return (
          ((uint32_t) G_ADDR_CODEHANDLER)
        + ((uint32_t) G_SIZE_CODEHANDLER)
        + ((uint32_t) (G_CurProcPtrLine * ((uint32_t) (sizeof(uint32_t) * 2))))
    );
}
#endif

/* ********************************************************************************************************************
 * CT0: Write
 ******************************************************************************************************************* */

void __G_Write__(GCodeSubType typ, uint32_t addr, uint32_t val, uint32_t extraCount, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_WRITE | typ | flg | (uint32_t) addr;
    uint32_t geckoVal = (extraCount << 16) | val;
    __G_PrintCodeType__(gecko, geckoVal);
}

void G_WriteString(uint32_t addr, uint16_t valsSz, uint8_t *vals, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_WRITE | GCST_WRITESTR | flg | addr;
    uint32_t valsSz32 = (uint32_t) valsSz;
    __G_PrintCodeType__(gecko, valsSz32);
    
    uint32_t valEvenSz = __RoundUpToNearest8__(valsSz32);
    for (uint32_t i = 0; i < valEvenSz; i++) {
        uint8_t val = 0;
        if (i < valsSz32)
            val = vals[i];
        __G_Print8__(val);
        
        if (!G_IsOutputBin) {
            uint32_t idx = i + 1;
            if (idx < valEvenSz) {
                if (idx != 1 && (idx % 8) == 0)
                    fprintf(G_OutputHandle, "\n");
                else if ((idx % 4) == 0)
                    fprintf(G_OutputHandle, " ");
            } else
                fprintf(G_OutputHandle, "\n");
        }
    }
}

void __G_WriteSerial__(GSerialDataType sdType, uint32_t addr, uint32_t value, uint32_t count, uint32_t addrIncr,
uint32_t valueIncr, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    uint32_t gecko1 = GCT_WRITE | GCST_WRITESRL | flg | addr;
    __G_PrintCodeType__(gecko1, value);
    
    uint32_t gecko2 = addrIncr | sdType | ((((count || (uint32_t) 1) - 1) & 0x00000FFF) << 16);
    __G_PrintCodeType__(gecko2, valueIncr);
}

/* ********************************************************************************************************************
 * CT1: Regular If
 ******************************************************************************************************************* */

void __G_If__(GCodeSubType typ, uint32_t addr, uint32_t val, uint32_t endif, uint32_t mask, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_REGIF | typ | flg | (uint32_t) (addr + endif);
    uint32_t geckoVal = (mask << 16) | val;
    __G_PrintCodeType__(gecko, geckoVal);
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
    __G_PrintCodeType__(gecko, addr);
}

void __G_SetBAOrPOToCodeAddress__(GCodeSubType typ, int16_t offs) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_BAORPO | typ | ((int32_t) (offs & 0xFFFF));
    __G_PrintCodeType__(gecko, 0);
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
    
    uint32_t gecko = GCT_CTRLFLW | typ | exec | cnt | (((int32_t) (offs - 1)) & 0xFFFF);
    __G_PrintCodeType__(gecko, block);
}

/* ********************************************************************************************************************
 * CT4: Gecko Register (grN)
 ******************************************************************************************************************* */

void __G_GR__(GCodeSubType typ, GRegister gr, GRegisterDataType rdType, uint32_t addrOrVal, GOffsetFlags oFlg,
GCodeFlags flg) {
    __G_CheckIfExec__();
    
    if (gr == GR_NONE)
        gr = GR_0;
    if (typ != GCST_GRSET)
        oFlg &= ~GOF_ADDTO;
    if ((flg & GCF_USEPOINTER) == GCF_USEPOINTER)
        oFlg |= GOF_PTRORBASEADDR;
    oFlg &= ~GOF_GECKOREG;
    flg &= ~GCF_ADDRISSTACK;
    
    uint32_t gecko = GCT_GR | typ | flg | oFlg | rdType | gr;
    uint32_t geckoVal = addrOrVal;
    __G_PrintCodeType__(gecko, geckoVal);
}

void __G_GROperation__(GCodeSubType typ, GRegister grn, GRegister grk, GRegisterOp op, GRegisterOpType ref,
uint32_t val) {
    __G_CheckIfExec__();
    
    if (grn == GR_NONE)
        grn = GR_0;
    if (grk == GR_NONE)
        grk = GR_0;
    
    uint32_t gecko = GCT_GR | typ | grn | op | ref;
    uint32_t geckoVal = val;
    if (typ == GCST_GROP)
        geckoVal = (uint32_t) grk;
    __G_PrintCodeType__(gecko, geckoVal);
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
    __G_PrintCodeType__(gecko, addr);
}

/* ********************************************************************************************************************
 * CT5: Special If
 ******************************************************************************************************************* */

void __G_SpecIf__(GCodeSubType typ, GRegister grn, GRegister grk, uint32_t addr, uint32_t endif, uint32_t mask,
GCodeFlags flg) {
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
    
    uint32_t gecko = GCT_SPECIF | typ | flg | (uint32_t) (addr + endif);
    uint32_t geckoVal = (grk << 28) | (grn << 24) | mask;
    __G_PrintCodeType__(gecko, geckoVal);
}

void __G_IfCounter__(GCodeSubType typ, uint32_t counter, uint32_t max, uint32_t mask, GIfCounterFlags cFlg) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_SPECIF | typ | cFlg | (counter << 4);
    uint32_t geckoVal = (mask << 16) | max;
    __G_PrintCodeType__(gecko, geckoVal);
}

/* ********************************************************************************************************************
 * CT6: Misc
 ******************************************************************************************************************* */

void G_ExecuteAssembly(uint32_t valsSz, uint32_t *vals) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_MISC | GCST_ASMEXEC;
    uint32_t valEvenSz = __RoundUpToNearest2__(valsSz);
    __G_PrintCodeType__(gecko, valEvenSz / 2);
    
    for (uint32_t i = 0; i < valEvenSz; i++) {
        uint32_t val = 0;
        if (i < valsSz)
            val = vals[i];
        __G_Print32__(val);
        
        if (!G_IsOutputBin) {
            uint32_t idx = i + 1;
            if (idx < valEvenSz) {
                if (idx != 1 && (idx % 2) == 0)
                    fprintf(G_OutputHandle, "\n");
                else if ((idx % 1) == 0)
                    fprintf(G_OutputHandle, " ");
            } else
                fprintf(G_OutputHandle, "\n");
        }
    }
}

void G_InsertAssembly(uint32_t addr, uint32_t valsSz, uint32_t *vals, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_MISC | GCST_ASMINST | flg | addr;
    uint32_t valEvenSz = __RoundUpToNearest2__(valsSz);
    if (valsSz == valEvenSz)
        valEvenSz += 2;
    __G_PrintCodeType__(gecko, valEvenSz / 2);
    
    for (uint32_t i = 0; i < valEvenSz; i++) {
        uint32_t val = 0;
        if (i < valsSz)
            val = vals[i];
        else if (((i + 1) % 2) != 0)
            val = 0x60000000; // nop
        __G_Print32__(val);
        
        if (!G_IsOutputBin) {
            uint32_t idx = i + 1;
            if (idx < valEvenSz) {
                if (idx != 1 && (idx % 2) == 0)
                    fprintf(G_OutputHandle, "\n");
                else if ((idx % 1) == 0)
                    fprintf(G_OutputHandle, " ");
            } else
                fprintf(G_OutputHandle, "\n");
        }
    }
}

void G_CreateBranch(uint32_t addr, uint32_t branch, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_MISC | GCST_ASMBRCH | flg | addr;
    __G_PrintCodeType__(gecko, branch);
}

void G_Switch(void) {
    __G_CheckIfExec__();
    
    __G_PrintCodeType__(GCT_MISC | GCST_SWITCH, 0);
}

void __G_RangeCheck__(uint32_t startAddr, uint32_t endAddr, uint32_t endif, GCodeFlags flg) {
    __G_CheckIfExec__();
    
    flg &= ~GCF_ADDRISSTACK;
    uint32_t gecko = GCT_MISC | GCST_RNGCHCK | flg | endif;
    uint32_t geckoVal = (startAddr << 16) | endAddr;
    __G_PrintCodeType__(gecko, geckoVal);
}

/* ********************************************************************************************************************
 * CT7: End
 ******************************************************************************************************************* */

void __G_End__(GCodeSubType typ, uint32_t ba, uint32_t po, uint32_t endifCount, uint32_t doelse) {
    __G_CheckIfExec__();
    
    uint32_t gecko = GCT_END | typ | (doelse << 20) | endifCount;
    uint32_t geckoVal = (ba << 16) | po;
    __G_PrintCodeType__(gecko, geckoVal);
}

void G_EndGCT(void) {
    __G_CheckIfExec__();
    
    __G_PrintCodeType__(GCT_END | GCST_ENDOFCODE, 0);
}

/* ********************************************************************************************************************
 * Special Extensions
 ******************************************************************************************************************* */

void G_BeginGCT(void) {
    __G_CheckIfExec__();
    
    __G_PrintCodeType__(GCT_MAGIC, GCT_MAGIC);
}
