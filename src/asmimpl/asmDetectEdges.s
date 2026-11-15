.file "asmDetectEdges.s"
.intel_syntax noprefix

.global asmDetectEdges
.type asmDetectEdges, @function

# uint8_t* asmDetectEdges(size_t width, size_t height, const uint8_t* input, size_t bytesPerLine, double amount, size_t* outBytesPerLine);
# width in rdi, height in rsi, input in rdx, bytesPerLine in rcx, amount in xmm0, outBytesPerLine in r8
asmDetectEdges:
    mov r9, qword ptr asmDetectEdgesWorker@GOTPCREL[rip]
    jmp detectEdges@PLT

.size asmDetectEdges, .-asmDetectEdges
