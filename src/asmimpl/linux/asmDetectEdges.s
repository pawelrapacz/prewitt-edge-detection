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

.file "asmDetectEdges.s"
.intel_syntax noprefix

.global asmDetectEdges
.type asmDetectEdges, @function

# uint8_t* asmDetectEdges(size_t width, size_t height, Format fmt, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine);
# width in rdi, height in rsi, input in rdx, bytesPerLine in rcx, amount in xmm0, outBytesPerLine in r8
asmDetectEdges:
    sub rsp, 16
    push qword ptr asmDetectEdgesWorker@GOTPCREL[rip]
    call detectEdges@PLT
    add rsp, 24
    ret

.size asmDetectEdges, .-asmDetectEdges
