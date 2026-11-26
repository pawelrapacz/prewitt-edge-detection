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

.file "asmDetectEdgesWorker.s"
.intel_syntax noprefix

.include "detect_edges_params_offsets.inc"

.text
.global asmDetectEdgesWorker
.type asmDetectEdgesWorker, @function

# int asmDetectEdgesWorker(detect_edges_params* params);
# ptr in rdi
asmDetectEdgesWorker:
    mov r8, qword ptr [rdi + PARAM_INPUT]
    cmp r8, 0       # check if input not nullptr
    je ret_on_fail

    mov r9, qword ptr [rdi + PARAM_OUTPUT]    
    cmp r9, 0       # check output not nullptr
    je ret_on_fail

    # calculate size = width*height
    mov r10, qword ptr [rdi + PARAM_WIDTH]
    mov rax, qword ptr [rdi + PARAM_HEIGHT]
    mul r10             # size in rax
    test rax, rax
    je ret_on_fail      # check if size == 0

    # input in r8
    # output in r9

    # check start < end
    mov r11, qword ptr [rdi + PARAM_START]  # start row
    cmp r11, qword ptr [rdi + PARAM_END]    # end row
    jnb ret_no_cleanup

    # save stack
    push rbx
    push r12
    push r13
    push r15

    mov r15, qword ptr [rdi + PARAM_FMT] # channels in r15
    
    # check channels < (width - 1) * channels
    mov rax, r10
    sub rax, 1 # width - 1 in r10
    mul r15
    cmp rax, r15
    jbe ret_and_restore_stack

    mov r10, rax

    mov r13, qword ptr [rdi + PARAM_IN_BPL]

    cvtsd2ss xmm2, qword ptr [rdi + PARAM_AMOUNT]

    # calculate starting address input
    # start row * inBytesPerLine
    mov rax, r11
    mul r13
    lea r8, [r8 + rax]  # in address in r8

    # calculate starting address output
    # start row * outBytesPerLine
    mov rax, r11
    mul qword ptr [rdi + PARAM_OUT_BPL]
    lea r9, [r9 + rax]  # out address in r8

heightloop:
    mov rsi, r15 # start from channel to omit the first pixel

    widthloop:
        # break false dependency
        xorps xmm0, xmm0
        xorps xmm1, xmm1

        # calculate current pixel index - channel
        lea rdx, [r8 + rsi]
        sub rdx, r15

        # calculate index - channel - bytesPerLine (upper row)
        mov rax, rdx
        sub rax, r13

        # perform read and addition/substraction
        # top left
        movzx ecx, byte ptr [rax]
        
        # top center
        movzx ebx, byte ptr [rax + r15]

        # top right
        movzx r12d, byte ptr [rax + r15 * 2]

        add ebx, ecx
        sub ecx, r12d
        add ebx, r12d

        # center left
        movzx eax, byte ptr [rdx]

        # center right
        movzx r12d, byte ptr [rdx + r15 * 2]

        add ecx, eax
        sub ecx, r12d

        # bottom left
        lea rdx, [rdx + r13]
        movzx eax, byte ptr [rdx]

        # bottom center
        movzx r12d, byte ptr [rdx + r15]

        sub ebx, eax
        add ecx, eax
        sub ebx, r12d

        # bottom right
        movzx eax,  byte ptr [rdx + r15 * 2]
        
        sub ebx, eax
        sub ecx, eax

        # calculate sqrt(gx * gx + gy * gy)
        imul ebx, ebx
        imul ecx, ecx
        add ebx, ecx
        # convert to float
        cvtsi2ss xmm0, ebx
        sqrtss xmm0, xmm0

        # multiply by amount
        mulss xmm0, xmm2

        # convert to int
        cvttss2si edx, xmm0

        # check if the value is higher than 255
        mov eax, 255
        cmp edx, eax

        cmovg edx, eax  # if higher than 255 change to 255
        mov byte ptr [r9 + rsi], dl # write result

        # widthloop increment and condition check
        inc rsi
        cmp rsi, r10    # rsi < (width - 1) * channels
        jb widthloop

    # increment output and input pointers to the next row
    lea r8, [r8 + r13]  # startRow + inBytesPerLine
    mov rax, qword ptr [rdi + PARAM_OUT_BPL]
    lea r9, [r9 + rax]  # startRow + outBytesPerLine

    # heightloop increment and condition check
    inc r11
    cmp r11, qword ptr [rdi + PARAM_END]    # startRow < endRow
    jb heightloop

ret_and_restore_stack:
    # restore stack and return 0
    pop r15
    pop r13
    pop r12
    pop rbx
ret_no_cleanup:
    xor eax, eax
    ret

ret_on_fail:
    # return fail (-1)
    mov eax, -1
    ret

.size asmDetectEdgesWorker, .-asmDetectEdgesWorker
