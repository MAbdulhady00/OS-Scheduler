#include <stdio.h>
#include "Memory_Manager.h"

int main()
{
    //SortedLinkedList* Q1 = CreateSortedLinkedList();
    // insert_sorted(Q1,0);
    // insert_sorted(Q1,1);
    // insert_sorted(Q1,4);
    // insert_sorted(Q1,2);
    // insert_sorted(Q1,3);
    // print(Q1);
    // printf("Delete\n");
    // printf("Delete result %d\n",delete(Q1,2));
    // printf("Delete result %d\n",delete(Q1,0));
    // printf("Delete result %d\n",delete(Q1,6));
    // print(Q1);

    // printf("get first\n");
    // printf("Element %d \n",get_first_position(Q1));
    // printf("Element %d \n",get_first_position(Q1));
    // printf("Element %d \n",get_first_position(Q1));
    // printf("Element %d \n",get_first_position(Q1));
    MEM_init();
    int * startarr = malloc(sizeof(int)*4);


    printf("allocate memory at position: %d \n",startarr[0]=allocate_MEM(9));
    printf("allocate memory at position: %d \n",startarr[1]=allocate_MEM(8));
    printf("allocate memory at position: %d \n",startarr[2]=allocate_MEM(32));
    printf("allocate memory at position: %d \n",startarr[3]=allocate_MEM(32));
    print_freemem();
    deallocate_MEM(startarr[0],9);
    printf("deallocate memory at position: %d \n",startarr[0]);
    print_freemem();
    deallocate_MEM(startarr[1],8);
    printf("deallocate memory at position: %d \n",startarr[1]);
    print_freemem();
    deallocate_MEM(startarr[2],32);
    printf("deallocate memory at position: %d \n",startarr[2]);
    print_freemem();
    deallocate_MEM(startarr[3],32);
    printf("deallocate memory at position: %d \n",startarr[3]);
    print_freemem();

    DestroyMEM();

    return 0;
}

