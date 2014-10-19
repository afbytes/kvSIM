/*
 * kvSIM patch for Siemens 6688i(SL45i) v55lg8
 *
 * Copyright (C) 2004-2007 Konca Fung.
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

/* Author: Konca Fung <konca@tom.com>. http://www.konca.com */


#include "SlckOsFunc.h"

// must use the 'romdata' to ensure the data is saved in rom
#pragma romdata

// ÖØÆôº¯Êý
#define SendMessage 0xB4724C
void SendRebootMsg(void)
{
    // Call "SendMessage" at 0xB4724C for SLCK v55lg8
#pragma asm
        mov     r12,    #34A0h
        mov     r13,    #0Eh
        mov     r14,    #5
        mov     r15,    #5
        calls   0B4h,   724Ch
#pragma endasm	
}

P_STRLEN            *const far STRLEN         = (P_STRLEN*) 0xC78536;
P_MEMCPY            *const far LIB_Memcpy     = (P_MEMCPY*) 0xC783B4;
P_SIM_ACCESS        *const far SIM_Access     = (P_SIM_ACCESS*) 0xB2E410;
P_COM_SEND_STRING   *const far COM_SendString = (P_COM_SEND_STRING*) 0xCDB04C;

P_MMI_Free              *const MMI_Free              = (P_MMI_Free              *)0xD389C8;
P_MMI_Malloc            *const MMI_Malloc            = (P_MMI_Malloc            *)0xD3898C;
P_MMI_AllocObject       *const MMI_AllocObject       = (P_MMI_AllocObject       *)0xE5B24C;
P_MMI_ObjectInit        *const MMI_ObjectInit        = (P_MMI_ObjectInit        *)0xE5E492;
P_InitWindowClass       *const InitWindowClass       = (P_InitWindowClass       *)0xE5E20A;
P_SetWindowItemCount    *const SetWindowItemCount    = (P_SetWindowItemCount    *)0xE5E4A8;
P_SetSelMenu            *const SetSelMenu            = (P_SetSelMenu            *)0xE5E54E;
P_InitFrameClass        *const InitFrameClass        = (P_InitFrameClass        *)0xD63EF8;
P_CreateWnd_FullScreen  *const CreateWnd_FullScreen  = (P_CreateWnd_FullScreen  *)0xF3EA82;

P_GetFahterWindowHandle *const GetFahterWindowHandle = (P_GetFahterWindowHandle *)0xE5EEA0;
P_CreateWString         *const CreateWString         = (P_CreateWString     *)0xF60000;
P_SetItemSoftKey        *const SetItemSoftKey        = (P_SetItemSoftKey        *)0xE5E516;
P_SetItemImage          *const SetItemImage          = (P_SetItemImage          *)0xE5EC32;
P_GetSelectMenuIdx      *const GetSelectMenuIdx      = (P_GetSelectMenuIdx      *)0xE5EE40;
P_CreateMenuText        *const CreateMenuText        = (P_CreateMenuText        *)0xE5F0E2;
P_SetMenuTextByHandle   *const SetMenuTextByHandle   = (P_SetMenuTextByHandle   *)0xF60132;
P_RefreshMenuItem       *const RefreshMenuItem       = (P_RefreshMenuItem       *)0xE5D860;
P_UTF8ToWString         *const UTF8ToWString         = (P_UTF8ToWString         *)0xE4F6FC;
P_StoreASCStringAsID    *const StoreASCStringAsID    = (P_StoreASCStringAsID    *)0xE4A7CE;
P_ClearStringID         *const ClearStringID         = (P_ClearStringID         *)0xE4A990;

