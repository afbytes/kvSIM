#include "a3a8.h"

// must use the 'romdata' to ensure the data is saved in rom
#pragma romdata

/* An implementation of the GSM A3A8 algorithm.  (Specifically, COMP128.) 
 */

/* Copyright 1998, Marc Briceno, Ian Goldberg, and David Wagner.
 * All rights reserved.
 */

/* These are the COMP128 compression tables */
const unsigned char table_0[512] = {
  0x66, 0xB1, 0xBA, 0xA2, 0x02, 0x9C, 0x70, 0x4B,
  0x37, 0x19, 0x08, 0x0C, 0xFB, 0xC1, 0xF6, 0xBC,
  0x6D, 0xD5, 0x97, 0x35, 0x2A, 0x4F, 0xBF, 0x73,
  0xE9, 0xF2, 0xA4, 0xDF, 0xD1, 0x94, 0x6C, 0xA1,
  0xFC, 0x25, 0xF4, 0x2F, 0x40, 0xD3, 0x06, 0xED,
  0xB9, 0xA0, 0x8B, 0x71, 0x4C, 0x8A, 0x3B, 0x46,
  0x43, 0x1A, 0x0D, 0x9D, 0x3F, 0xB3, 0xDD, 0x1E,
  0xD6, 0x24, 0xA6, 0x45, 0x98, 0x7C, 0xCF, 0x74,
  0xF7, 0xC2, 0x29, 0x54, 0x47, 0x01, 0x31, 0x0E,
  0x5F, 0x23, 0xA9, 0x15, 0x60, 0x4E, 0xD7, 0xE1,
  0xB6, 0xF3, 0x1C, 0x5C, 0xC9, 0x76, 0x04, 0x4A,
  0xF8, 0x80, 0x11, 0x0B, 0x92, 0x84, 0xF5, 0x30,
  0x95, 0x5A, 0x78, 0x27, 0x57, 0xE6, 0x6A, 0xE8,
  0xAF, 0x13, 0x7E, 0xBE, 0xCA, 0x8D, 0x89, 0xB0,
  0xFA, 0x1B, 0x65, 0x28, 0xDB, 0xE3, 0x3A, 0x14,
  0x33, 0xB2, 0x62, 0xD8, 0x8C, 0x16, 0x20, 0x79,
  0x3D, 0x67, 0xCB, 0x48, 0x1D, 0x6E, 0x55, 0xD4,
  0xB4, 0xCC, 0x96, 0xB7, 0x0F, 0x42, 0xAC, 0xC4,
  0x38, 0xC5, 0x9E, 0x00, 0x64, 0x2D, 0x99, 0x07,
  0x90, 0xDE, 0xA3, 0xA7, 0x3C, 0x87, 0xD2, 0xE7,
  0xAE, 0xA5, 0x26, 0xF9, 0xE0, 0x22, 0xDC, 0xE5,
  0xD9, 0xD0, 0xF1, 0x44, 0xCE, 0xBD, 0x7D, 0xFF,
  0xEF, 0x36, 0xA8, 0x59, 0x7B, 0x7A, 0x49, 0x91,
  0x75, 0xEA, 0x8F, 0x63, 0x81, 0xC8, 0xC0, 0x52,
  0x68, 0xAA, 0x88, 0xEB, 0x5D, 0x51, 0xCD, 0xAD,
  0xEC, 0x5E, 0x69, 0x34, 0x2E, 0xE4, 0xC6, 0x05,
  0x39, 0xFE, 0x61, 0x9B, 0x8E, 0x85, 0xC7, 0xAB,
  0xBB, 0x32, 0x41, 0xB5, 0x7F, 0x6B, 0x93, 0xE2,
  0xB8, 0xDA, 0x83, 0x21, 0x4D, 0x56, 0x1F, 0x2C,
  0x58, 0x3E, 0xEE, 0x12, 0x18, 0x2B, 0x9A, 0x17,
  0x50, 0x9F, 0x86, 0x6F, 0x09, 0x72, 0x03, 0x5B,
  0x10, 0x82, 0x53, 0x0A, 0xC3, 0xF0, 0xFD, 0x77,
  0xB1, 0x66, 0xA2, 0xBA, 0x9C, 0x02, 0x4B, 0x70,
  0x19, 0x37, 0x0C, 0x08, 0xC1, 0xFB, 0xBC, 0xF6,
  0xD5, 0x6D, 0x35, 0x97, 0x4F, 0x2A, 0x73, 0xBF,
  0xF2, 0xE9, 0xDF, 0xA4, 0x94, 0xD1, 0xA1, 0x6C,
  0x25, 0xFC, 0x2F, 0xF4, 0xD3, 0x40, 0xED, 0x06,
  0xA0, 0xB9, 0x71, 0x8B, 0x8A, 0x4C, 0x46, 0x3B,
  0x1A, 0x43, 0x9D, 0x0D, 0xB3, 0x3F, 0x1E, 0xDD,
  0x24, 0xD6, 0x45, 0xA6, 0x7C, 0x98, 0x74, 0xCF,
  0xC2, 0xF7, 0x54, 0x29, 0x01, 0x47, 0x0E, 0x31,
  0x23, 0x5F, 0x15, 0xA9, 0x4E, 0x60, 0xE1, 0xD7,
  0xF3, 0xB6, 0x5C, 0x1C, 0x76, 0xC9, 0x4A, 0x04,
  0x80, 0xF8, 0x0B, 0x11, 0x84, 0x92, 0x30, 0xF5,
  0x5A, 0x95, 0x27, 0x78, 0xE6, 0x57, 0xE8, 0x6A,
  0x13, 0xAF, 0xBE, 0x7E, 0x8D, 0xCA, 0xB0, 0x89,
  0x1B, 0xFA, 0x28, 0x65, 0xE3, 0xDB, 0x14, 0x3A,
  0xB2, 0x33, 0xD8, 0x62, 0x16, 0x8C, 0x79, 0x20,
  0x67, 0x3D, 0x48, 0xCB, 0x6E, 0x1D, 0xD4, 0x55,
  0xCC, 0xB4, 0xB7, 0x96, 0x42, 0x0F, 0xC4, 0xAC,
  0xC5, 0x38, 0x00, 0x9E, 0x2D, 0x64, 0x07, 0x99,
  0xDE, 0x90, 0xA7, 0xA3, 0x87, 0x3C, 0xE7, 0xD2,
  0xA5, 0xAE, 0xF9, 0x26, 0x22, 0xE0, 0xE5, 0xDC,
  0xD0, 0xD9, 0x44, 0xF1, 0xBD, 0xCE, 0xFF, 0x7D,
  0x36, 0xEF, 0x59, 0xA8, 0x7A, 0x7B, 0x91, 0x49,
  0xEA, 0x75, 0x63, 0x8F, 0xC8, 0x81, 0x52, 0xC0,
  0xAA, 0x68, 0xEB, 0x88, 0x51, 0x5D, 0xAD, 0xCD,
  0x5E, 0xEC, 0x34, 0x69, 0xE4, 0x2E, 0x05, 0xC6,
  0xFE, 0x39, 0x9B, 0x61, 0x85, 0x8E, 0xAB, 0xC7,
  0x32, 0xBB, 0xB5, 0x41, 0x6B, 0x7F, 0xE2, 0x93,
  0xDA, 0xB8, 0x21, 0x83, 0x56, 0x4D, 0x2C, 0x1F,
  0x3E, 0x58, 0x12, 0xEE, 0x2B, 0x18, 0x17, 0x9A,
  0x9F, 0x50, 0x6F, 0x86, 0x72, 0x09, 0x5B, 0x03,
  0x82, 0x10, 0x0A, 0x53, 0xF0, 0xC3, 0x77, 0xFD
};

const unsigned char table_1[256] = {
  0x13, 0x0B, 0x50, 0x72, 0x2B, 0x01, 0x45, 0x5E,
  0x27, 0x12, 0x7F, 0x75, 0x61, 0x03, 0x55, 0x2B,
  0x1B, 0x7C, 0x46, 0x53, 0x2F, 0x47, 0x3F, 0x0A,
  0x2F, 0x59, 0x4F, 0x04, 0x0E, 0x3B, 0x0B, 0x05,
  0x23, 0x6B, 0x67, 0x44, 0x15, 0x56, 0x24, 0x5B,
  0x55, 0x7E, 0x20, 0x32, 0x6D, 0x5E, 0x78, 0x06,
  0x35, 0x4F, 0x1C, 0x2D, 0x63, 0x5F, 0x29, 0x22,
  0x58, 0x44, 0x5D, 0x37, 0x6E, 0x7D, 0x69, 0x14,
  0x5A, 0x50, 0x4C, 0x60, 0x17, 0x3C, 0x59, 0x40,
  0x79, 0x38, 0x0E, 0x4A, 0x65, 0x08, 0x13, 0x4E,
  0x4C, 0x42, 0x68, 0x2E, 0x6F, 0x32, 0x20, 0x03,
  0x27, 0x00, 0x3A, 0x19, 0x5C, 0x16, 0x12, 0x33,
  0x39, 0x41, 0x77, 0x74, 0x16, 0x6D, 0x07, 0x56,
  0x3B, 0x5D, 0x3E, 0x6E, 0x4E, 0x63, 0x4D, 0x43,
  0x0C, 0x71, 0x57, 0x62, 0x66, 0x05, 0x58, 0x21,
  0x26, 0x38, 0x17, 0x08, 0x4B, 0x2D, 0x0D, 0x4B,
  0x5F, 0x3F, 0x1C, 0x31, 0x7B, 0x78, 0x14, 0x70,
  0x2C, 0x1E, 0x0F, 0x62, 0x6A, 0x02, 0x67, 0x1D,
  0x52, 0x6B, 0x2A, 0x7C, 0x18, 0x1E, 0x29, 0x10,
  0x6C, 0x64, 0x75, 0x28, 0x49, 0x28, 0x07, 0x72,
  0x52, 0x73, 0x24, 0x70, 0x0C, 0x66, 0x64, 0x54,
  0x5C, 0x30, 0x48, 0x61, 0x09, 0x36, 0x37, 0x4A,
  0x71, 0x7B, 0x11, 0x1A, 0x35, 0x3A, 0x04, 0x09,
  0x45, 0x7A, 0x15, 0x76, 0x2A, 0x3C, 0x1B, 0x49,
  0x76, 0x7D, 0x22, 0x0F, 0x41, 0x73, 0x54, 0x40,
  0x3E, 0x51, 0x46, 0x01, 0x18, 0x6F, 0x79, 0x53,
  0x68, 0x51, 0x31, 0x7F, 0x30, 0x69, 0x1F, 0x0A,
  0x06, 0x5B, 0x57, 0x25, 0x10, 0x36, 0x74, 0x7E,
  0x1F, 0x26, 0x0D, 0x00, 0x48, 0x6A, 0x4D, 0x3D,
  0x1A, 0x43, 0x2E, 0x1D, 0x60, 0x25, 0x3D, 0x34,
  0x65, 0x11, 0x2C, 0x6C, 0x47, 0x34, 0x42, 0x39,
  0x21, 0x33, 0x19, 0x5A, 0x02, 0x77, 0x7A, 0x23
};

const unsigned char table_2[128] = {
  0x34, 0x32, 0x2C, 0x06, 0x15, 0x31, 0x29, 0x3B,
  0x27, 0x33, 0x19, 0x20, 0x33, 0x2F, 0x34, 0x2B,
  0x25, 0x04, 0x28, 0x22, 0x3D, 0x0C, 0x1C, 0x04,
  0x3A, 0x17, 0x08, 0x0F, 0x0C, 0x16, 0x09, 0x12,
  0x37, 0x0A, 0x21, 0x23, 0x32, 0x01, 0x2B, 0x03,
  0x39, 0x0D, 0x3E, 0x0E, 0x07, 0x2A, 0x2C, 0x3B,
  0x3E, 0x39, 0x1B, 0x06, 0x08, 0x1F, 0x1A, 0x36,
  0x29, 0x16, 0x2D, 0x14, 0x27, 0x03, 0x10, 0x38,
  0x30, 0x02, 0x15, 0x1C, 0x24, 0x2A, 0x3C, 0x21,
  0x22, 0x12, 0x00, 0x0B, 0x18, 0x0A, 0x11, 0x3D,
  0x1D, 0x0E, 0x2D, 0x1A, 0x37, 0x2E, 0x0B, 0x11,
  0x36, 0x2E, 0x09, 0x18, 0x1E, 0x3C, 0x20, 0x00,
  0x14, 0x26, 0x02, 0x1E, 0x3A, 0x23, 0x01, 0x10,
  0x38, 0x28, 0x17, 0x30, 0x0D, 0x13, 0x13, 0x1B,
  0x1F, 0x35, 0x2F, 0x26, 0x3F, 0x0F, 0x31, 0x05,
  0x25, 0x35, 0x19, 0x24, 0x3F, 0x1D, 0x05, 0x07
};

const unsigned char table_3[64] = {
  0x01, 0x05, 0x1D, 0x06, 0x19, 0x01, 0x12, 0x17,
  0x11, 0x13, 0x00, 0x09, 0x18, 0x19, 0x06, 0x1F,
  0x1C, 0x14, 0x18, 0x1E, 0x04, 0x1B, 0x03, 0x0D,
  0x0F, 0x10, 0x0E, 0x12, 0x04, 0x03, 0x08, 0x09,
  0x14, 0x00, 0x0C, 0x1A, 0x15, 0x08, 0x1C, 0x02,
  0x1D, 0x02, 0x0F, 0x07, 0x0B, 0x16, 0x0E, 0x0A,
  0x11, 0x15, 0x0C, 0x1E, 0x1A, 0x1B, 0x10, 0x1F,
  0x0B, 0x07, 0x0D, 0x17, 0x0A, 0x05, 0x16, 0x13
};

const unsigned char table_4[32] = {
  0x0F, 0x0C, 0x0A, 0x04, 0x01, 0x0E, 0x0B, 0x07,
  0x05, 0x00, 0x0E, 0x07, 0x01, 0x02, 0x0D, 0x08,
  0x0A, 0x03, 0x04, 0x09, 0x06, 0x00, 0x03, 0x02,
  0x05, 0x06, 0x08, 0x09, 0x0B, 0x0D, 0x0F, 0x0C
};

const unsigned char *table[5] = {
  table_0, table_1, table_2, table_3, table_4
};

// ---------------------------------------------------------------------

/*
 * rand[0..15]: the challenge from the base station
 * key[0..15]: the SIM's A3/A8 long-term key Ki
 * simoutput[0..11]: what you'd get back if you fed rand and key to a real
 * SIM.
 *
 *   The GSM spec states that simoutput[0..3] is SRES,
 *   and simoutput[4..11] is Kc (the A5 session key).
 *   (See GSM 11.11, Section 8.16.  See also the leaked document
 *   referenced below.)
 *   Note that Kc is bits 74..127 of the COMP128 output, followed by 10
 *   zeros.
 *   In other words, A5 is keyed with only 54 bits of entropy. This
 *   represents a deliberate weakening of the key used for voice privacy
 *   by a factor of over 1000.
 * 
 * Verified with a Pacific Bell Schlumberger SIM.  Your mileage may vary.
 *
 * Marc Briceno <marc@scard.org>, Ian Goldberg <iang@cs.berkeley.edu>,
 * and David Wagner <daw@cs.berkeley.edu>
 */
/*
 * This code derived from a leaked document from the GSM standards.
 * Some missing pieces were filled in by reverse-engineering a working SIM.
 * We have verified that this is the correct COMP128 algorithm.
 * 
 * The first page of the document identifies it as
 *      _Technical Information: GSM System Security Study_.
 *      10-1617-01, 10th June 1988.
 * The bottom of the title page is marked
 *      Racal Research Ltd.
 *      Worton Drive, Worton Grange Industrial Estate,
 *      Reading, Berks. RG2 0SB, England.
 *      Telephone: Reading (0734) 868601   Telex: 847152
 * The relevant bits are in Part I, Section 20 (pages 66--67).  Enjoy!
 * 
 * Note: There are three typos in the spec (discovered by
 * reverse-engineering).
 * First, "z = (2 * x[n] + x[n]) mod 2^(9-j)" should clearly read
 * "z = (2 * x[m] + x[n]) mod 2^(9-j)".
 * Second, the "k" loop in the "Form bits from bytes" section is severely
 * botched: the k index should run only from 0 to 3, and clearly the range
 * on "the (8-k)th bit of byte j" is also off (should be 0..7, not 1..8,
 * to be consistent with the subsequent section).
 * Third, SRES is taken from the first 8 nibbles of x[], not the last 8 as
 * claimed in the document.  (And the document doesn't specify how Kc is
 * derived, but that was also easily discovered with reverse engineering.)
 * All of these typos have been corrected in the following code.
 */
void A3A8(/* in */ const unsigned char rand[16], /* in */ const unsigned char key[16],
    /* out */ unsigned char simoutput[12])
{
    unsigned char x[32], bits[128];
    int i, j, k, l, m, n, y, z, next_bit;

    /* ( Load RAND into last 16 bytes of input ) */
    for (i=16; i<32; i++)
        x[i] = rand[i-16];

    /* ( Loop eight times ) */
    for (i=1; i<9; i++) 
    {
        /* ( Load key into first 16 bytes of input ) */
        for (j=0; j<16; j++)
            x[j] = key[j];
        /* ( Perform substitutions ) */
        for (j=0; j<5; j++)
            for (k=0; k<(1<<j); k++)
                for (l=0; l<(1<<(4-j)); l++) 
                {
                    m = l + k*(1<<(5-j));
                    n = m + (1<<(4-j));
                    y = (x[m]+2*x[n]) % (1<<(9-j));
                    z = (2*x[m]+x[n]) % (1<<(9-j));
                    x[m] = table[j][y];
                    x[n] = table[j][z];
                }
        /* ( Form bits from bytes ) */
        for (j=0; j<32; j++)
            for (k=0; k<4; k++)
                bits[4*j+k] = (x[j]>>(3-k)) & 1;
        /* ( Permutation but not on the last loop ) */
        if (i < 8)
        {
            for (j=0; j<16; j++) 
            {
                x[j+16] = 0;
                for (k=0; k<8; k++) 
                {
                    next_bit = ((8*j + k)*17) % 128;
                    x[j+16] |= bits[next_bit] << (7-k);
                }
            }
        }
    }

    /*
     * ( At this stage the vector x[] consists of 32 nibbles.
     *   The first 8 of these are taken as the output SRES. )
     */

    /* The remainder of the code is not given explicitly in the
     * standard, but was derived by reverse-engineering.
     */

    for (i=0; i<4; i++)
        simoutput[i] = (x[2*i]<<4) | x[2*i+1];
    for (i=0; i<6; i++)
        simoutput[4+i] = (x[2*i+18]<<6) | (x[2*i+18+1]<<2)
                | (x[2*i+18+2]>>2);
    simoutput[4+6] = (x[2*6+18]<<6) | (x[2*6+18+1]<<2);
    simoutput[4+7] = 0;
}


