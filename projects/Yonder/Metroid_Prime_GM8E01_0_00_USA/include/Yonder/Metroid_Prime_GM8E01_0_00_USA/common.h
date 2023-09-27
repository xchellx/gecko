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

#ifndef __YONDER_METROID_PRIME_GM8E01_0_00_USA_COMMON_H__
#define __YONDER_METROID_PRIME_GM8E01_0_00_USA_COMMON_H__
#define ADDR_GameID 0x00000000
#define VAL_GameID_GM8E 0x474D3845

#define ADDR_GameRevision 0x00000007
#define VAL_GameRevision 0x0

#define CStateManager_xb54_finalInput_x2c 0x0045AD28
#define CFinalInput_x2c_b29_L 0x4
#define CFinalInput_x2c_b30_R 0x2
#define CFinalInput_x2c_b31_DPUp 0x1

#define CPlayer_x490_gun 0x0046BE0C

#define CPlayerGun_x33c_phazonBeamState 0x0000033C
#define EPhazonBeamState_Active 0x3
#define EPhazonBeamState_Inactive 0x0

#define CPlayerGun_x835 0x00000835
#define CPlayerGun_x835_25_inPhazonBeam 0x40
#define CPlayerGun_x835_24_canFirePhazon 0x80
#endif
