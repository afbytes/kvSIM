; 
; kvSIM patch for Siemens 6688i(SL45i) v55lg8
; 
; Copyright (C) 2004-2007 Konca Fung.
; 
; This program is free software; you can redistribute it and/or modify it under
; the terms of the GNU General Public License as published by the Free
; Software Foundation; either version 2, or (at your option) any later
; version.
; 
; This program is distributed in the hope that it will be useful, but WITHOUT
; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
; FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
; more details.
; 
; You should have received a copy of the GNU General Public License along with
; this program; if not, write to the Free Software Foundation, Inc., 59
; Temple Place - Suite 330, Boston, MA 02111-1307, USA.
; 
; 
; Author: Konca Fung <konca@tom.com>. http://www.konca.com


;@" @(#)cstart.asm	1.94 02/01/17
;*****************************************************************************
;*
;* MODULE	: cstart.asm
;*
;* DESCRIPTION	: C startup code for non-extended C166 architecture CPU's
;*		  (e.g. SAB 80C166, GOLD)
;*
;*		- Processor initialization.
;*		- Initialization of static variables in internal or external
;*		  ram is done in '_c_init.asm'.
;*		- Call the user program: main().
;*		- On exit cpu is set in idle mode.
;*
;* Copyright 1991-2002 Altium BV
;*
;*****************************************************************************

$CASE
$GENONLY
$DEBUG
$NOLOCALS
$CHECKCPU16
$CHECKBUS18
$NOMOD166			; disable the internal set of SAB 80C166 SFRs
@IF( ! @DEFINED(__STDNAMES ) )	
	$STDNAMES(reg166.def)	; use non-extended set of SFR's by default
@ELSE
	$STDNAMES(@__STDNAMES)	; use processor specific register definition 
@ENDI

	
NAME	XCSTART			; module name.
	
EXTERN _SIM_Cmd_Hook:FAR
EXTERN _SIM_Rsp_Hook:FAR
EXTERN _KVSIM_MenuEntrance:FAR
EXTERN _ReplaceProviderName:FAR

;*****************************************************************************
;* __CSTART
;*****************************************************************************
__CSTART_MY	SECTION CODE WORD PUBLIC 'CMYINIT'
	assume DPP0:SYSTEM
__CSTART 	PROC  FAR 
;---------------------- Some Jumpers ------------------------
label_sim_begin:
    jmps label_sim_cmd_hook
    jmps label_sim_rsp_hook
    jmps label_password_parse
    jmps label_provider_name
    nop          ; \
    nop          ; / Reserved for Jumper Use 5
    nop          ; \
    nop          ; / Reserved for Jumper Use 6
    nop          ; \
    nop          ; / Reserved for Jumper Use 7
    nop          ; \
    nop          ; / Reserved for Jumper Use 8

    ;-------------- Password, Virtual SIM Card Datas ------------
        ; If something above in this file change,
        ;     Do not forget to change the 'pointer' in C Code.
    
    ; True SIM Card Name (16 bytes, with Ending Char)
    TrueCardName    db 'TrueSimCard', 0, 0, 0, 0, 0
        
    ; KVSIM_PASSWORD (8 bytes, with Ending Char)
    KVSIM_PASSWORD  db '579394', 0, 0
    JUNK_BYTES      db 0, 0, 0, 0, 0, 0, 0, 0

; (Card 2~8 is Virtual Card )
; Card 2 Data - Name (16 bytes), IMSI, KI, SMS Center
    db 'None',     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

; Card 3 Data - Name (16 bytes), IMSI, KI, SMS Center
    db 'None',     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

; Card 4 Data - Name (16 bytes), IMSI, KI, SMS Center
    db 'None',     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

; Card 5 Data - Name (16 bytes), IMSI, KI, SMS Center
    db 'None',     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

; Card 6 Data - Name (16 bytes), IMSI, KI, SMS Center
    db 'None',     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

; Card 7 Data - Name (16 bytes), IMSI, KI, SMS Center
    db 'None',     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

; Card 8 Data - Name (16 bytes), IMSI, KI, SMS Center
    db 'None',     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

;------------------------------------------------------------
label_sim_cmd_hook:
	jmps _SIM_Cmd_Hook  ; no need to go back

;------------------------------------------------------------
label_sim_rsp_hook:
	; at C5000:6E18
	mov [-r0], r1
	mov [-r0], r2
	mov [-r0], r3
	mov [-r0], r4
	mov [-r0], r5
	mov [-r0], r6
	mov [-r0], r7
	mov [-r0], r8
	mov [-r0], r9
	mov [-r0], r10
	mov [-r0], r11
 	mov [-r0], r12
	mov [-r0], r13
	mov [-r0], r14
	mov [-r0], r15
 	call _SIM_Rsp_Hook
	mov r15,[r0+]
	mov r14,[r0+]
	mov r13,[r0+]
	mov r12,[r0+]
	mov r11,[r0+]
	mov r10,[r0+]
	mov r9, [r0+]
	mov r8, [r0+]
	mov r7, [r0+]
	mov r6, [r0+]			
	mov r5, [r0+]
	mov r4, [r0+]
	mov r3, [r0+]
	mov r2, [r0+]
	mov r1, [r0+]
 
	mov DPP0, #32h
	jmps 0c5h, 6e1ch  ; go back

;---------- Parse the 'Input' in 'Main Window' ---------------------------
label_password_parse:
        mov     r12, r8
        mov     r13, r9
        calls   0C7h, 8536h  ; strlen
        cmp     r4, #0
        jmpr    cc_Z, BACK_TO_STRING_PARSE
        
        mov     r12, r8
        mov     r13, r9
        mov     r14, #POF(KVSIM_PASSWORD)
        mov     r15, #PAG(KVSIM_PASSWORD)
        calls   0C7h, 84EEh  ; strcmp
        cmp     r4, #0
        jmpr    cc_NZ, BACK_TO_STRING_PARSE
        
        ;;;; Do My work
        ;calls   SEG(KVSIM_MenuEntrance), SOF(KVSIM_MenuEntrance)
		calls _KVSIM_MenuEntrance
        
        ;;;; Go to the Exit of Org function (with Code 4)
        jmps    0A3h, 0B06h
    
    
    BACK_TO_STRING_PARSE:
        mov     r12, r0
        and     r12, #3FFFh
        jmps    0A3h, 08F2h
	; -------- Parse Input End --------

;-------- Replace Network Provider Name with SIM Card Name ------
label_provider_name:
    add   r0,  #26h  ; - run the replaced "Code"
    mov   r12, r8    ; \
    mov   r13, r9    ; / use "pointer of Output" as 'Parameter'
    calls _ReplaceProviderName
    jmps  0D3h, 8FEEh    ; jump back

label_end:
	ret
__CSTART	ENDP


__CSTART_MY	ENDS

CSTART_RBANK    REGDEF R0-R15
			
	END
