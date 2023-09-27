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

#define ADDR_GameID 0x00000000
#define VAL_GameID_GM8P 0x474D3850

#define CStateManager_xb52_finalInput_x2c 0x003E2C08
#define CFinalInput_x2c_b29_L 0x4
#define CFinalInput_x2c_b30_R 0x2
#define CFinalInput_x2c_b31_DPUp 0x1

#define CPlayer_x490_gun 0x003F3D44

#define CPlayerGun_x33c_phazonBeamState 0x0000033C
#define EPhazonBeamState_Active 0x3
#define EPhazonBeamState_Inactive 0x0

#define CPlayerGun_x835 0x00000835
#define CPlayerGun_x835_25_inPhazonBeam 0x40
#define CPlayerGun_x835_24_canFirePhazon 0x80

void toggle_phazon_beam_GM8P01_0_00_PAL(void) {
    G_DeclareLabel(L_END);
    G_BeginCode();
    G_If32Equal(ADDR_GameID, VAL_GameID_GM8P, GCF_NONE);
    G_GotoIfFalse(G_GetLabel(L_END));
    G_Read8GR(GR_6, CStateManager_xb52_finalInput_x2c, GOF_PTRORBASEADDR, GCF_NONE);
    G_GRDirectAND(GR_6, GROT_SRCVALUE_DSTVALUE, CFinalInput_x2c_b29_L | CFinalInput_x2c_b30_R | CFinalInput_x2c_b31_DPUp);
    G_If32Equal(G_ADDR_GR6, CFinalInput_x2c_b29_L | CFinalInput_x2c_b30_R | CFinalInput_x2c_b31_DPUp, GCF_NONE);
    G_GotoIfFalse(G_GetLabel(L_END));
    G_ReadPO(CPlayer_x490_gun, GOF_PTRORBASEADDR, GCF_NONE);
    G_RangeCheck(G_ADDR_RNG_BA, G_ADDR_RNG_STACK, GCF_USEPOINTER);
    G_GotoIfFalse(G_GetLabel(L_END));
    G_If32Equal(CPlayerGun_x33c_phazonBeamState, EPhazonBeamState_Active, GCF_USEPOINTER);
    G_Read8GR(GR_6, CPlayerGun_x835, GOF_PTRORBASEADDR, GCF_USEPOINTER);
    G_GRDirectAND(GR_6, GROT_SRCVALUE_DSTVALUE, (~CPlayerGun_x835_24_canFirePhazon) & 0xFF);
    G_Write8GR(GR_6, CPlayerGun_x835, GOF_PTRORBASEADDR, GCF_USEPOINTER);
    G_GotoIfTrue(G_GetLabel(L_END));
    G_If32Equal(CPlayerGun_x33c_phazonBeamState, EPhazonBeamState_Inactive, GCF_USEPOINTER);
    G_Read8GR(GR_6, CPlayerGun_x835, GOF_PTRORBASEADDR, GCF_USEPOINTER);
    G_GRDirectOR(GR_6, GROT_SRCVALUE_DSTVALUE, CPlayerGun_x835_25_inPhazonBeam | CPlayerGun_x835_24_canFirePhazon);
    G_Write8GR(GR_6, CPlayerGun_x835, GOF_PTRORBASEADDR, GCF_USEPOINTER);
    G_DefineLabel(L_END);
    G_FullTerminator();
    G_EndCode();
}
