; $Id$
;
; Setup File for TK's MIDIbox SID
; Explanations to the switches and levers: see setup_midibox16e.asm
;
#define DEFAULT_NUMBER_ENCS	15

#define DEFAULT_ENCODER_GROUP_WIDTH 16

#define DEFAULT_NUMBER_BUTTONS  64

#define DEFAULT_SRIO_DEBOUNCE_CTR 0

#define DEFAULT_PATCHMODE 0
#define DEFAULT_BANK_PRG_CHNG_CHANNEL 16
#define DEFAULT_PATCH_PRG_CHNG_CHANNEL 15

#define DEFAULT_BANKSTICK_CS	7	; (selecting BankStick #7 so that the common SID BankStick won't be overwritten)

#define DEFAULT_DEVICE_ID	0x00
#define DEFAULT_AUTO_DEVICE_ID	1

#define DEFAULT_MIDI_MONITOR_ENABLED 0
#define DEFAULT_MIDI_RX_LED 0x00
#define DEFAULT_MIDI_TX_LED 0x01

#define LEDRINGS_SR_ENC1_16_CATHODES_1	0
#define LEDRINGS_SR_ENC1_16_CATHODES_2	0
#define LEDRINGS_SR_ENC1_16_ANODES_1	0
#define LEDRINGS_SR_ENC1_16_ANODES_2	0
	
#define LEDRINGS_SR_ENC17_32_CATHODES_1	0
#define LEDRINGS_SR_ENC17_32_CATHODES_2	0
#define LEDRINGS_SR_ENC17_32_ANODES_1	0
#define LEDRINGS_SR_ENC17_32_ANODES_2	0

#define LEDRINGS_SR_ENC33_48_CATHODES_1	0
#define LEDRINGS_SR_ENC33_48_CATHODES_2	0
#define LEDRINGS_SR_ENC33_48_ANODES_1	0
#define LEDRINGS_SR_ENC33_48_ANODES_2	0
;
#define LEDRINGS_SR_ENC49_64_CATHODES_1	0
#define LEDRINGS_SR_ENC49_64_CATHODES_2	0
#define LEDRINGS_SR_ENC49_64_ANODES_1	0
#define LEDRINGS_SR_ENC49_64_ANODES_2	0

#define DEFAULT_ENC_SPEED_NORMAL_MODE   MIOS_ENC_SPEED_NORMAL
#define DEFAULT_ENC_SPEED_NORMAL_DIV	0
#define DEFAULT_ENC_SPEED_SLOW_MODE     MIOS_ENC_SPEED_SLOW
#define DEFAULT_ENC_SPEED_SLOW_DIV	6
#define DEFAULT_ENC_SPEED_FAST_MODE     MIOS_ENC_SPEED_FAST
#define DEFAULT_ENC_SPEED_FAST_DIV	4

#define DEFAULT_DIN_SR_PIN_01_08	3
#define DEFAULT_DIN_SR_PIN_09_16	1
#define DEFAULT_DIN_SR_PIN_17_24	2
#define DEFAULT_DIN_SR_PIN_25_32	4
#define DEFAULT_DIN_SR_PIN_33_40	5
#define DEFAULT_DIN_SR_PIN_41_48	6
#define DEFAULT_DIN_SR_PIN_49_56	7
#define DEFAULT_DIN_SR_PIN_57_64	8

#define DEFAULT_ENC_DATAWHEEL   15	; encoder #15 used as datawheel
#define DEFAULT_DIN_MENU_EXEC	2
#define DEFAULT_DIN_MENU_RIGHT	3
#define DEFAULT_DIN_MENU_LEFT	4
#define DEFAULT_DIN_MENU_SNAPSHOT 5
;
#define DEFAULT_DOUT_SR_PIN_01_08	0
#define DEFAULT_DOUT_SR_PIN_09_16	0
#define DEFAULT_DOUT_SR_PIN_17_24	0
#define DEFAULT_DOUT_SR_PIN_25_32	0
#define DEFAULT_DOUT_SR_PIN_33_40	0
#define DEFAULT_DOUT_SR_PIN_41_48	0
#define DEFAULT_DOUT_SR_PIN_49_56	0
#define DEFAULT_DOUT_SR_PIN_57_64	0

#define DEFAULT_GP_DIN_SR0	2
#define DEFAULT_GP_DIN_SR1	3

#define DEFAULT_GP_DOUT_SR0	1
#define DEFAULT_GP_DOUT_SR1	2

#define DEFAULT_YOFFSET_LINE0	0x00 + 2
#define DEFAULT_YOFFSET_LINE1	0x40 + 2
#define DEFAULT_YOFFSET_LINE2	0x14 + 2
#define DEFAULT_YOFFSET_LINE3	0x54 + 2

#define DEFAULT_MORPH_FUNCTION_ENABLED 1

#define DEFAULT_NUMBER_AIN		0
#define DEFAULT_ENABLE_AIN_MUX		0
#define DEFAULT_ENABLE_MOTORDRIVER	0
#define DEFAULT_MF_PWM_DUTY_UP	        0x01
#define DEFAULT_MF_PWM_DUTY_DOWN        0x01
#define DEFAULT_MF_PWM_PERIOD           0x03
#define DEFAULT_TOUCH_SENSOR_SR		0
#define DEFAULT_TOUCH_SENSOR_MODE	0
#define	DEFAULT_TOUCH_SENSOR_SENSITIVITY 3


	org	0x3280		; never change the origin!
	;; encoder entry structure
ENC_ENTRY MACRO sr, din_0, mode
	dw	(mode << 8) | (din_0 + 8*(sr-1))
	ENDM	
ENC_EOT	MACRO
	dw	0xffff
	ENDM

MIOS_ENC_PIN_TABLE
	;; encoders 1-16
	;;        SR  Pin  Mode
	;; TK's MIDIbox SID Setup
	ENC_ENTRY  3,  0,  MIOS_ENC_MODE_NON_DETENTED	; Osc delay/transpose/assign #1
	ENC_ENTRY  3,  2,  MIOS_ENC_MODE_NON_DETENTED	; Osc attack/finetune/assign #2
	ENC_ENTRY  3,  4,  MIOS_ENC_MODE_NON_DETENTED	; Osc decay/portamento/assign #3
	ENC_ENTRY  3,  6,  MIOS_ENC_MODE_NON_DETENTED	; Osc sustain/release/assign #4
	ENC_ENTRY  3,  8,  MIOS_ENC_MODE_NON_DETENTED	; Osc release/pulsewidth/assign #5
	ENC_ENTRY  5,  4,  MIOS_ENC_MODE_NON_DETENTED	; Filter CutOff
	ENC_ENTRY  5,  6,  MIOS_ENC_MODE_NON_DETENTED	; Filter Resonance
	ENC_ENTRY  4,  6,  MIOS_ENC_MODE_NON_DETENTED	; LFO rate
	ENC_ENTRY  5,  0,  MIOS_ENC_MODE_NON_DETENTED	; LFO depth
	ENC_ENTRY  6,  0,  MIOS_ENC_MODE_NON_DETENTED	; Env depth/assign #1
	ENC_ENTRY  6,  2,  MIOS_ENC_MODE_NON_DETENTED	; Env attack/assign #2
	ENC_ENTRY  6,  4,  MIOS_ENC_MODE_NON_DETENTED	; Env decay/assign #3
	ENC_ENTRY  6,  6,  MIOS_ENC_MODE_NON_DETENTED	; Env sustain/assign #4
	ENC_ENTRY  7,  0,  MIOS_ENC_MODE_NON_DETENTED	; Env release/assign #5

	ENC_ENTRY  1,  0,  MIOS_ENC_MODE_DETENTED2	; menu encoder used as data wheel here

	;; encoders 17-32
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT

	;; encoders 33-48
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT

	;; encoders 49-64
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT
	ENC_EOT


#include "src/main.inc"
