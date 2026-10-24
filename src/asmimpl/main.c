#include "asmimpl.h"
#include <stdlib.h>


int main() {
    auto a =asmGrayScale(100, 100, nullptr);
    if (a)
        free(a);
    return 0;
}