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

extern detectEdges:proc
extern asmDetectEdgesWorker:proc

.code

; uint8_t* asmDetectEdges(size_t width, size_t height, Format fmt, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine);
asmDetectEdges proc frame
    sub rsp, 48h                    ; 20h shadow + 4 slots to stack + 8 alignment
    .allocstack 48h
    .endprolog

    ; rcx, rdx, r8, r9 (args 1-4) no changes

    ; 5th arg: bytesPerLine
    mov rax, [rsp + 48h + 28h]
    mov [rsp + 20h], rax

    ; 6th arg: amount (double)
    mov rax, [rsp + 48h + 30h]
    mov [rsp + 28h], rax

    ; 7th arg: outBytesPerLine
    mov rax, [rsp + 48h + 38h]
    mov [rsp + 30h], rax

    ; 8th arg: worker ptr
    lea rax, asmDetectEdgesWorker
    mov [rsp + 38h], rax

    call detectEdges

    add rsp, 48h
    ret
asmDetectEdges endp

end
