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

    push rbx
    push r12
    push r13
    push r14
    push r15

    # input in r8
    # output in r9

    cvtsd2ss xmm2, qword ptr [rdi + PARAM_AMOUNT]

    dec r10

    mov r11, qword ptr [rdi + PARAM_IN_BPL]

    mov r13, qword ptr [rdi + PARAM_START]

    # calculate starting address in
    mov rax, r13
    mul r11
    lea r8, [r8 + rax]

    # calculate starting address out
    mov rax, r13
    mul qword ptr [rdi + PARAM_OUT_BPL]
    lea r9, [r9 + rax]

heightloop:
    mov rsi, 1

    widthloop:
        # calculate current pixel index
        lea rdx, [r8 + rsi]

        # top row
        mov rax, rdx
        sub rax, r11

        # top left
        movzx ebx, byte ptr [rax - 1]
        # top center
        movzx r15d, byte ptr [rax]
        # top right
        movzx r14d, byte ptr [rax + 1]
        # center left
        movzx ecx, byte ptr [rdx - 1]
        add ecx, ebx
        add ebx, r15d
        add ebx, r14d
        sub ecx, r14d
        # bottom right
        movzx eax,  byte ptr [rdx + r11 + 1]
        sub ebx, eax
        sub ecx, eax
        # bottom left
        movzx r15d, byte ptr [rdx + r11 - 1]
        # bottom center
        movzx r14d, byte ptr [rdx + r11]
        # center right
        movzx eax, byte ptr [rdx + 1]
        sub ebx, r14d
        sub ebx, r15d
        cvtsi2ss xmm0, ebx
        add ecx, r15d
        mulss xmm0, xmm0
        sub ecx, eax
        cvtsi2ss xmm1, ecx
        mulss xmm1, xmm1

        mov eax, 255
        addss xmm0, xmm1

        sqrtss xmm0, xmm0
        mulss xmm0, xmm2

        cvttss2si edx, xmm0

        cmp edx, eax
        cmovg edx, eax
        mov byte ptr [r9 + rsi], dl

        inc rsi
        cmp rsi, r10
        jb widthloop

    mov r11, qword ptr [rdi + PARAM_IN_BPL]
    lea r8, [r8 + r11]
    inc r13
    mov r12, qword ptr [rdi + PARAM_OUT_BPL]
    lea r9, [r9 + r12]

    cmp r13, qword ptr [rdi + PARAM_END]
    jb heightloop


ret_and_restore_stack:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    xor eax, eax
    ret

ret_on_fail:
    mov eax, -1
    ret

.size asmDetectEdgesWorker, .-asmDetectEdgesWorker
