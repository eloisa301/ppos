#include <stdio.h>
#include <stdlib.h>

int global = 15;


void ptr_func (void **p1, void *p2) {
   printf("\nF: PTR1 = %x [%x]", p1, *(int*)p1);
   printf("\nF: PTR2 = %x [%x]", p2, *(int*)p2);
   printf("\nF: PTR1 [%x] == PTR2 [%x] [%x]", p1, p2, (p1 == p2)); 
   printf("\nF: *PTR1 [%x] == PTR2 [%x] [%x]", *p1, p2, (*p1 == p2)); 
   if (*p1 == p2)
        printf("\nSAO IGUAIS!!!! [%x == %x] [%x == %x]", *p1, p2, *(int*)*p1, *(int*)p2);
   printf("\n");
}

int main (int argc, char *argv[])
{
   void *ptr1, *ptr2, *ptr3;

   ptr1 = &global;
   ptr2 = &global;
   ptr3 = &ptr2;

   printf("\nPTR1 = %x [%x]", ptr1, *(int*)ptr1);
   printf("\nPTR2 = %x [%x]", ptr2, *(int*)ptr2);
   printf("\nPTR3 = %x [%x]", ptr3, *(int*)ptr3);
   printf("\n");

   printf("\nPTR1 [%x] == PTR2 [%x] [%x]", ptr1, ptr2, (ptr1 == ptr2));
   if (ptr1 == ptr2)
        printf("\nSAO IGUAIS!!!!");
   printf("\n");
   
   ptr_func(&ptr1, ptr2);

   exit (0);
}