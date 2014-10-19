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

#ifndef __SLCK_V55_FUNC_H__
#define __SLCK_V55_FUNC_H__

#include "SlckOsVar.h"

void SendRebootMsg(void);

/* ----------  ---------- */
typedef int  huge P_STRLEN(void *pString);
typedef void huge P_MEMCPY(void far *pDst, void far *pSrc, int nLen);
typedef void huge P_COM_SEND_STRING(const char *p);
typedef void huge P_SIM_ACCESS(int what, int cla, int ins,
                               int p1, int p2, int op,
                               int SendLen, unsigned char *SendBuf,
                               int RecvLen, unsigned char *RecvBuf);

/* ---------- The Definition is in File 'SlckOsFunc.c ---------- */
extern P_MEMCPY            *const far LIB_Memcpy;
extern P_STRLEN            *const far STRLEN;
extern P_SIM_ACCESS        *const far SIM_Access;
extern P_COM_SEND_STRING   *const far COM_SendString;

// ------------------------------------------------------------------------------

typedef void  huge P_MMI_Free(COMMON_DATA_STRU *pObject);
typedef COMMON_DATA_STRU* huge P_MMI_Malloc(int nSize);
typedef COMMON_DATA_STRU* huge P_MMI_AllocObject(P_VOID_VOID *pMallocFuncAddr, P_VOID_VOID *pFreeFuncAddr);
typedef void  huge P_MMI_ObjectInit(COMMON_DATA_STRU *pObject, COMMON_DATA_STRU *pObject); /* Cound Not Call */
typedef void  huge P_InitWindowClass(COMMON_DATA_STRU *pObject, COMMON_DATA_STRU *pClassStru);
typedef void  huge P_SetWindowItemCount(COMMON_DATA_STRU *pObject, long lItemCount);
typedef void  huge P_SetSelMenu(COMMON_DATA_STRU *pObject, long lItemIdx);
typedef void  huge P_InitFrameClass(COMMON_DATA_STRU *pObject, COMMON_DATA_STRU *pFrameStru, P_VOID_VOID *pMMI_AllocFunc);
typedef void  huge P_CreateWnd_FullScreen(COMMON_DATA_STRU *pObject);
extern P_MMI_Free              *const MMI_Free             ;
extern P_MMI_Malloc            *const MMI_Malloc           ;
extern P_MMI_AllocObject       *const MMI_AllocObject      ;
extern P_MMI_ObjectInit        *const MMI_ObjectInit       ;
extern P_InitWindowClass       *const InitWindowClass      ;
extern P_SetWindowItemCount    *const SetWindowItemCount   ;
extern P_SetSelMenu            *const SetSelMenu           ;
extern P_InitFrameClass        *const InitFrameClass       ;
extern P_CreateWnd_FullScreen  *const CreateWnd_FullScreen ;

// ------------------------------------------------------------------------------

typedef void * huge P_GetFahterWindowHandle(void *pObject);

/* 未肯定返回值是什么，不过无需判断该值 */
typedef void * huge P_CreateWString(void *pHeader, void *pBuffer, int nLen);

/* 设置菜单条目的软键配置 */
typedef void              huge P_SetItemSoftKey   (void *pItem, void *pKeyConfig);

/* 设置菜单条目的图像 */
typedef void              huge P_SetItemImage   (void* pItem, void* pFatherWindow, void *pKeyConfig);

/* 取得显示在"中间"的菜单索引号 */
typedef long huge P_GetSelectMenuIdx(void *pWindowHandle);

/* 为菜单条目创建文本，返回值是 *pMenuTextHandle，可用于更新文本 */
typedef void * huge P_CreateMenuText   (void *pItem, int nTextLen);

/* 通过菜单条目来更新其文本内容 */
typedef void              huge P_SetMenuTextByHandle (void *pMenuTextHandle, void *pWString);

/* 刷新菜单 */
typedef void              huge P_RefreshMenuItem     (void *pMenuItemObj,
                                                      void *pFatherWindowHandle,
                                                      void *pMenuTextHandle,
                                                      int nItemIndex);

extern P_GetFahterWindowHandle *const GetFahterWindowHandle;
extern P_CreateWString         *const CreateWString        ;
extern P_SetItemSoftKey        *const SetItemSoftKey       ;
extern P_SetItemImage          *const SetItemImage         ;
extern P_GetSelectMenuIdx      *const GetSelectMenuIdx     ;
extern P_CreateMenuText        *const CreateMenuText       ;
extern P_SetMenuTextByHandle   *const SetMenuTextByHandle  ;
extern P_RefreshMenuItem       *const RefreshMenuItem      ;

// ------------------------------------------------------------------------------
/* True or False */
typedef int               huge P_UTF8ToWString     (void *pWString, void *pText, int nTextLen);
typedef void              huge P_StoreASCStringAsID(int nStringId, void *pText, int nUnknown);
typedef void              huge P_ClearStringID     (int nStringId, int nUnknown);
extern P_UTF8ToWString         *const UTF8ToWString        ;
extern P_StoreASCStringAsID    *const StoreASCStringAsID   ;
extern P_ClearStringID         *const ClearStringID        ;


#endif
