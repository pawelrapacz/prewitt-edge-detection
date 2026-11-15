/*
 * Project: Prewitt edge detection
 * 
 * The Prewitt algorithm detects edges in images by calculating
 * intensity changes in horizontal and vertical directions using
 * two convolution kernels.
 * 
 * 2026/2027 sem. 5
 * Author: Paweł Rapacz
 * 
 * Version: 1.0
 * 
 * Version history:
 * 1.0 - Initial version of the application.
 *      Implemented Prewitt edge detection in C and x86-64 ASM.
 *      Created GUI app and benchmark with core functionality.
 */

.equ PARAM_INPUT,           0
.equ PARAM_OUTPUT,          8
.equ PARAM_IN_BPL,         16
.equ PARAM_OUT_BPL,        24
.equ PARAM_AMOUNT,         32
.equ PARAM_WIDTH,          40
.equ PARAM_HEIGHT,         48
.equ PARAM_START,          56
.equ PARAM_END,            64
