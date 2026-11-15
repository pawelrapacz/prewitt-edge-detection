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
    cmp r9, 0       # check output nullptr
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
    jnb ret_on_fail

    # check 1 < width - 1
    sub r10, 1 # width - 1 in r10
    cmp r10, 1
    jbe ret_on_fail

    # save stack
    push rbx
    push r12
    push r13
    push r14

    mov r13, qword ptr [rdi + PARAM_IN_BPL]

    cvtsd2ss xmm2, qword ptr [rdi + PARAM_AMOUNT]

    # calculate starting address input
    # start row * bytesPerLine
    mov rax, r11
    mul r13
    lea r8, [r8 + rax]  # in address in r8

    # calculate starting address output
    # start row * bytesPerLine
    mov rax, r11
    mul qword ptr [rdi + PARAM_OUT_BPL]
    lea r9, [r9 + rax]  # out address in r8

heightloop:
    mov rsi, 1  # start inside loop

    widthloop:
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
        movzx ebx, byte ptr [rax - 1]
        
        # top center
        movzx r12d, byte ptr [rax]

        # top right
        movzx r14d, byte ptr [rax + 1]
        mov ecx, ebx
        add ebx, r12d
        sub ecx, r14d
        add ebx, r14d

        # center left
        movzx eax, byte ptr [rdx - 1]

        # center right
        movzx r12d, byte ptr [rdx + 1]
        add ecx, eax
        sub ecx, r12d

        # bottom left
        movzx r14d, byte ptr [rdx + r13 - 1]
        sub ebx, r14d
        add ecx, r14d

        # bottom center
        movzx r12d, byte ptr [rdx + r13]
        sub ebx, r12d

        # bottom right
        movzx r14d,  byte ptr [rdx + r13 + 1]
        sub ebx, r14d
        sub ecx, r14d

        # calculate sqrt(gx * gx + gy * gy)
        # convert to float
        cvtsi2ss xmm0, ebx
        cvtsi2ss xmm1, ecx
        # square numbers
        mulss xmm0, xmm0
        mulss xmm1, xmm1

        addss xmm0, xmm1

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

        # loop increment and condition check
        inc rsi
        cmp rsi, r10
        jb widthloop

    # increment output and input pointers to the next row
    lea r8, [r8 + r13]
    mov r12, qword ptr [rdi + PARAM_OUT_BPL]
    lea r9, [r9 + r12]

    # loop increment and condition check
    inc r11
    cmp r11, qword ptr [rdi + PARAM_END]
    jb heightloop


ret_and_restore_stack:
    # restore stack and return 0
    pop r14
    pop r13
    pop r12
    pop rbx
    xor eax, eax
    ret

ret_on_fail:
    # return fail (-1)
    mov eax, -1
    ret

.size asmDetectEdgesWorker, .-asmDetectEdgesWorker
