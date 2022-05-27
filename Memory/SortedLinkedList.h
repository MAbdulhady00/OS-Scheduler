#pragma once
#include <stdio.h>
struct Sorted_Node
{
    /* data */
    struct Sorted_Node* Node_PTR_Next;
    int start_position;
};
typedef struct Sorted_Node Sorted_Node;

typedef struct
{
    Sorted_Node *head;
} SortedLinkedList;


SortedLinkedList *CreateSortedLinkedList();

Sorted_Node *newNode(int start_pos);

void insert_sorted(SortedLinkedList* Memory_List ,int start_pos);
int get_first_position (SortedLinkedList* Memory_List );
int delete(SortedLinkedList* Memory_List ,int start_pos);
void print(SortedLinkedList* Memory_List );

void DestroySortedLinkedList(SortedLinkedList *Q);

