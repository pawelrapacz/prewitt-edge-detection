.file "asmDetectEdgesWorker.s"
.intel_syntax noprefix

.include "detect_edges_params_offsets.inc"

.global asmDetectEdgesWorker
.type asmDetectEdgesWorker, @function

# uint8_t* asmDetectEdgesWorker(detect_edges_params* params);
# ptr in rdi
asmDetectEdgesWorker:
    mov r8, qword ptr [rdi + PARAM_INPUT]
    test r8, r8       # check if input not nullptr
    jz ret_on_fail

    # calculate size = width*height
    mov r10, qword ptr [rdi + PARAM_WIDTH]
    mov rax, qword ptr [rdi + PARAM_HEIGHT]
    mul r10             # size in rax
    test rax, rax
    jz ret_on_fail      # check if size == 0

    mov r9, qword ptr [rdi + PARAM_OUTPUT]    
    test r9, r9       # check output nullptr
    jz ret_on_fail

    push rbx
    push r12
    push r13
    push r14
    push r15


    # input in r8
    # output in r9

    movsd xmm2, qword ptr [rdi + PARAM_AMOUNT]

    dec r10    # width-1 in r10

    mov r11, qword ptr [rdi + PARAM_IN_BPL]
    mov r12, qword ptr [rdi + PARAM_OUT_BPL]

    mov r13, qword ptr [rdi + PARAM_START]
    mov r14, qword ptr [rdi + PARAM_END]
heightloop:
    mov rsi, 1

    widthloop:
        # available r10, r11, r12, rax, rdx
        # calculate current pixel index
        mov rax, r13
        mul r11
        add rax, rsi    # current in pixel index in rax
        mov rdx, rax

        xor rbx, rbx    # Gy
        xor rcx, rcx    # Gx

        # ####################
        # top row
        mov rax, rdx
        sub rax, r11


        # left
        movzx r15, byte ptr [r8 + rax - 1]
        add rbx, r15
        add rcx, r15

        # center
        movzx r15, byte ptr [r8 + rax]
        add rbx, r15

        # right
        movzx r15, byte ptr [r8 + rax + 1]
        add rbx, r15
        sub rcx, r15

        # ####################
        # center row

        # left
        movzx r15, byte ptr [r8 + rdx - 1]
        add rcx, r15

        # right
        add rax, 2
        movzx r15, byte ptr [r8 + rdx - 1]
        sub rcx, r15

        # ####################
        # bottom row
        mov rax, rdx
        add rax, r11

        # left
        movzx r15, byte ptr [r8 + rax - 1]
        sub rbx, r15
        add rcx, r15

        # center
        movzx r15, byte ptr [r8 + rax]
        sub rbx, r15

        # right
        movzx r15,  byte ptr [r8 + rax + 1]
        sub rbx, r15
        sub rcx, r15

        cvtsi2sd xmm0, rbx
        mulsd xmm0, xmm0

        cvtsi2sd xmm1, rcx
        mulsd xmm1, xmm1

        addsd xmm0, xmm1

        sqrtsd xmm0, xmm0
        mulsd xmm0, xmm2

        cvtsd2si r15, xmm0

        cmp r15, 255
        jb no_squash

        mov r15b, 255        
    
        no_squash:
            mov rax, r13
            mul r12
            add rax, rsi    # current out pixel index in rax
            mov byte ptr [r9 + rax], r15b
        
        inc rsi
        cmp rsi, r10
        jb widthloop

    inc r13
    cmp r13, r14
    jb heightloop


ret_and_restore_stack:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    xor rax, rax
    ret

ret_on_fail:
    mov rax, -1
    ret

.size asmDetectEdgesWorker, .-asmDetectEdgesWorker
