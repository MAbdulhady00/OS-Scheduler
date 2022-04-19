#include <stdio.h>
#include <stdlib.h>
#include "../DataStructures/Dynamic_Array.h"

int main()
{

    DynamicArray *arr = CreateDynamicArray(0);
    //Test case one check extension and pushback
    for (int i = 0; i < 128; ++i)
    {
        process *p = malloc(sizeof(process));
        p->pid = i;
        push_back(arr, p);
    }

    for (int i = 0; i < 128; ++i)
    {
        if (arr->data[i]->pid != i)
        {
            fprintf(stderr, "Error in dynamic array element at location %d, value: %d Expected: %d\n", i, arr->data[i]->pid, i);
            return -1;
        }
    }

    if (arr->_capacity != 128)
    {
        fprintf(stderr, "Error in dynamic array capacity, value: %d Expected: %d\n", arr->_capacity, 128);
        return -1;
    }

    DestroyDynamicArray(arr);

    //Test case 2: Check capacity after extension and pop_back
    arr = CreateDynamicArray(223);
    arr->size = 223;
    process p;
    p.pid = 6;
    push_back(arr, &p);
    if (arr->data[223]->pid != 6)
    {
        fprintf(stderr, "Error in dynamic array element at location %d, value: %d Expected: %d\n", 223, arr->data[223]->pid, 6);
        return -1;
    }

    for (int i = 0; i < 5; ++i)
        pop_back(arr);

    if (arr->size != 219)
    {
        fprintf(stderr, "Error in dynamic array size, value: %d Expected: %d\n", arr->size, 219);
        return -1;
    }

    if (arr->_capacity != 446)
    {
        fprintf(stderr, "Error in dynamic array capacity, value: %d Expected: %d\n", arr->_capacity, 446);
        return -1;
    }

    DestroyDynamicArray(arr);

    printf("Dynamic array test completed successfully!\n");
    return 0;
}