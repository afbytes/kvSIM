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


#include <stdio.h>

#include "SlckOsFunc.h"
#include "SlckOsVar.h"
#include "kvSIM_var.h"


#pragma romdata

#define KVSIM_MENU_ITEM_COUNT   8
KV_SIM_CARD_DATA *g_SimData = (KV_SIM_CARD_DATA*)(KV_SIM_VIRTUAL_CARDS_ADDR);

unsigned int  OnKeyPress(void *pWindowHandle, void *pKeyInfoStru);
void OnRefresh(void *pItemObj, long lItemIdx);

#define MENU_IMAGE_RADIO_FULL ((COMMON_DATA_STRU *)0xE679DE)
#define MENU_IMAGE_RADIO_NULL ((COMMON_DATA_STRU *)0xE679E2)
#define MENU_IMAGE_CHECK_FULL ((COMMON_DATA_STRU *)0xE679D6)
#define MENU_IMAGE_CHECK_NULL ((COMMON_DATA_STRU *)0xE679DA)

#define WSTRING_HEADER_SIZE 20


WINDOW_CLASS KVSIM_WindowClass = {
    NULL,
    (void*)(OnKeyPress),                // 按键回调函数
    NULL,
    NULL,
    (COMMON_DATA_STRU*)0xF2BCE8,        // 窗体的软键信息(Internet -> Profile)
    (COMMON_DATA_STRU*)0xF2BCD6,        // 软键配置信息组(Internet -> Profile)
    1,                      // Window Type
    0,                      // Unknown
    (void*)(OnRefresh),        // 某菜单项被刷新时的回调函数
    NULL,                   // 菜单项结构列表
    NULL,                   // 菜单项被选中的函数列表的指针
    0                       // MenuItemCount
};

FRAME_CLASS  KVSIM_FrameClass = {
    0,
    0,
    101,                                // 条目宽度
    12,                                 // 条目高度
    (COMMON_DATA_STRU*)0xD1D26A,        // 图标配置
    KVSIM_APP_TITLE_STRING_ID,          // 标题StringID
    0x7FFF
};


/*
Return: 
 0xFFFF - 按键已处理，不退出窗口(长按红键可强行退出)
      1 - 按键已处理，退出此窗口
      0 - 按键未处理，按系统默认处理
*/
unsigned int OnKeyPress(void *pWindowHandle, void *pKeyInfoStru)
{
    int nKeyValue;
    int nMenuIndex;

#if 0
    int loop;
    unsigned char *pMyDst = (unsigned char*)0x31980;
    unsigned char *pMySrc = (unsigned char*)pKeyInfoStru;
    for(loop = 0; loop < 32; loop++)
    {
        *pMyDst++ = *pMySrc++;
    }
#endif

    // 第15个int是按键值
    nKeyValue = ((int*)pKeyInfoStru)[15];

#if 1
    // 用户按下 "选择" 键时
    if (0x04 == nKeyValue)
    {
        nMenuIndex = (int)GetSelectMenuIdx(pWindowHandle);
        
        // 选择非当前卡时
        if (nMenuIndex != KVSIM_SEL_CARD_IDX)
        {
            // 选择真实卡，或选择了IMSI非空的虚拟卡
            if ((0 == nMenuIndex)
             || ((nMenuIndex > 0) && (0 != g_SimData[nMenuIndex - 1].szImsi[0])))
            {
                KVSIM_SEL_CARD_IDX = (unsigned char)nMenuIndex;
                //KVSIM_CUR_CARD_IDX = KVSIM_SEL_CARD_IDX; // KoncaTest
                SendRebootMsg();
                return 0;
            }
        }
    }
#endif

    return 0;
}

void OnRefresh(void *pMenuItemObj, long lItemIdx)
{
    WSTRING_STRU *pWString;
    void *pFatherWindowHandle;
    void *pMenuTextHandle;
    void *pCardName;
    int nTextLen;

    unsigned char szWStringHeader[WSTRING_HEADER_SIZE];

    #define MY_WSTRING_CHAR_COUNT 30
    unsigned char szTempCardName[KV_CARD_DATA_NAME_LEN+8];
    unsigned char szWStringBuffer[MY_WSTRING_CHAR_COUNT * 2];

    if (lItemIdx >= KVSIM_MENU_ITEM_COUNT)
    {
        return;
    }

    if (lItemIdx == 0)
    {
        pCardName = (unsigned char*)KV_SIM_TRUE_CARD_NAME_ADDR;
    }
    else
    {
        // 虚拟号需将索引减1
        pCardName = g_SimData[lItemIdx - 1].szSimName;
    }

    // Add SeqNo to the Card Name
    szTempCardName[0] = '0' + lItemIdx + 1;
    szTempCardName[1] = '-';
    LIB_Memcpy(&szTempCardName[2], pCardName, KV_CARD_DATA_NAME_LEN);
    
    // 检查字符长度
    nTextLen = STRLEN(szTempCardName);
    if (nTextLen >= MY_WSTRING_CHAR_COUNT)
    {
        return;
    }

    // 创建一个WString
    pWString = (WSTRING_STRU*)CreateWString(szWStringHeader,
                                            szWStringBuffer,
                                            MY_WSTRING_CHAR_COUNT);

    UTF8ToWString(pWString, szTempCardName, nTextLen);

    // 创建菜单文本
    pMenuTextHandle = CreateMenuText(pMenuItemObj,
                                     pWString->pBuffer->nLen);

    SetMenuTextByHandle(pMenuTextHandle, pWString);
    
    // 取得父窗口句柄
    pFatherWindowHandle = GetFahterWindowHandle(pMenuItemObj);

    // 选择是哪种图标
    SetItemImage(pMenuItemObj,
                 pFatherWindowHandle,
                 (KVSIM_CUR_CARD_IDX == lItemIdx) ? MENU_IMAGE_RADIO_FULL : MENU_IMAGE_RADIO_NULL);

    // 更新软键
    SetItemSoftKey(pMenuItemObj, (COMMON_DATA_STRU *)0xF2BCDC);

    // 更新菜单
    RefreshMenuItem(pMenuItemObj,
                    pFatherWindowHandle,
                    pMenuTextHandle,
                    lItemIdx);

    return;
}

void KVSIM_MenuEntrance(void)
{
    void *pWindowObj;
    
    pWindowObj = MMI_AllocObject((void*)MMI_Malloc, (void*)MMI_Free);
    //MMI_ObjectInit(pWindowObj, NULL);
    InitWindowClass(pWindowObj, (COMMON_DATA_STRU*)(&KVSIM_WindowClass));

    // 调用条目数量
    SetWindowItemCount(pWindowObj, 8);

    // 设置位于中心的菜单项(非选中)
    SetSelMenu(pWindowObj, KVSIM_CUR_CARD_IDX);

    ClearStringID(KVSIM_APP_TITLE_STRING_ID, 0);
    StoreASCStringAsID(KVSIM_APP_TITLE_STRING_ID,
                       KVSIM_TITLE_WITH_VERSION,
                       0);
    

    // Fill it later
    InitFrameClass(pWindowObj,
                   (COMMON_DATA_STRU*)(&KVSIM_FrameClass),
                   (void*)MMI_Malloc);
    
    // 创建全屏窗口
    CreateWnd_FullScreen(pWindowObj);
}

char DefProviderName[] = "My Network";
void ReplaceProviderName(void *pOutput)
{
    unsigned char *pCardName;
    
    // 使用真实卡或当前卡号超出范围时，不修改运营商名字
    if ((INDEX_OF_TRUE_SIM_CARD == KVSIM_CUR_CARD_IDX)
     || (KVSIM_CUR_CARD_IDX > LAST_INDEX_OF_CARDS))
    {
        return;
    }
    
    // 虚拟号需将索引减1
    pCardName = g_SimData[KVSIM_CUR_CARD_IDX - 1].szSimName;
    
    // 转换和输出
    UTF8ToWString(pOutput, pCardName, STRLEN(pCardName));
}

