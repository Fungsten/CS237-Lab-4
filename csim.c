/*
   Will Fung, Jian Lu
   Lab 4, CS237 Spring 2018
*/

#include "cachelab.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <math.h>

typedef struct Line { // Change from Lines to Line
  int valid; // isValid or notIsValid
  int tag; // specifies line
  int offset;
  int *blocks; // which of the blocks does the desired data live in
  int dirty; // 0 if unmodded, 1 if modded
  int LRU; // This is the clock, least recently used
} Line;

typedef struct Set {
  struct Line *lines;
} Set;

typedef struct Cache {
  struct Set *sets;
} Cache;

void memory(int, int, int, Cache*);

int main(int argc, char **argv)
{
    FILE *fp;
    char str[60];
    int c;
    int s;
    int E;
    int b;
    Cache thecache; // come up with better name later

    //printSummary(0, 0 ,0);

    while ((c = getopt (argc, argv, "hvs:E:b:t:")) != -1)
      switch (c) {
        case 'h':
          // Optional help flag that prints usage info
          printf("Usage info\n");
          break;
        case 'v':
          // Optional verbose flag that displays trace info
          break;
        case 's':
          printf("%s\n", optarg); //optarg
          // Number of set index bits (S = 2^s is the number of sets)
          s = pow(2, atoi(optarg));
          break;
        case 'E':
          printf("%s\n", optarg);
          // Associativity (number of lines per set)
          E = atoi(optarg);
          break;
        case 'b':
          printf("%s\n", optarg);
          // Number of block bits (B = 2^b is the block size)
          b = pow(2, atoi(optarg));
          break;
        case 't':
          // Name of the valgrind trace to replay
          printf("Trying to open file.\n");
          printf("%s\n", optarg);    //increment by two if -h and -v are implemented
          fp = fopen(optarg, "r");
          if (fp == NULL) {
            perror("Error opening file");
            return -1;
          }
          while (fgets(str, 60, fp) != NULL) {
            puts(str);
            memory(s, E, b, &thecache);
          }

          fclose(fp);

          break;
        default:
          //another thing
          printf("Hello world.\n");
          break;
      }


return 0;
}

// void parse() // maybe later

void memory(int s, int E, int b, Cache *c){
  //memory allocation for sets, lines per set, and bit blocks for line
  Set *sets = malloc(s*sizeof(Set));//allocation for number of sets
  for (int i = s; i > 0; i--){
    Line *lines = malloc(E*sizeof(struct Line));
    for (int j = E; j > 0; j--){
      lines[j].blocks = malloc(b);
    }
    sets[i].lines = lines;
  }
  (*c).sets = sets;   // does the same as below
  // x->sets = sets;  // does the same as above
}

// Free the malloc'd stuff
void freeTheCache(Cache *c){

}
