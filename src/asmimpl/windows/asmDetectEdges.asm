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

; uint8_t* asmDetectEdges(size_t width, size_t height, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine);
asmDetectEdges proc frame
    sub rsp, 48h                ; 20h shadow + 18h na 3 + 10h
    .allocstack 48h
    .endprolog

    ; rcx, rdx, r8, r9 leave as is

    mov rax, [rsp + 48h + 28h]  ; amount
    mov [rsp + 20h], rax
    mov rax, [rsp + 48h + 30h]  ; outBytesPerLine
    mov [rsp + 28h], rax
    lea rax, asmDetectEdgesWorker
    mov [rsp + 30h], rax        ; worker

    call detectEdges

    add rsp, 48h
    ret
asmDetectEdges endp

end
