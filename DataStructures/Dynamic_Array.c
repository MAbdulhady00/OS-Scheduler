#include <stdlib.h>
#include <string.h>

//Provides dynamically sized array with no range checking
struct DynamicArray
{
    int _capacity;
    int *data;
    int size;
};

struct DynamicArray *CreateDynamicArray(int InitialCapacity)
{
    if (InitialCapacity <= 1)
        InitialCapacity = 2;

    struct DynamicArray *arr = malloc(sizeof(struct DynamicArray));
    arr->_capacity = InitialCapacity;
    arr->size = 0;
    arr->data = malloc(sizeof(int) * InitialCapacity);
}

void _Extend(struct DynamicArray *arr)
{
    // Temporary pointer to hold new array with double capacity
    int *temp = malloc(2 * arr->_capacity * sizeof(int));
    // Copy previous array to new one
    memcpy(temp, arr->data, arr->_capacity * sizeof(int));
    // Free old array
    free(arr->data);
    // Swap old and new array
    arr->data = temp;
    arr->_capacity *= 2;
}

void push_back(struct DynamicArray *arr, int element)
{
    if (arr->size == arr->_capacity)
        _Extend(arr);

    arr->data[(arr->size)++] = element;
}

void pop_back(struct DynamicArray *arr)
{
    arr->size--;
}

void DestroyDynamicArray(struct DynamicArray *arr)
{
    free(arr->data);
    free(arr);
}
