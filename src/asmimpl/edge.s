.file "edge.s"
.intel_syntax noprefix

.global asmDetectEdges
.type asmDetectEdges, @function

# uint8_t* asmDetectEdges(uint32_t width, uint32_t height, const uint8_t* input);
# width in edi, height in esi, input in rdx
asmDetectEdges:
    test rdx, rdx       # check if input not nullptr
    jz ret_on_fail

    mov r8, rdx         # input in r8
    mov eax, edi        # calculate size = width*height
    mul rsi             # size in rax
    test rax, rax
    jz ret_on_fail      # check if size == 0

    push rbx
    push r12
    push r13
    push r14

    mov r13, rdi        # width in r13
    mov r14, rsi        # height in r14
    mov rbx, r8        # input in rbx

    mov rdi, rax        # set size argument for malloc
    call malloc@PLT

    test rax, rax       # check malloc fail
    jz ret_and_restore_stack

    mov rcx, rax        # output in rcx

    # setup bounds for the loops
    mov rdi, r13; dec rdi
    mov rsi, r14; dec rsi

    mov r14, r13    # current row
    mov r8, 1
heightloop:
    mov r9, 1

    widthloop:
        # available r10, r11, r12, rax, rdx
        # calculate current pixel index
        mov rdx, r14    # current row
        add rdx, r9     # current pixel index in rdx

        xor r10, r10    # Gy
        xor r11, r11    # Gx

        # ####################
        # top row
        mov rax, rdx
        sub rax, r13
        dec rax

        # left
        movzx r12, byte ptr [rbx + rax]
        add r10, r12
        add r11, r12

        # center
        inc rax
        movzx r12, byte ptr [rbx + rax]
        add r10, r12

        # right
        inc rax
        movzx r12, byte ptr [rbx + rax]
        add r10, r12
        sub r11, r12

        # ####################
        # center row
        mov rax, rdx
        dec rax

        # left
        movzx r12, byte ptr [rbx + rax]
        add r11, r12

        # right
        add rax, 2
        movzx r12, byte ptr [rbx + rax]
        sub r11, r12

        # ####################
        # bottom row
        mov rax, rdx
        add rax, r13
        dec rax

        # left
        movzx r12, byte ptr [rbx + rax]
        sub r10, r12
        add r11, r12

        # center
        inc rax
        movzx r12, byte ptr [rbx + rax]
        sub r10, r12

        # right
        inc rax
        movzx r12,  byte ptr [rbx + rax]
        sub r10, r12
        sub r11, r12
        
        cvtsi2sd xmm0, r10
        mulsd xmm0, xmm0

        cvtsi2sd xmm1, r11
        mulsd xmm1, xmm1

        addsd xmm0, xmm1

        sqrtsd xmm0, xmm0

        cvtsd2si r11, xmm0

        cmp r11, 255
        jb no_squash

        mov r11b, 255        
    
        no_squash:
            mov byte ptr [rcx + rdx], r11b 

        inc r9
        cmp r9, rdi
        jb widthloop

    add r14, r13
    inc r8
    cmp r8, rsi
    jb heightloop

    mov rax, rcx        # output to rax
ret_and_restore_stack:
    pop r14
    pop r13
    pop r12
    pop rbx
    ret

ret_on_fail:
    xor rax, rax
    ret

.size asmDetectEdges, .-asmDetectEdges
