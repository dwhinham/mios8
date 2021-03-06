; $Id$
;
; MIOS Application
; MIDIO128
;
; ==========================================================================
;
;  Copyright 1998-2003 Thorsten Klose (tk@midibox.org)
;  Licensed for personal non-commercial use only.
;  All other rights reserved.
; 
; ==========================================================================

;; ---[ MIOS header file ]---
#include <mios.h>

;; ---[ useful macros ]---
#include <macros.h>

;; ---[ vectors to MIOS functions (never change!) ]---
#include <mios_vectors.inc>

;; ---[ user hooks (never change!) ]---
#include <user_vectors.inc>

;; ==========================================================================
;;  General Application Settings
;; ==========================================================================

;; ==========================================================================
;;  Control surface settings
;; ==========================================================================

;; ---[ variables used by application ]---
#include "app_defines.h"

; ==========================================================================

;; ---[ configuration table for MIDI processor and rotary encoders ]---
#include <mios_mt_table.inc>	; use dummy table located in $MIOS_PATH/include/asm
#include <mios_enc_table.inc>	; use dummy table located in $MIOS_PATH/include/asm

;; ---[ Custom LCD driver ]---
#include <app_lcd.inc>

;; ==========================================================================
;;  Standard MIOS hooks
;; ==========================================================================

;; --------------------------------------------------------------------------
;;  This function is called by MIOS after startup to initialize the 
;;  application
;; --------------------------------------------------------------------------
USER_Init
	;; initialize the shift registers
	movlw	0x10			; use all shift registers
	call	MIOS_SRIO_NumberSet
	movlw	0x01			; set update frequncy to 1/1 mS
	call	MIOS_SRIO_UpdateFrqSet
	movlw	DEFAULT_SRIO_DEBOUNCE_CTR ; set debounce counter
	call	MIOS_SRIO_DebounceSet

	;; the touch sensor sensitivity is defined in the SysEx dump (see MIDIO_presets.inc)
	;; and will be set in MIDIO_dump.inc
	movlw	0x00
	call	MIOS_SRIO_TS_SensitivitySet

	;; ------------------------------------------------------------------

	;; get device ID
#if DEFAULT_AUTO_DEVICE_ID == 0
	;; from EEPROM
	movlw	(EEPROM_READONLY & 0xff) + 1
	movwf	EEADR
	call	MIOS_EEPROM_Read
#else
	;; from MIOS
	call	MIOS_MIDI_DeviceIDGet
#endif
	andlw	0x07
	movff	WREG, MIDIO_DEVICE_ID

	;; initialize application specific variables
	clrf	MB_STAT

	;; load values from internal dump
	call	MIDIO_DUMP_RestoreAll

	;; init default LCD offsets
	call	MIDIO_LCD_SetDefaultOffsets

#if DEFAULT_AIN_ENABLED > 0
	;; optional:
        ;; initialize the AIN driver
        movlw   DEFAULT_AIN_ENABLED
        call    MIOS_AIN_NumberSet
#if DEFAULT_AIN_ENABLED > 8
        call    MIOS_AIN_Muxed        ; use multiplexer interface
#else
        call    MIOS_AIN_UnMuxed        ; don't use multiplexer interface
#endif
        movlw   0x07                    ; set deadband to 7
        call    MIOS_AIN_DeadbandSet
#endif

	;; ------------------------------------------------------------------
	return


;; --------------------------------------------------------------------------
;;  This function is called by MIOS in the mainloop when nothing else is to do
;; --------------------------------------------------------------------------
USER_Tick
	return


;; --------------------------------------------------------------------------
;;  This function is periodically called by MIOS. The frequency has to be
;;  initialized with MIOS_Timer_Set
;;  Note that this is an interrupt service routine! Use FSR2 instead of FSR0
;;  and IRQ_TMPx instead of TMPx -- and make the routine as fast as possible!
;; --------------------------------------------------------------------------
USER_Timer
	return


;; --------------------------------------------------------------------------
;;  This function is called by MIOS when a debug command has been received
;;  via SysEx
;;  Input:
;;     o WREG, MIOS_PARAMETER1, MIOS_PARAMETER2, MIOS_PARAMETER3 like
;;       specified in the debug command
;;  Output:
;;     o return values WREG, MIOS_PARAMETER1, MIOS_PARAMETER2, MIOS_PARAMETER3
;; --------------------------------------------------------------------------
USER_MPROC_DebugTrigger
	return


;; --------------------------------------------------------------------------
;;  This function is called by MIOS when the display content should be 
;;  initialized. Thats the case during startup and after a temporary message
;;  has been printed on the screen
;; --------------------------------------------------------------------------
USER_DISPLAY_Init
	;; clear screen
	call	MIOS_LCD_Clear
	;; request display update
	bsf	MB_STAT, MB_STAT_DISPLAY_UPDATE_REQ
	return

;; --------------------------------------------------------------------------
;;  This function is called in the mainloop when no temporary message is shown
;;  on screen. Print the realtime messages here
;; --------------------------------------------------------------------------
USER_DISPLAY_Tick
	;; call the MIDIO display handler (features a minimal MIDI monitor)
	goto	MIDIO_DISPLAY_Handler


;; --------------------------------------------------------------------------
;;  This function is called by MIOS when a complete MIDI event has been received
;;  Input:
;;     o first  MIDI event byte in MIOS_PARAMETER1
;;     o second MIDI event byte in MIOS_PARAMETER2
;;     o third  MIDI event byte in MIOS_PARAMETER3
;; --------------------------------------------------------------------------
USER_MPROC_NotifyReceivedEvent
	;; continue at MIDIO MIDI handler
	goto	MIDIO_MIDI_NotifyReceivedEvent


;; --------------------------------------------------------------------------
;;  This function is called by MIOS when a MIDI event has been received
;;  which has been specified in the CONFIG_MIDI_IN table
;;  Input:
;;     o number of entry in WREG
;;     o first  MIDI event byte in MIOS_PARAMETER1
;;     o second MIDI event byte in MIOS_PARAMETER2
;;     o third  MIDI event byte in MIOS_PARAMETER3
;; --------------------------------------------------------------------------
USER_MPROC_NotifyFoundEvent
	return


;; --------------------------------------------------------------------------
;;  This function is called by MIOS when a MIDI event has not been completly
;;  received within 2 seconds
;; --------------------------------------------------------------------------
USER_MPROC_TO_STR_0	STRING 16, 0x00, " MIDI Time Out! "
USER_MPROC_TO_STR_1	STRING 16, 0x40, "  Whats up ???  "
USER_MPROC_NotifyTimeout
	;; print timeout message
	TABLE_ADDR USER_MPROC_TO_STR_0
	call	MIOS_LCD_PrintMessage
	call	MIOS_LCD_PrintMessage
	;; request display update
	bsf	MB_STAT, MB_STAT_DISPLAY_UPDATE_REQ
	
	;; -> jump to "ActionInvalid" for a proper reset of the sysex parser
	goto	MIDIO_SYSEX_ActionInvalid


;; --------------------------------------------------------------------------
;;  This function is called by MIOS when a MIDI byte has been received
;;  Input:
;;     o received MIDI byte in WREG and MIOS_PARAMETER1
;; --------------------------------------------------------------------------
USER_MPROC_NotifyReceivedByte
	;; -> continue at MIDIO sysex parser
	goto	MIDIO_SYSEX_Parser

;; --------------------------------------------------------------------------
;;  This function is called by MIOS before the transfer of a MIDI byte. 
;;  It can be used to monitor the Tx activity or to do any other actions
;;  (e.g. to switch a pin for multiplexed MIDI Outs) before the byte will 
;;  be sent.
;;  Note that this is an interrupt service routine! Use FSR2 instead of FSR0
;;  and IRQ_TMPx instead of TMPx -- and make the routine as fast as possible!
;;  Input:
;;     o transmitted byte in WREG
;; --------------------------------------------------------------------------
USER_MIDI_NotifyTx
	;; branch to Rx/Tx handler
	goto	MIDI_RXTX_NotifyTx

;; --------------------------------------------------------------------------
;;  This function is called by MIOS when a MIDI byte has been received.
;;  It can be used to monitor the Rx activity or to do any action - e.g.
;;  to react on realtime events like MIDI clock (0xf8) with a minimum latency
;;  Note that this is an interrupt service routine! Use FSR2 instead of FSR0
;;  and IRQ_TMPx instead of TMPx -- and make the routine as fast as possible!
;;  Input:
;;     o received byte in WREG
;; --------------------------------------------------------------------------
USER_MIDI_NotifyRx
	;; branch to Rx/Tx handler
	goto	MIDI_RXTX_NotifyRx

;; --------------------------------------------------------------------------
;;  This function is called by MIOS when an button has been toggled
;;  Input:
;;     o Button number in WREG and MIOS_PARAMETER1
;;     o Button value MIOS_PARAMETER2:
;;       - 1 if button has been released (=5V)
;;       - 0 if button has been pressed (=0V)
;; --------------------------------------------------------------------------
USER_DIN_NotifyToggle
	;; continue at MIDIO button handler
	goto	MIDIO_DIN_Handler


;; --------------------------------------------------------------------------
;;  This function is called by MIOS when an encoder has been moved
;;  Input:
;;     o Encoder number in WREG and MIOS_PARAMETER1
;;     o signed incrementer value in MIOS_PARAMETER2:
;;       - is positive when encoder has been turned clockwise
;;       - is negative when encoder has been turned counter clockwise
;; --------------------------------------------------------------------------
USER_ENC_NotifyChange
	;; do nothing (yet! ;-)
	return


;; --------------------------------------------------------------------------
;;  This function is called by MIOS before the shift register are loaded
;;  Note that this is an interrupt service routine! Use FSR2 instead of FSR0
;;  and IRQ_TMPx instead of TMPx -- and make the routine as fast as possible
;; --------------------------------------------------------------------------
USER_SR_Service_Prepare
	;; branch to the Rx/Tx LED handler
	goto	MIDI_RXTX_Handler


;; --------------------------------------------------------------------------
;;  This function is called by MIOS after the shift register have been loaded
;;  Note that this is an interrupt service routine! Use FSR2 instead of FSR0
;;  and IRQ_TMPx instead of TMPx -- and make the routine as fast as possible
;; --------------------------------------------------------------------------
USER_SR_Service_Finish
	;; fetch DIN registers if requested
	BRA_IFCLR MB_STAT, MB_STAT_FETCH_DIN_REQ, ACCESS, USER_SR_Service_Finish_End
	bcf	MB_STAT, MB_STAT_FETCH_DIN_REQ
	call	MIDIO_DOUT_FetchDIN
USER_SR_Service_Finish_End
	return

;; --------------------------------------------------------------------------
;;  This function is called by MIOS when a pot has been moved
;;  Input:
;;     o Pot number in WREG and MIOS_PARAMETER1
;;     o LSB value in MIOS_PARAMETER2
;;     o MSB value in MIOS_PARAMETER3
;; --------------------------------------------------------------------------
USER_AIN_NotifyChange
#if DEFAULT_AIN_ENABLED > 0
        ;; store pot number in PRODL
        movwf   PRODL

        ;; copy 7bit value into MIOS_PARAMETER2 (pot number already in WREG)
        call    MIOS_AIN_Pin7bitGet
        movwf   MIOS_PARAMETER2

        ;; restore pot number
	movff	PRODL, MIOS_PARAMETER1

        ;; now: pot number in WREG and MIOS_PARAMETER1
        ;; 7-bit value in MIOS_PARAMETER2

	movlw	0xb0			; CC channel #1
	call	MIOS_MIDI_TxBufferPut
	movf	MIOS_PARAMETER1, W
	addlw	16			; CC#16 + pot number
	call	MIOS_MIDI_TxBufferPut
	movf	MIOS_PARAMETER2, W	; CC value
	call	MIOS_MIDI_TxBufferPut
#endif
	
	return

	
;; ==========================================================================
;;  Application code (see comments in files)
;; ==========================================================================
	
;; ---[ reusable utility functions ]---
#include "midi_evnt.inc"
#include "midi_rxtx.inc"

;; ---[ MIDIO128 kernel ]---
#include "midio_bank.inc"
#include "midio_sysex.inc"
#include "midio_din.inc"
#include "midio_dout.inc"
#include "midio_display.inc"
#include "midio_lcd.inc"
#include "midio_fsr.inc"
#include "midio_addr.inc"
#include "midio_dump.inc"
#include "midio_midi.inc"
#include "midio_meta.inc"

;; ---[ midio presets stored in Flash/EEPROM ]---
#include "midio_presets.inc"

	END
