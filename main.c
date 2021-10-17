#include <stdio.h>
#include "heap.h"

int main(void)
{
    char* alphabet = (char*)heap_alloc(27);
    
    for (int i = 0; i < 26; i++)
        alphabet[i] = i + 'a';
    alphabet[26] = 0;

    printf("%s\n", alphabet);

    heap_free((void*)alphabet);
    return 0;
}
