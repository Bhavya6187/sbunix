# include <stdio.h>
# include <libc/malloc.h>

int main()
{
  char *a = (char *) malloc(sizeof(char) * 64);

  a = "Dushant Goyal loves mota";
  u_printf("%s", a);
  return 0;
}
