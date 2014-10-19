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

#ifndef __KV_SIM_VAR_H__
#define __KV_SIM_VAR_H__

// Change this three value according to 'Link Address' and VirtualSIM Data
#define KVSIM_PATCH_ORG            0xBFCB00
#define KV_SIM_TRUE_CARD_NAME_ADDR (KVSIM_PATCH_ORG + 0x20)
#define KV_SIM_VIRTUAL_CARDS_ADDR  (KVSIM_PATCH_ORG + 0x40)

#define KVSIM_APP_TITLE_STRING_ID 0x1400
#define KVSIM_TITLE_WITH_VERSION  ("kvSIM ver 0.9")

// Length of some Data
#define KI_BYTE_LEN         16
#define RAND_BYTE_LEN       16
#define SRES_BYTE_LEN       4
#define KC_BYTE_LEN         8
#define SIM_RSP_LEN         (SRES_BYTE_LEN + KC_BYTE_LEN)

#define EF_SMSP_BYTE_LEN            28
#define SERVICE_CENTRE_BYTE_LEN     12
#define SERVICE_CENTRE_OFFSET_BACK  15

#define IMSI_DATA_BYTE_LEN  9  // len:U8, BCD:U8 * 8

// Definition for SIM Card (in GSM 11.11)
#define SIM_INS_AUTH        0x88
#define SIM_INS_GET_RSP     0xC0
#define SIM_INS_SELECT      0xA4
#define SIM_INS_READ_BIN    0xB0
#define SIM_INS_READ_REC    0xB2
#define SIM_INS_UPDATE_BIN  0xD6
#define SIM_INS_UPDATE_REC  0xDC
#define SIM_INS_STATUS      0xF2

#define FILE_TYPE_MASTER        0x3F   /* Master File                            */
#define FILE_TYPE_DEDICATED     0x7F   /* Dedicated File                         */
#define FILE_TYPE_ELEMENTARY_UM 0x2F   /* Elementary File under the Master File  */
#define FILE_TYPE_ELEMENTARY_UD 0x6F   /* Elementary File under a Dedicated File */

#define FILE_ID_NULL        0x00
#define FILE_ID_IMSI        0x07
#define FILE_ID_SMSP        0x42
#define FILE_ID_LOCI        0x7E

// My Action in Rsp Function
enum
{
    RSP_ACTION_NULL,
    RSP_ACTION_DO_MY_A3A8,      // Do my A3A8 operation
    RSP_ACTION_CHANGE_DATA,     // Change Data in Rsp Function according to EF File
    RSP_ACTION_BUTT
};

// kvSIM Control Block in RAM
typedef struct _KV_SIM_CTRL_BLOCK
{
    unsigned char CurElementFileId; // Current Element File Id, only used for "ReadBinary"
    unsigned char RspAction;        // What Action should I do in 'Rsp Function'
    unsigned char DataLength;       // The length of Data to send to receieve
    unsigned char JunkByte;
    struct 
    {
        // Must use "int" or will fail to compile
        unsigned int bCtrlDataVeritied :  1;  // 是否有已检查控制数据的正确性
        unsigned int bImsiGot          :  1;
        unsigned int bLociGot          :  1;
        unsigned int bLociSetChoose    :  1;
        unsigned int bLociUpdated      :  1;
        unsigned int reserve           :  11;
    }Flags;
    unsigned char szRand[RAND_BYTE_LEN];                  // to save Rand
    unsigned char ServiceCentre[SERVICE_CENTRE_BYTE_LEN]; // saveing SMS ServiceCentre
    unsigned char LociAndCheck[12];                       // for future use
    // the next is just for temporay use
}KV_SIM_CTRL_BLOCK;

/* Data of per VirtualCard */
#define KV_CARD_DATA_NAME_LEN   16
typedef struct _KV_SIM_CARD_DATA
{
    unsigned char szSimName[KV_CARD_DATA_NAME_LEN];
    unsigned char szImsi   [16];
    unsigned char szKi     [16];
    unsigned char szSmsCentre[16];
}KV_SIM_CARD_DATA, *PKV_SIM_CARD_DATA;

#define KV_SIM_SIZE_OF_PER_V_VARD  (sizeof(KV_SIM_CARD_DATA))
#define PP_KV_SIM_CARD_ARRAIES ((KV_SIM_CARD_DATA**)(KV_SIM_VIRTUAL_CARDS_ADDR))

#define LOCI_LEN 11

typedef struct _KV_SIM_EEP_DATA
{
    unsigned char LociAndCheck1[12];
    unsigned char LociAndCheck2[12];
    unsigned char SelCardIdx; // 用户选择的卡
    unsigned char CurCardIdx; // 开机后使用的卡
}KV_SIM_EEP_DATA, *PKV_SIM_EEP_DATA;

#define MAX_SIM_CARD_COUNT          8
#define INDEX_OF_TRUE_SIM_CARD      0
#define LAST_INDEX_OF_CARDS         ((MAX_SIM_CARD_COUNT) - 1)

// Some Addresses
#define ADDR_OF_IMSI_IN_RAM ((void*)0xC85B8)

#define P_KV_EEP_DATA   ((KV_SIM_EEP_DATA*)(0xD8D60))
#if 0
#define KV_EEP_LOCI1           (P_KV_EEP_DATA->LociAndCheck1)
#define KV_EEP_LOCI2           (P_KV_EEP_DATA->LociAndCheck2)
#endif
#define KVSIM_SEL_CARD_IDX     (P_KV_EEP_DATA->SelCardIdx)
#define KVSIM_CUR_CARD_IDX     (P_KV_EEP_DATA->CurCardIdx)

#define KVSIM_RAM_BLOCK_BASE    0x031010
#define P_CTRL_BLOCK            ((KV_SIM_CTRL_BLOCK*)KVSIM_RAM_BLOCK_BASE)
#define KV_CB_CurElementFileId  (P_CTRL_BLOCK->CurElementFileId)
#define KV_CB_RSP_ACTION        (P_CTRL_BLOCK->RspAction)
#define KV_CB_DATA_LENGTH       (P_CTRL_BLOCK->DataLength)
#define KV_CB_RAND              (P_CTRL_BLOCK->szRand)
#define KV_CB_FLAG_VERIFIED     (P_CTRL_BLOCK->Flags.bCtrlDataVeritied)
#define KV_CB_FLAG_IMSI_GOT     (P_CTRL_BLOCK->Flags.bImsiGot)
#define KV_CB_FLAG_LOCI_GOT     (P_CTRL_BLOCK->Flags.bLociGot)
#define KV_CB_FLAG_LOCI_UPDATED (P_CTRL_BLOCK->Flags.bLociUpdated)
#define KV_CB_FLAG_LOCI_CHOOSE  (P_CTRL_BLOCK->Flags.bLociSetChoose)
#define KV_CB_LOCI_AND_CHECK    (P_CTRL_BLOCK->LociAndCheck)
#define KV_CB_SERVICE_CENTRE    (P_CTRL_BLOCK->ServiceCentre)

#endif
