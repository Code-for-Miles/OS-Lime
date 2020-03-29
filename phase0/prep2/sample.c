#include <spede/stdio.h>
#include <spede/flames.h>

int main(void) {
  
  long i;
  i=28;

  printf("%l Hello World %l \n", i, i*28);

  cons_printf("--> Hello World <--\n CPE/CSC");

  return 0;
}
