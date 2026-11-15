.file "asmDetectEdgesSIMDWorker.s"
.intel_syntax noprefix

.include "detect_edges_params_offsets.inc"

.text
.global asmDetectEdgesSIMDWorker
.type asmDetectEdgesSIMDWorker, @function

# int asmDetectEdgesSIMDWorker(detect_edges_params* params);
# ptr in rdi
asmDetectEdgesSIMDWorker:
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

    # check 1 < width - 1
    sub r10, 1 # width - 1 in r10
    cmp r10, 1
    jbe ret_no_cleanup

    # width - 4 in r14
    mov r14, r10
    sub r14, 3

    # save stack
    push rbx
    push r12
    push r13
    push r14

    mov r13, qword ptr [rdi + PARAM_IN_BPL]

    cvtsd2ss xmm9, qword ptr [rdi + PARAM_AMOUNT]
    shufps xmm9, xmm9, 0


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
    mov rsi, 1  # start inside loop

    widthloop:
        cmp rsi, r14    # rsi < width - 4
        jae leftover

        # break false dependency
        xorps xmm0, xmm0
        xorps xmm1, xmm1

        # calculate current pixel index
        lea rdx, [r8 + rsi]

        # calculate index - bytesPerLine (upper row)
        mov rax, rdx
        sub rax, r13

        # load 4 bytes into 4 dwords
        # top left
        pmovzxbd xmm0, dword ptr [rax - 1]
        # top center
        pmovzxbd xmm1, dword ptr [rax]
        # top right
        pmovzxbd xmm2, dword ptr [rax + 1]
        # center left
        pmovzxbd xmm3, dword ptr [rdx - 1]
        # center right
        pmovzxbd xmm4, dword ptr [rdx + 1]
        # bottom left
        pmovzxbd xmm5, dword ptr [rdx + r13 - 1]
        # bottom center
        pmovzxbd xmm6, dword ptr [rdx + r13]
        # bottom right
        pmovzxbd xmm7, dword ptr [rdx + r13 + 1]

        movdqa xmm8, xmm0
        paddd xmm8, xmm1
        paddd xmm8, xmm2
        psubd xmm8, xmm5
        psubd xmm8, xmm6
        psubd xmm8, xmm7

        pmulld xmm8, xmm8

        paddd xmm0, xmm3
        paddd xmm0, xmm5
        psubd xmm0, xmm2
        psubd xmm0, xmm4
        psubd xmm0, xmm7

        pmulld xmm0, xmm0

        paddd xmm0, xmm8

        cvtdq2ps xmm0, xmm0

        sqrtps xmm0, xmm0
        mulps xmm0, xmm9
        cvttps2dq xmm0, xmm0

        packssdw xmm0, xmm0
        packuswb xmm0, xmm0

        movd dword ptr [r9 + rsi], xmm0

        # widthloop increment and condition check
        add rsi, 4
        jmp widthloop

    leftover:
        cmp rsi, r10
        jae leftover_done

        # break false dependency
        xorps xmm0, xmm0
        xorps xmm1, xmm1

        # calculate current pixel index
        lea rdx, [r8 + rsi]

        # calculate index - bytesPerLine (upper row)
        mov rax, rdx
        sub rax, r13

        # perform read and addition/substraction
        # top left
        movzx ecx, byte ptr [rax - 1]
        
        # top center
        movzx ebx, byte ptr [rax]

        # top right
        movzx r12d, byte ptr [rax + 1]

        add ebx, ecx
        sub ecx, r12d
        add ebx, r12d

        # center left
        movzx eax, byte ptr [rdx - 1]

        # center right
        movzx r12d, byte ptr [rdx + 1]

        add ecx, eax
        sub ecx, r12d

        # bottom left
        movzx eax, byte ptr [rdx + r13 - 1]

        # bottom center
        movzx r12d, byte ptr [rdx + r13]

        sub ebx, eax
        add ecx, eax
        sub ebx, r12d

        # bottom right
        movzx eax,  byte ptr [rdx + r13 + 1]
        
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
        mulss xmm0, xmm9

        # convert to int
        cvttss2si edx, xmm0

        # check if the value is higher than 255
        mov eax, 255
        cmp edx, eax

        cmovg edx, eax  # if higher than 255 change to 255
        mov byte ptr [r9 + rsi], dl # write result

        inc rsi
        jmp leftover

    leftover_done:

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
    pop r14
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

.size asmDetectEdgesSIMDWorker, .-asmDetectEdgesSIMDWorker
