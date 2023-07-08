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

#include <gecko.h>

#define CStateManager_x_finalInput_Minus 0x004C007A
#define CStateManager_x_finalInput_Plus 0x004C0089
#define CStateManager_x_finalInput_DPUp 0x004C008C

#define CPlayer_x480_gun 0x004D40A0

#define CPlayerGun_x33c_phazonBeamState 0x0000033C
#define EPhazonBeamState_Active 0x3
#define EPhazonBeamState_Inactive 0x0

#define CPlayerGun_x84B 0x0000084B
#define CPlayerGun_x84B_26_inPhazonBeam 0x20
#define CPlayerGun_x84B_25_canFirePhazon 0x40

INLINE void printCode() {
    G_DeclareLabel(L_END);
    G_BeginCode();
    G_SetGR(GR_6, 0, GOF_NONE, GCF_NONE);
    G_Read8GR(GR_7, CStateManager_x_finalInput_Minus, GOF_PTRORBASEADDR, GCF_NONE);
    G_GRAdd(GR_6, GR_7, GROT_SRCVALUE_DSTVALUE);
    G_Read8GR(GR_7, CStateManager_x_finalInput_Plus, GOF_PTRORBASEADDR, GCF_NONE);
    G_GRAdd(GR_6, GR_7, GROT_SRCVALUE_DSTVALUE);
    G_Read8GR(GR_7, CStateManager_x_finalInput_DPUp, GOF_PTRORBASEADDR, GCF_NONE);
    G_GRAdd(GR_6, GR_7, GROT_SRCVALUE_DSTVALUE);
    G_If32Equal(G_ADDR_GR6, 3, GCF_NONE);
    G_GotoIfFalse(G_GetLabel(L_END));
    G_ReadPO(CPlayer_x480_gun, GOF_PTRORBASEADDR, GCF_NONE);
    G_RangeCheck(G_ADDR_RNG_BA, G_ADDR_RNG_STACK, GCF_USEPOINTER);
    G_GotoIfFalse(G_GetLabel(L_END));
    G_If32Equal(CPlayerGun_x33c_phazonBeamState, EPhazonBeamState_Active, GCF_USEPOINTER);
    G_Read8GR(GR_6, CPlayerGun_x84B, GOF_PTRORBASEADDR, GCF_USEPOINTER);
    G_GRDirectAND(GR_6, GROT_SRCVALUE_DSTVALUE, (~CPlayerGun_x84B_25_canFirePhazon) & 0xFF);
    G_Write8GR(GR_6, CPlayerGun_x84B, GOF_PTRORBASEADDR, GCF_USEPOINTER);
    G_GotoIfTrue(G_GetLabel(L_END));
    G_If32Equal(CPlayerGun_x33c_phazonBeamState, EPhazonBeamState_Inactive, GCF_USEPOINTER);
    G_Read8GR(GR_6, CPlayerGun_x84B, GOF_PTRORBASEADDR, GCF_USEPOINTER);
    G_GRDirectOR(GR_6, GROT_SRCVALUE_DSTVALUE, CPlayerGun_x84B_26_inPhazonBeam | CPlayerGun_x84B_25_canFirePhazon);
    G_Write8GR(GR_6, CPlayerGun_x84B, GOF_PTRORBASEADDR, GCF_USEPOINTER);
    G_DefineLabel(L_END);
    G_FullTerminator();
    G_EndCode();
}

#define CODE_NAME "Toggle Phazon Beam (R3ME01 0-00) (see notes)"
#define CODE_AUTHOR "Yonder"
#define CODE_DEFINITION printCode
static char *codeDescription = (
"*Toggle with Minus + Plus + DPad UP ALL at the SAME TIME."
"\n*Holding down the button combo and/or incrementally pressing the button combo is buggy."
"\n*NOTE: This uses gr6 and gr7."
);
#define CODE_DESCRIPTION codeDescription

#include <standard.h>
