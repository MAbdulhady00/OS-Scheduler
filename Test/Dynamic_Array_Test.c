#include <stdio.h>
#include "../DataStructures/Dynamic_Array.c"

int main()
{

    struct DynamicArray *arr = CreateDynamicArray(0);
    for (int i = 0; i < 128; ++i)
    {
        push_back(arr, i);
    }

    for (int i = 0; i < 128; ++i)
    {
        if (arr->data[i] != i)
        {
            fprintf(stderr, "Error in dynamic array element at location %d, value: %d Expected: %d\n", i, arr->data[i], i);
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
    push_back(arr, 6);
    if (arr->data[223] != 6)
        {
            fprintf(stderr, "Error in dynamic array element at location %d, value: %d Expected: %d\n", 223, arr->data[223], 6);
            return -1;
        }
    
    for(int i = 0; i < 5; ++i)
        pop_back(arr);

    if (arr->size != 219)
        {
            fprintf(stderr, "Error in dynamic array size, value: %d Expected: %d\n", arr->size, 219);
            return -1;
        }

    if (arr->_capacity != 446)
        {
            fprintf(stderr, "Error in dynamic array size, value: %d Expected: %d\n", arr->_capacity, 446);
            return -1;
        }

    DestroyDynamicArray(arr);

    printf("Dynamic array test completed successfully!\n");
    return 0;
}