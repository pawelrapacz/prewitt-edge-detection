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
    jz ret_on_fail      # check if size == 0

    push rbx
    push r12
    push r13
    push r14
    push r15

    mov r12, rdi        # width in r12
    mov r13, rsi        # height in r13
    mov rbx, r8        # input in rbx

    mov rdi, rax        # set size argument for malloc
    call malloc@PLT

    test rax, rax       # check malloc fail
    jz ret_and_restore_stack

    mov rcx, rax        # output in rcx

    # setup bounds for the loops
    mov rdi, r12; dec rdi
    mov rsi, r13; dec rsi

    mov r8, 1
heightloop:
    mov r9, 1

    mov rax, r12
    mul r8
    widthloop:
        # calculate current pixel index
        mov r10, rax
        add r10, r9     # pixel index in r10
        
        # vertical
        xor r15, r15

        # upper row
        mov r14, r10
        sub r14, r12    # offset back to upper row
        dec r14         # top left px index in r14

        movzx r11, byte ptr [rbx + r14]
        add r15, r11

        inc r14
        movzx r11, byte ptr [rbx + r14]
        add r15, r11

        inc r14
        movzx r11, byte ptr [rbx + r14]
        add r15, r11

        # lower row
        mov r14, r10
        add r14, r12    # offset back to upper row
        dec r14         # top left px index in r14

        movzx r11, byte ptr [rbx + r14]
        sub r15, r11

        inc r14
        movzx r11, byte ptr [rbx + r14]
        sub r15, r11

        inc r14
        movzx r11, byte ptr [rbx + r14]
        sub r15, r11

        cvtsi2sd xmm0, r15
        mulsd xmm0, xmm0

        # horizontal
        xor r15, r15
        # left column
        mov r14, r10
        sub r14, r12
        dec r14

        movzx r11, byte ptr [rbx + r14]
        add r15, r11

        add r14, r12
        movzx r11, byte ptr [rbx + r14]
        add r15, r11

        add r14, r12
        movzx r11, byte ptr [rbx + r14]
        add r15, r11

        # right column
        mov r14, r10
        sub r14, r12
        inc r14

        movzx r11, byte ptr [rbx + r14]
        sub r15, r11

        add r14, r12
        movzx r11, byte ptr [rbx + r14]
        sub r15, r11

        add r14, r12
        movzx r11, byte ptr [rbx + r14]
        sub r15, r11


        cvtsi2sd xmm1, r15
        mulsd xmm1, xmm1

        addsd xmm0, xmm1

        sqrtsd xmm0, xmm0

        cvtsd2si r11, xmm0

        cmp r11, 255
        jb no_squash

        mov r11b, 255        
    
        no_squash:
            mov byte ptr [rcx + r10], r11b 

        inc r9
        cmp r9, rdi
        jb widthloop

    inc r8
    cmp r8, rsi
    jb heightloop

ret_and_restore_stack:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    mov rax, rcx        # output to rax
    ret

ret_on_fail:
    xor rax, rax
    ret

.size asmDetectEdges, .-asmDetectEdges
