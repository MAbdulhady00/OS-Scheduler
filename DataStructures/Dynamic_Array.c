#include <stdlib.h>
#include <string.h>
#include "Dynamic_Array.h"

DynamicArray *CreateDynamicArray(int InitialCapacity)
{
    if (InitialCapacity <= 1)
        InitialCapacity = 2;

    DynamicArray *arr = malloc(sizeof(DynamicArray));
    arr->_capacity = InitialCapacity;
    arr->size = 0;
    arr->data = malloc(sizeof(element_t) * InitialCapacity);
}

static void Extend(DynamicArray *arr)
{
    // Temporary pointer to hold new array with double capacity
    element_t *temp = malloc(2 * arr->_capacity * sizeof(element_t));
    // Copy previous array to new one
    memcpy(temp, arr->data, arr->_capacity * sizeof(element_t));
    // Free old array
    free(arr->data);
    // Swap old and new array
    arr->data = temp;
    arr->_capacity *= 2;
}

void push_back(DynamicArray *arr, element_t element)
{
    if (arr->size == arr->_capacity)
        Extend(arr);

    arr->data[(arr->size)++] = element;
}

void pop_back(DynamicArray *arr)
{
    arr->size--;
}

void DestroyDynamicArray(DynamicArray *arr)
{
    free(arr->data);
    free(arr);
}
