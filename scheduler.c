// Driver file: sched.c file
#include<stdio.h>
#include"sched.h"   //  Custom header file

int main(int argc, char** argv){

  // Arguments checking
  if(argc!=2){
    printf("Usage: ./scheduler  inputFile.txt\n");
    return 0;
  }

  /** File read check  */
  FILE* fd = fopen(argv[1], "r");
  if(fd==NULL){
    printf("Failed to read input file!\n");
    return 0;
  }

  // Generate schedule
  int result = create_schedule(fd);

  return 0;
}
