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

#ifndef __SLCK_V55_VAR_H__
#define __SLCK_V55_VAR_H__



// You can't use a global value event you add 'const' before it
//   because it will be saved at a unknown place.
// So, I use MACRO here. You can event set its value: SIM_LAST_BUF_POINTER = NULL
#define SIM_LAST_INS (*((int*)0xc8ae6))   // word
#define SIM_LAST_CLA (*((int*)0xc8ae8))   // word
#define SIM_LAST_P1  (*((int*)0xc8aea))   // word
#define SIM_LAST_P2  (*((int*)0xc8aec))   // word
#define SIM_LAST_P3  (*((int*)0xc8aee))   // word
#define SIM_LAST_BUF_POINTER (*((unsigned char **)(0xc8af0))) // pointer
#define SIM_LAST_RW_FLAG     (*((int*)0xc8af4))                // word
#define SIM_LAST_SW1         (*((unsigned char*)0xc8af6))      // byte 
#define SIM_LAST_SW2         (*((unsigned char*)0xc8af7))      // byte

#define IS_SET_PROFILE_FIND_NETWORK ((*(unsigned char*)(0x364a5)) & 0x04)
#define LEVEL_OF_GAME_THE_MAZE      (*((unsigned char*)(0xD8D94)))

// type of "void foo(void)"
typedef void P_VOID_VOID(void);

// This is a pointer of "Data" (Something with 'and 3FFFF')
typedef struct _COMMON_DATA_STRU
{
    unsigned int nWhat; // Must leave a field here, or it will be error
}COMMON_DATA_STRU;

/* Windows Class*/
typedef struct _WINDOW_CLASS
{
    void                *pUnknown1;
    P_VOID_VOID         *pFuncForKeyPress;
    void                *pUnknown3;
    void                *pUnknown4;
    COMMON_DATA_STRU    *pSoftKeyInfo;
    COMMON_DATA_STRU    *pSoftKeyConf;
    int                 nWindowType;
    int                 nUnknown;
    P_VOID_VOID         *pFuncForItemRefresh;
    COMMON_DATA_STRU    *pItemConfArray;
    COMMON_DATA_STRU    *pFuncForPerItemArray;
    int   nItemCount;
}WINDOW_CLASS;

/* Frame Class*/
typedef struct _FRAME_CLASS
{
    unsigned short      usUnknown1;
    unsigned short      usUnknown2;
    unsigned short      usWidth;
    unsigned short      usItemHeight;
    COMMON_DATA_STRU    *pIconConfig;
    unsigned short      usTitleStringId;
    unsigned short      usType;
}FRAME_CLASS;

typedef struct _WSTR_BUF
{
    int nLen;
    unsigned short Buffer[1]; // 定义一个以作变长使用
}WSTR_BUF;

typedef struct _WSTRING_STRU
{
    WSTR_BUF *pBuffer;
    int nCount;
}WSTRING_STRU;

#endif
