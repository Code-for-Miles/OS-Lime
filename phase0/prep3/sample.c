#include <spede/stdio.h>
#include <spede/flames.h>

void DisplayMsg(long i) {
   printf("%l Hello World %l \n", i, i*28);
   cons_printf("--> Hello World <-- CPE/CSC\n");
}

int main(void) {
  
  long i,j;
  i=111;

  for(j=0; j<5; j++) {
    DisplayMsg(i);
    i++;
  }
  return 0;
}
