#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void siginthandler(int param)
{
  printf("User pressed Ctrl+C\n");
  exit(1);
}

int main()
{
  signal(SIGINT, siginthandler);
  while(1);
  return 0;
}