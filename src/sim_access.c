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

#include "SlckOsVar.h"
#include "SlckOsFunc.h"
#include "kvSIM_var.h"
#include "a3a8.h"

// must use the 'romdata' to ensure the data is saved in rom
#pragma romdata

const unsigned char g_ChosenLoci[LOCI_LEN] =
    {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xF0, 0x00, 0xFF, 0xFE, 0x00, 0x00};

KV_SIM_CARD_DATA *g_pSimArray = (KV_SIM_CARD_DATA*)(KV_SIM_VIRTUAL_CARDS_ADDR);
#if 0
void MakeLociCheckSum(unsigned char *LociBuf)
{
    unsigned char CheckSum;
    unsigned char loop;

    if (NULL == LociBuf) return;

    CheckSum = 0;
    for (loop = 0; loop < LOCI_LEN; loop++)
    {
        CheckSum += *LociBuf++;
    }
    CheckSum = ~CheckSum;

    // set the value
    *LociBuf = CheckSum;
}

// 1 for OK, 0 for NG
unsigned char TestLociCheckSum(unsigned char *LociBuf)
{
    unsigned char CheckSum;
    unsigned char loop;

    if (NULL == LociBuf) return 0; // parameter null, return NG

    CheckSum = 0;
    for (loop = 0; loop < LOCI_LEN; loop++)
    {
        CheckSum += *LociBuf++;
    }
    CheckSum = ~CheckSum;

    return ((*LociBuf) == CheckSum);
}
#endif

void MakeChooseLoci(unsigned char *pLoci, unsigned char *pImsi)
{
    unsigned char loop;
    if (NULL == pLoci) return;

    if (NULL == pImsi)
    {
        for (loop = 0; loop < 4; loop++)
        {
            pLoci[loop] = g_ChosenLoci[loop];
        }
        for (loop = 7; loop < LOCI_LEN; loop++)
        {
            pLoci[loop] = g_ChosenLoci[loop];
        }
    }
    else
    {
        // set the LOCI to be "Choose"
        for (loop = 0; loop < LOCI_LEN; loop++)
        {
            pLoci[loop] = g_ChosenLoci[loop];
        }
        
        // Set the MNC from IMSI
        pLoci[4] = ((pImsi[2] & 0x0F) << 4) | (pImsi[1] >> 4);
        pLoci[5] = (0xF0) | (pImsi[2] >> 4);
        pLoci[6] = pImsi[3];
    }
}

typedef struct _LOG_UNIT
{
    unsigned char ins;
    unsigned char p1;
    unsigned char p2;
    unsigned char sendLen;
    unsigned char recvLen;
    unsigned char sendByte0;
    unsigned char sendByte1;
    unsigned char recvByte0;
}LOG_UNIT;

/*
    对于一般的 BIN 格式的EF，6688会有下列两种读取的指令顺序:
      1. Select, GetResponse, ReadBinary;
      2. Select, ReadBinary (即不经过GetResponse确认)
    所以，需兼容第二种读取方式。
*/

// check the SIM idx and the kvSIM data
void VerifyData(void)
{
    // "选定卡" 索引超出范围时归零
    if (KVSIM_SEL_CARD_IDX > LAST_INDEX_OF_CARDS)
    {
        KVSIM_SEL_CARD_IDX = INDEX_OF_TRUE_SIM_CARD;
    }
    
    // 用户选择虚拟卡时，如果IMSI为空则变为真实卡
    if (INDEX_OF_TRUE_SIM_CARD != KVSIM_SEL_CARD_IDX)
    {
        // 虚拟卡编号从1开始，所以需减1
        if (0 == g_pSimArray[KVSIM_SEL_CARD_IDX-1].szImsi[0])
        {
            KVSIM_SEL_CARD_IDX = INDEX_OF_TRUE_SIM_CARD;
        }
    }
    
    // 用户未换卡，无需特殊处理LOCI
    if (KVSIM_CUR_CARD_IDX == KVSIM_SEL_CARD_IDX)
    {
        KV_CB_FLAG_LOCI_CHOOSE = 0;
    }
    // 用户换卡时，更新 "当前卡"，并将 "新选网" 标志置上
    else
    {
        KVSIM_CUR_CARD_IDX = KVSIM_SEL_CARD_IDX;
        KV_CB_FLAG_LOCI_CHOOSE = 1; // 表示需使用CHOOSE的LOCI
    }
}


typedef struct _MyLogData
{
    int count;
    LOG_UNIT unit[1];
}MyLogData;
#define FREE_STACK_BEGIN 0xAC100
#define FREE_STACK_END   0xB0000
#define MAX_LOG_COUNT ((FREE_STACK_END - FREE_STACK_BEGIN) / sizeof(LOG_UNIT))

void SIM_Cmd_Hook(int what, int cla, int ins,
                int p1, int p2, int rw,
                int SendLen, unsigned char *SendBuf,
                int RecvLen, unsigned char *RecvBuf)
{
#if 0
    // 此代码仅用于测试
    MyLogData *pLog = (MyLogData*)(FREE_STACK_BEGIN-sizeof(int));
    if (pLog->count < MAX_LOG_COUNT)
    {
        pLog->unit[pLog->count].ins     = ins;
        pLog->unit[pLog->count].p1      = p1;
        pLog->unit[pLog->count].p2      = p2;
        pLog->unit[pLog->count].sendLen = SendLen;
        pLog->unit[pLog->count].recvLen = RecvLen;
        pLog->unit[pLog->count].sendByte0 = SendBuf[0];
        pLog->unit[pLog->count].sendByte1 = SendBuf[1];
        pLog->unit[pLog->count].recvByte0 = RecvBuf[0];
        
        (pLog->count)++;
    }
#endif

    // 开机后进行一次数据检查
    if (!KV_CB_FLAG_VERIFIED)
    {
        VerifyData();
        
        KV_CB_FLAG_VERIFIED = 1;
    }

    /* Clear it first, and it will be set when necessary following.
       And only 'GetResponse' can be skipped. */
    if (SIM_INS_GET_RSP != ins)
    {
        KV_CB_RSP_ACTION = RSP_ACTION_NULL;
    }

    switch (ins)
    {
        case SIM_INS_AUTH:
            // Save the 'RAND' to RAM
            LIB_Memcpy(KV_CB_RAND, SendBuf, RAND_BYTE_LEN);

            // we should Do our A3A8 operation
            KV_CB_RSP_ACTION = RSP_ACTION_DO_MY_A3A8;
            break;
        case SIM_INS_SELECT:
            // save the 'Current EF File ID'
            if (FILE_TYPE_ELEMENTARY_UD == SendBuf[0])
            {
                KV_CB_CurElementFileId   = SendBuf[1];
            }
            
            break;
        case SIM_INS_READ_BIN:
            switch (KV_CB_CurElementFileId)
            {
                // add the File Id we are care here
                case FILE_ID_IMSI:
                case FILE_ID_LOCI:
                    KV_CB_RSP_ACTION = RSP_ACTION_CHANGE_DATA;
                    break;
                default:
                    KV_CB_RSP_ACTION = RSP_ACTION_NULL;
                    break;
            }
            break;
        case SIM_INS_READ_REC:
            switch (KV_CB_CurElementFileId)
            {
                // add the File Id we are care here
                case FILE_ID_SMSP:
                    // only change the "First Record" of SMSP
                    if ((1 == p1) && (4 == p2))
                    {
                        KV_CB_RSP_ACTION  = RSP_ACTION_CHANGE_DATA;
                        
                        // save the length of data to receieve
                        KV_CB_DATA_LENGTH = RecvLen;
                    }
                    break;
                default:
                    KV_CB_RSP_ACTION = RSP_ACTION_NULL;
                    break;
            }
            break;
        case SIM_INS_UPDATE_REC:
            {
                switch (KV_CB_CurElementFileId)
                {
                    // add the File Id we are care here
                    case FILE_ID_SMSP:
                        // only change the "First Record" of SMSP
                        if ((1 == p1) && (4 == p2))
                        {
                            /* Not using 'True Card', restore the Service Centre backuped.
                               so, user can only update the ServiceCentre when he is using TrueCard. */
                            if (INDEX_OF_TRUE_SIM_CARD != KVSIM_CUR_CARD_IDX)
                            {
                                LIB_Memcpy((char*)SIM_LAST_BUF_POINTER + (SendLen - SERVICE_CENTRE_OFFSET_BACK),
                                           KV_CB_SERVICE_CENTRE,
                                           SERVICE_CENTRE_BYTE_LEN);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
            break;
#if 0
        // for kvSIM 0.8 : 暂不处理向SIM卡里更新数据的情况
        case SIM_INS_UPDATE_BIN:
            switch (KV_CB_FILE_ID)
            {
                case FILE_ID_LOCI:
                    KV_CB_FLAG_LOCI_CHOOSE = 0; // 表示不再使用LOCI $
                    KV_CB_FILE_ID = FILE_ID_NULL; // 后面不关心，所以清掉
            #if 0
                    // 暂不关心，校验
                    if (!TestLociCheckSum(KV_CB_LOCI_AND_CHECK))
                    {
                        // 如果记录的SIM卡数据被改，则只保证使其为$
                    }
            #endif
                    /*
                    if (INDEX_OF_VIRTUAL_CARD1 == KVSIM_CUR_CARD_IDX)
                    {
                        LIB_Memcpy(KV_EEP_LOCI1, SendBuf, LOCI_LEN);
                        //MakeLociCheckSum(KV_EEP_LOCI1); // 产生校验位
                        KV_CB_FLAG_LOCI_UPDATED = 1;
                    }
                    if (INDEX_OF_VIRTUAL_CARD2 == KVSIM_CUR_CARD_IDX)
                    {
                        LIB_Memcpy(KV_EEP_LOCI2, SendBuf, LOCI_LEN);
                        //MakeLociCheckSum(KV_EEP_LOCI2); // 产生校验位
                        KV_CB_FLAG_LOCI_UPDATED = 1;
                    }
                    // 使用内存中记录的SIM卡中的LOCI数据保存回SIM卡中
                    LIB_Memcpy(SendBuf, KV_CB_LOCI_AND_CHECK, LOCI_LEN);
                    */
                    break;
                default:
                    break;
            }
            break;
#endif
        default:
            break;
    }
    
    // call the true function
    SIM_Access(what, cla, ins, p1, p2, rw,
            SendLen, SendBuf, RecvLen, RecvBuf);
}

// please do to pay attention to the Registers used in this function
// and modify the Regiesters that should be saved in the 'cstart.asm'
void SIM_Rsp_Hook(void)
{
    unsigned char CardIndex;

    // Do nothing when 'Action' invalid
    if (RSP_ACTION_NULL == KV_CB_RSP_ACTION)      return;

    // out of range
    if (KVSIM_CUR_CARD_IDX > LAST_INDEX_OF_CARDS) return;

    // 对 "真实卡" 的处理分支 - 只需处理换卡时的LOCI的修改即可
    if (INDEX_OF_TRUE_SIM_CARD == KVSIM_CUR_CARD_IDX)
    {
        if ((SIM_INS_READ_BIN       == SIM_LAST_INS)
         && (RSP_ACTION_CHANGE_DATA == KV_CB_RSP_ACTION)
         && (FILE_ID_LOCI           == KV_CB_CurElementFileId))
        {
            // 当用户改变SIM卡时将LOCI设置为 CHOOSE
            //   注：因为手机可能取多次LOCI，所以并不清除该标志。
            //       该标志只有在不改变SIM卡而重新开机时才未置位
            if (KV_CB_FLAG_LOCI_CHOOSE)
            {
                // 从内存的IMSI中生成CHOOSE 的 LOCI
                MakeChooseLoci(SIM_LAST_BUF_POINTER,
                               ADDR_OF_IMSI_IN_RAM);
            }
            
            // 标为 "曾经取过LOCI"
            KV_CB_FLAG_LOCI_GOT = 1;
            
            // Clear the Action
            KV_CB_RSP_ACTION = RSP_ACTION_NULL;
        }
    }
    // 对 "虚拟卡" 的处理分支
    else
    {
        // 真实卡编号为0, 虚拟卡由1开始, 在数组中需减1
        CardIndex = KVSIM_CUR_CARD_IDX - 1;
        
        switch (SIM_LAST_INS)
        {
            case SIM_INS_GET_RSP:
                // 执行A3A8指令后，会使用GetResponse指令来取结果
                if (RSP_ACTION_DO_MY_A3A8 == KV_CB_RSP_ACTION)
                {
                    A3A8(KV_CB_RAND,
                         g_pSimArray[CardIndex].szKi,
                         SIM_LAST_BUF_POINTER);

                    KV_CB_RSP_ACTION = RSP_ACTION_NULL;  // Clear the Action
                }
                break;
            case SIM_INS_READ_BIN:
                {
                    // 读Binary文件，且需修改数据时则根据 '当前EF' 进行相应的修改
                    if (RSP_ACTION_CHANGE_DATA == KV_CB_RSP_ACTION)
                    {
                        switch (KV_CB_CurElementFileId)
                        {
                            case FILE_ID_IMSI:
                                KV_CB_FLAG_IMSI_GOT = 1;
                                
                                LIB_Memcpy(SIM_LAST_BUF_POINTER,
                                           g_pSimArray[CardIndex].szImsi,
                                           IMSI_DATA_BYTE_LEN);
                                break;

                            case FILE_ID_LOCI:
                                // 标为 "曾经取过LOCI"
                                KV_CB_FLAG_LOCI_GOT = 1;
                                
                                // 当用户改变SIM卡时将LOCI设置为 CHOOSE
                                //   注：因为手机可能取多次LOCI，所以并不清除该标志。
                                //       该标志只有在不改变SIM卡而重新开机时才未置位
                                if (KV_CB_FLAG_LOCI_CHOOSE)
                                {
                                    // 从内存的IMSI中生成CHOOSE 的 LOCI
                                    MakeChooseLoci(SIM_LAST_BUF_POINTER,
                                                   ADDR_OF_IMSI_IN_RAM);
                                }
                                break;
                            default:
                                break;
                        }

                        // Clear the Action
                        KV_CB_RSP_ACTION = RSP_ACTION_NULL;
                    }
                }
                break;
            case SIM_INS_READ_REC:
                {
                    if (RSP_ACTION_CHANGE_DATA == KV_CB_RSP_ACTION)
                    {
                        switch (KV_CB_CurElementFileId)
                        {
                            case FILE_ID_SMSP:
                                // Backup the one from the SIM Card
                                LIB_Memcpy(KV_CB_SERVICE_CENTRE,
                                           (char*)SIM_LAST_BUF_POINTER + (KV_CB_DATA_LENGTH - SERVICE_CENTRE_OFFSET_BACK),
                                           SERVICE_CENTRE_BYTE_LEN);
                                // Replace Service Centre with the one of Virtual Card
                                LIB_Memcpy((char*)SIM_LAST_BUF_POINTER + (KV_CB_DATA_LENGTH - SERVICE_CENTRE_OFFSET_BACK),
                                           g_pSimArray[CardIndex].szSmsCentre,
                                           SERVICE_CENTRE_BYTE_LEN);
                                break;
                            default:
                                break;
                        }
                        
                        // Clear the Action
                        KV_CB_RSP_ACTION = RSP_ACTION_NULL;
                    }
                }
                break;
            default:
                break;
        }
    }
}

