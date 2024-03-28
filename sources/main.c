#include <stdio.h>
#include "tests/tests.h"

int main()
{
    printf("tests_mark_range()\n");
    tests_mark_range();
    printf("tests_alloc_pages()\n");
    tests_alloc_pages();
    return 0;
}