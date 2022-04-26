#pragma once
#include "../process_data.h"
#define element_t process * // Change this to change dynamic array type used
// Provides dynamically sized array with no range checking
typedef struct
{
    int _capacity;
    element_t *data;
    int size;
} DynamicArray;

DynamicArray *CreateDynamicArray(int InitialCapacity);

void push_back(DynamicArray *arr, element_t element);

void pop_back(DynamicArray *arr);

void DestroyDynamicArray(DynamicArray *arr);