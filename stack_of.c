#include <stdio.h>
#include <stdlib.h>

/* Assuming page size is 4KB */
#define PAGE_SIZE (1LU << 12)
#define PAGE_MASK (~(PAGE_SIZE - 1))

int main () 
{
        int a;
        int b;
        /* Identify the page boundary */
        int *ptr = (int *)((unsigned int)&a & PAGE_MASK);
        /* Access the previous frame which is beyond the scope of the current program */
        *(ptr - 1) = 10;
        for(;;);
        return 0;
}
