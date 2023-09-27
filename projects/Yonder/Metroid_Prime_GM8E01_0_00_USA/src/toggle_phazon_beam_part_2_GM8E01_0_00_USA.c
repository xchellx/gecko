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

#include <__gen__/standard_defs.h>

#include <Yonder/Metroid_Prime_GM8E01_0_00_USA/common.h>

#define BUTTON_MASK (CFinalInput_x2c_b29_L | CFinalInput_x2c_b30_R | CFinalInput_x2c_b31_DPUp)

void toggle_phazon_beam_part_2_GM8E01_0_00_USA(void) {
    G_DeclareLabel(L_ELSE);
    G_DeclareLabel(L_END);
    G_BeginCode();
    
    G_If32Equal(ADDR_GameID, VAL_GameID_GM8E, GCF_NONE);
    G_GotoIfFalse(G_GetLabel(L_END));
    
    G_Endif_If8Equal(ADDR_GameRevision, VAL_GameRevision, GCF_NONE);
    G_GotoIfFalse(G_GetLabel(L_END));
    
    G_Endif_If32GreaterThan(G_ADDR_GR6, 0xFFFE, GCF_NONE);
    G_GotoIfFalse(G_GetLabel(L_END));
    
    G_ReadGR8(GR_6, CStateManager_xb54_finalInput_x2c, GOF_PTRORBASEADDR, GCF_NONE);
    G_GRXOR(GR_6, GR_7, GROT_SRCVALUE_DSTVALUE);
    G_GRAND(GR_6, GR_7, GROT_SRCVALUE_DSTVALUE);
    G_GRANDDirect(GR_6, GROT_SRCVALUE_DSTVALUE, BUTTON_MASK);
    G_Endif_If32Equal(G_ADDR_GR6, BUTTON_MASK, GCF_NONE);
    G_GotoIfFalse(G_GetLabel(L_END));
    
    G_ReadPO(CPlayer_x490_gun, GOF_PTRORBASEADDR, GCF_NONE);
    G_RangeCheck(G_ADDR_RNG_BA, G_ADDR_RNG_STACK, GCF_USEPOINTER);
    G_GotoIfFalse(G_GetLabel(L_END));
    
    G_Endif_If32Equal(CPlayerGun_x33c_phazonBeamState, EPhazonBeamState_Active, GCF_USEPOINTER);
    G_GotoIfFalse(G_GetLabel(L_ELSE));
    G_ReadGR8(GR_6, CPlayerGun_x835, GOF_PTRORBASEADDR, GCF_USEPOINTER);
    G_GRANDDirect(GR_6, GROT_SRCVALUE_DSTVALUE, (~CPlayerGun_x835_24_canFirePhazon) & 0xFF);
    G_WriteGR8(GR_6, CPlayerGun_x835, GOF_PTRORBASEADDR, GCF_USEPOINTER);
    G_GotoIfTrue(G_GetLabel(L_END));
    
    G_DefineLabel(L_ELSE);
    G_Endif_If32Equal(CPlayerGun_x33c_phazonBeamState, EPhazonBeamState_Inactive, GCF_USEPOINTER);
    G_GotoIfFalse(G_GetLabel(L_END));
    G_ReadGR8(GR_6, CPlayerGun_x835, GOF_PTRORBASEADDR, GCF_USEPOINTER);
    G_GRORDirect(GR_6, GROT_SRCVALUE_DSTVALUE, CPlayerGun_x835_25_inPhazonBeam | CPlayerGun_x835_24_canFirePhazon);
    G_WriteGR8(GR_6, CPlayerGun_x835, GOF_PTRORBASEADDR, GCF_USEPOINTER);
    
    G_DefineLabel(L_END);
    G_FullTerminator();
    
    G_EndCode();
}
