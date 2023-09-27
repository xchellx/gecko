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

void toggle_phazon_beam_part_1_GM8E01_0_00_USA(void) {
    G_DeclareLabel(L_ELSE);
    G_DeclareLabel(L_END);
    G_BeginCode();
    
    G_If32Equal(ADDR_GameID, VAL_GameID_GM8E, GCF_NONE);
    G_GotoIfFalse(G_GetLabel(L_END));
    
    G_Endif_If8Equal(ADDR_GameRevision, VAL_GameRevision, GCF_NONE);
    G_GotoIfFalse(G_GetLabel(L_END));
    
    G_ReadGR8(GR_7, CStateManager_xb54_finalInput_x2c, GOF_PTRORBASEADDR, GCF_NONE);
    
    G_Endif_If32Equal(G_ADDR_GR6, 0, GCF_NONE);
    G_GotoIfFalse(G_GetLabel(L_ELSE));
    G_SetGR(GR_6, 0xFFFD, GOF_NONE, GCF_NONE);
    
    G_DefineLabel(L_ELSE);
    G_Endif_If32LessThan(G_ADDR_GR6, 0xFFFF, GCF_NONE);
    G_GRAddDirect(GR_6, GROT_SRCVALUE_DSTVALUE, 1);
    
    G_DefineLabel(L_END);
    G_FullTerminator();
    G_EndCode();
}
