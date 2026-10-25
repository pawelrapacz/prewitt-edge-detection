.file "gray.s"
.intel_syntax noprefix
.text

.global asmGrayScale
.type asmGrayScale, @function

# uint8_t* cGrayScale(uint32_t width, uint32_t height, const rgb_t* input)
# width in edi, height in esi, input in rdx
asmGrayScale:
    test rdx, rdx
    jz exit_null_input

    push rbx
    mov rbx, rdx

    mov eax, edi
    mul rsi
    test rax, rax
    jz exit_size_0

    mov rdi, rax
    push rax
    call malloc@PLT

    test rax, rax
    jz exit_malloc_null

    mov r11, rax
    pop rdi

    xor rsi, rsi
grayloop:
    mov al, byte ptr [rbx + rsi]
    mov r9b, byte ptr [rbx + rsi + 1]
    mov r10b, byte ptr [rbx + rsi + 2]

    add al, r9b
    add al, r10b

    mov cl, 3
    xor ah, ah
    div cl

    mov byte ptr [r11 + rsi], al

    add rsi, 3
    cmp rsi, rdi
    jb grayloop


    pop rbx
    ret

exit_null_input:
    xor rax, rax
    ret

exit_malloc_null:
    pop rdi
exit_size_0:
    pop rbx
    ret

.size asmGrayScale, .-asmGrayScale