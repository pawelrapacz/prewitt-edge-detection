;
; Project: Prewitt edge detection
; 
; The Prewitt algorithm detects edges in images by calculating
; intensity changes in horizontal and vertical directions using
; two convolution kernels.
; 
; 2026/2027 sem. 5
; Author: Paweł Rapacz
; 
; Version: 1.0
; 
; Version history:
; 1.0 - Initial version of the application.
;      Implemented Prewitt edge detection in C and x86-64 ASM.
;      Created GUI app and benchmark with core functionality.
;

PARAM_INPUT         equ  0
PARAM_OUTPUT        equ  8
PARAM_IN_BPL        equ 16
PARAM_OUT_BPL       equ 24
PARAM_AMOUNT        equ 32
PARAM_WIDTH         equ 40
PARAM_HEIGHT        equ 48
PARAM_FMT           equ 56
PARAM_START         equ 64
PARAM_END           equ 72
