#include "Memory_Manager.h"

const int NO_LinkedLists = 8;
void MEM_init()
{
    
    MEM = malloc(sizeof(SortedLinkedList *) * NO_LinkedLists);
    for (int i = 0; i < NO_LinkedLists; i++)
    {
        MEM[i] = CreateSortedLinkedList();
    }
    insert_sorted(MEM[NO_LinkedLists - 1], 0);
    max_available = NO_LinkedLists - 1;
}
int get_index(int size)
{
    int index = log2(size);
    int smallest_index = log2(smallest_size);
    if (index < smallest_index)
    {
        index = smallest_index;
    }
    return index - smallest_index;
}
int round_mem_size(int process_size)
{
    int floored_size = floor(log2(process_size));
    int finalsize = pow(2, floored_size);
    if (process_size > finalsize)
    {
        finalsize = finalsize * 2;
    }
    return finalsize;
}
int getsize(int index)
{
    index = index + log2(smallest_size);
    return pow(2,index);
}
// int split(SortedLinkedList **MEM, int index, int index2)
// {
//     if (index2 >)
//         int address_index2 = get_first_position(MEM[index2]);
//     if (address_index2 == -1)
//     {
//         int address = split(MEM, index, index2++);
//         if (index == index2 - 1)
//         {
//         }
//         else
//         {
//         }
//     }
//     else
//     {
//         insert_sorted(MEM[index2 - 1], address_index2 + pow(2, index2 - 1));
//         return address_index2;
//     }
// }

int split(int index)
{
    int first = get_first_position(MEM[index]);
    if (first == -1)
    {
        if (NO_LinkedLists == index+1)
            return -1;

        first = split(index + 1);

        if (first == -1)
            return -1;
    }
    insert_sorted(MEM[index-1], first + getsize(index - 1));
    
    // if(index == max_available && peekFront(MEM[index]) == -1) {
    //         --max_available;
    //         printf("max avail changed: %d\n", max_available);
    // }

    return first;
}

int allocate_MEM(int process_size)
{
    int memsize = round_mem_size(process_size);
    int index = get_index(memsize);
    int address = get_first_position(MEM[index]);
    if (address == -1)
    {
        address = split(index + 1);
    }
    return address;
}

void setmaxavailable() {
    max_available = -1;
    for(int i = NO_LinkedLists - 1; i >= 0; --i) {
        if(peekFront(MEM[i]) != -1) {
            max_available = i;
            return;
        }
    }
    if(max_available == -1)
        printf("No available memory \n");
}

void deallocate_MEM(int start_pos,int process_size)
{
    int memsize = round_mem_size(process_size);
    int index = get_index(memsize);
    
    while(1)
    {
        int check = start_pos/memsize;
        if(check %2 ==0)
        {
            int result = delete(MEM[index],start_pos+memsize);
            if(result == 0)
            {
                insert_sorted(MEM[index],start_pos);
                if(index > max_available)
                    max_available = index;
                break;
            }
            index += 1;
            memsize*=2;  
        
        }
        else
        {
            int result = delete(MEM[index],start_pos-memsize);
            if(result == 0)
            {
                insert_sorted(MEM[index],start_pos);
                if(index > max_available)
                    max_available = index;
                break;
            }
            start_pos = start_pos-memsize;
            index += 1;
            memsize*=2; 
        }

    }
    
}

void print_freemem ()
{
    printf("Free Memory Start\n");
    for(int i =0;i<NO_LinkedLists;i++)
    {
        printf("Free Memory of Size: %d  \n",getsize(i));
        print(MEM[i]);
    }
    printf("Free Memory End\n");
}

void DestroyMEM()
{
    for(int i=0;i<NO_LinkedLists;i++)
    {
        DestroySortedLinkedList(MEM[i]);
        free(MEM[i]);
    }
}