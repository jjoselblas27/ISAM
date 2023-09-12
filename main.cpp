#include <iostream>
#include <unistd.h>

const long page_size = sysconf(_SC_PAGE_SIZE);

struct Var {
    int* var;

    Var() : var(new int[page_size / sizeof(int)]) {}
    ~Var() { delete[] var; }
};

int main() {
    Var variable;
    return 0;
}