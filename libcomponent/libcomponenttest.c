#include <stdio.h>
#include "libcomponent.h"
void print_e_resistance_test(float orig_resistance);

float res[3];
int main()
{
   print_e_resistance_test(1398.f);
   print_e_resistance_test(1185.5f);
   print_e_resistance_test(4.f);
   print_e_resistance_test(1200.f);
   return 0;
}

void print_e_resistance_test(float orig_resistance)
{
    int count = e_resistance(orig_resistance, res);
    printf("%.1f -> %.1f", orig_resistance, res[0]);
    for(int i=1;i<count;i++)
    {
       printf(" + %.1f", res[i]);
    }
    printf("\n");
}
