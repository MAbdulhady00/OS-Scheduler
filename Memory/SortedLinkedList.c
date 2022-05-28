#include "SortedLinkedList.h"
#include <stdlib.h>

SortedLinkedList *CreateSortedLinkedList()
{
    SortedLinkedList *Q = malloc(sizeof(SortedLinkedList));
    Q->head = NULL;
    return Q;
}
Sorted_Node *newSortedNode(int start_pos)
{
    Sorted_Node *temp = malloc(sizeof(Sorted_Node));
    temp->start_position = start_pos;
    temp->Node_PTR_Next = NULL;
}

void print(SortedLinkedList* Memory_List )
{
    Sorted_Node *iterator = Memory_List->head;
    while (iterator)
    {
        printf("Position: %d \n",iterator->start_position);
        iterator= iterator->Node_PTR_Next;
    }
}
void insert_sorted(SortedLinkedList* Memory_List ,int start_pos)
{
    Sorted_Node *tmp = newSortedNode(start_pos);
    if (!Memory_List->head)
    {   
        Memory_List->head = tmp;
        return;
    }
    Sorted_Node* iterator = Memory_List->head;
    Sorted_Node* iterator_previous = NULL;
    while(iterator && iterator->start_position < start_pos)
    {
        iterator_previous = iterator;
        iterator = iterator->Node_PTR_Next;
    }
    if(iterator)
    {
        if(iterator_previous)
        {
            iterator_previous->Node_PTR_Next = tmp;
            tmp->Node_PTR_Next = iterator;
        }
        else
        {
            Memory_List->head = tmp;
            tmp->Node_PTR_Next= iterator;
        }
        
    }
    else
    {
     iterator_previous->Node_PTR_Next = tmp;   
    }
}

int peekFront(SortedLinkedList* Memory_List ) {
    int start_pos = -1;
    if(Memory_List->head != NULL) {
        start_pos = Memory_List->head->start_position;
    }

    return start_pos;
}

int get_first_position (SortedLinkedList* Memory_List )
{
    int start_pos = -1;
    if (Memory_List->head)
    {   
        Sorted_Node* deleted_node = Memory_List->head;
        Memory_List->head = Memory_List->head->Node_PTR_Next;
        start_pos = deleted_node->start_position;
        free(deleted_node);   
    }
    return start_pos;
}
int delete(SortedLinkedList* Memory_List ,int start_pos)
{
    if (!Memory_List->head)
    {   
        return 0;
    }
    else if(Memory_List->head->start_position == start_pos)
    {
        Sorted_Node* deleted_node = Memory_List->head;
        Memory_List->head = Memory_List->head->Node_PTR_Next;
        free(deleted_node);
        return 1;
    }
    Sorted_Node* iterator = Memory_List->head;
    Sorted_Node* iterator_previous = NULL;
    while(iterator && iterator->start_position != start_pos)
    {
        iterator_previous = iterator;
        iterator = iterator->Node_PTR_Next;
    }
    if(iterator)
    {
        Sorted_Node* deleted_node = iterator;
        iterator_previous->Node_PTR_Next = iterator->Node_PTR_Next;
        free(deleted_node);
        return 1;
    }
    return 0;
}
void DestroySortedLinkedList(SortedLinkedList *Q)
{
    while (Q->head)
    {
        Sorted_Node *tmp = Q->head;
        Q->head = Q->head->Node_PTR_Next;
        //free(tmp->val);
        free(tmp);
    }
}
