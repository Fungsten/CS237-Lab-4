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
#include <string.h>

typedef struct Line { // Change from Lines to Line
  int valid; // isValid or notIsValid
  int tag; // specifies line
  int LRU; // This is the clock, least recently used
} Line;

typedef struct Set {
  struct Line *lines;
} Set;

typedef struct Cache {
  struct Set *sets;
} Cache;

void memory(int, int, int, Cache*);
void freeTheCache(int, int, int, Cache*);
unsigned int getTag(int, int, int);
unsigned int getSet(int, int, int);
void cacheSim(Cache*, char*, int, int, int, int, int*);
// void printSummary(int, int, int, int, int);

int main(int argc, char **argv)
{
    FILE *fp;
    char str[60];
    int c, s, E, b, setbit, blockbit;
    int help = 0, verbose = 0;
    int hitRate[3] = {0, 0, 0};
    Cache thecache; // come up with better name later

    while ((c = getopt (argc, argv, "hvs:E:b:t:")) != -1)
      switch (c) {
        case 'h':
          // Optional help flag that prints usage info
          help = 1;
          printf("Usage info\n");
          break;
        case 'v':
          // Optional verbose flag that displays trace info
          printf("Verbose mode\n");
          verbose = 1;
          break;
        case 's':
          printf("%s\n", optarg); //optarg
          // Number of set index bits (S = 2^s is the number of sets)
          setbit = atoi(optarg);
          s = pow(2, atoi(optarg));
          break;
        case 'E':
          printf("%s\n", optarg);
          // Associativity (number of lines per set)
          E = atoi(optarg);
          break;
        case 'b':
          blockbit = atoi(optarg);
          printf("%s\n", optarg);
          // Number of block bits (B = 2^b is the block size)
          b = pow(2, atoi(optarg));
          break;
        case 't':
          // Name of the valgrind trace to replay
          printf("Trying to open file.\n");
          printf("%s\n", optarg);
          fp = fopen(optarg, "r");
          if (fp == NULL) {
            perror("Error opening file");
            return -1;
          }
          while (fgets(str, 60, fp) != NULL) {
            // puts(str);
            memory(s, E, b, &thecache);
            cacheSim(&thecache, str, setbit, blockbit, help, verbose, hitRate);
	          thecache.sets[16].lines[1].valid = 1;
	          printf("valid: %d\n", thecache.sets[1].lines[1].valid);
          }

          fclose(fp);

          break;
        default:
          //another thing
          printf("Hello world.\n");
          break;
      }

printSummary(hitRate[0], hitRate[1], hitRate[2]);
freeTheCache(s, E, b, &thecache);
return 0;
}

void memory(int s, int E, int b, Cache *c){
  //memory allocation for sets, lines per set, and bit blocks for line
  Set *sets = malloc(s*sizeof(Set)); //allocation for number of sets

  if (sets == NULL) {
    fprintf(stderr, "Error - unable to allocate memory for sets\n");
  }

  for (int i = s; i > 0; i--){
    Line *lines = malloc(E*sizeof(struct Line));

    if (lines == NULL) {
      fprintf(stderr, "Error - unable to allocate memory for lines\n");
    }
    sets[i].lines = lines;
  }
  (*c).sets = sets;   // does the same as below
  // x->sets = sets;  // does the same as above
}

// Free the malloc'd stuff
void freeTheCache(int s, int E, int b, Cache *c){
  printf("Reached the end; now freeing the cache\n");
  for (int i = s; i > 0; i--){
    free((*c).sets[i].lines);
  }
  free((*c).sets);
}

// helper function for reading in the data
void cacheSim(Cache *c, char *a, int b, int s, int help, int verbose, int *hitRate){
  // printf("first print: %c\n", a[0]);
  int addr, tag, set;
  // int hits = 0, misses = 0, evicts = 0;
  char instr[2], ignore[10], trace[20];
  strcpy(trace, a);
  sscanf(trace, "%s %x,%s", instr, &addr, ignore);

  if(a[0] == ' ') {
    printf("test print: %s, %x, %s\n", instr, addr, ignore);
    tag = getTag(addr, b, s);
    set = getSet(addr, b, s);

    printf("tag: %x\n", tag);
    printf("set: %d\n", set);

    printf("%s\n", (instr)); // why does this work but char i = (instr) not?!?!

    if (a[1] == 'L') {
      printf("L\n");
      hitRate[0]++;
    }
    if (a[1] == 'S') {
      printf("S\n");
    }
    if (a[1] == 'M') {
      printf("M\n");
    }

  } else {
    printf("Instruction ignored\n");
  }

}

unsigned int getTag(int addr, int b, int s){
  int tag, u_addr;
  u_addr = (unsigned) addr;
  // printf("addr, u: %x, %x\n", addr, u_addr);
  // printf("b, s: %x, %x\n", b, s);
  tag = ((u_addr >> b) >> s);
  return tag;
}

unsigned int getSet(int addr, int b, int s){
  int set, u_addr;
  u_addr = (unsigned) addr;
  set = (u_addr >> b) & ~(~0 <<  s);
  return set;
}
