#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  int size = 2;
  char *s = calloc(size, sizeof(char));
  printf("%lu\n", size * sizeof(s));

  return (0);
}
