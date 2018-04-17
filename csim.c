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
  long tag; // specifies line
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
unsigned long getTag(long, int, int);
unsigned long getSet(long, int, int);
void cacheSim(Cache*, char*, int, int, int, int*, int, int);
void printCache(int, int, Cache*);

int main(int argc, char **argv)
{
    FILE *fp;
    char str[60];
    int c, s, E, b, setbit, blockbit;
    int verbose = 0;
    int hitRate[3] = {0, 0, 0};
    int counter = 0;
    Cache thecache; // come up with better name later

    while ((c = getopt (argc, argv, "hvs:E:b:t:")) != -1)
      switch (c) {
        case 'h':
          // Optional help flag that prints usage info
          printf("Usage info:\n");
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
	         memory(s,E,b, &thecache);
          while (fgets(str, 60, fp) != NULL) {
            // puts(str);

            cacheSim(&thecache, str, setbit, blockbit, verbose, hitRate, counter, E);
	          // thecache.sets[16].lines[1].valid = 1;
	          // printf("valid: %d\n", thecache.sets[1].lines[1].valid);
            // printCache(E, s, &thecache);
            counter++;
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

  for (int i = 0; i < s; i++){
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
  for (int i = 0; i < s; i++){
    free((*c).sets[i].lines);
  }
  free((*c).sets);
}

// helper function for reading in the data
void cacheSim(Cache *c, char *a, int s, int b, int verbose, int *hitRate, int counter, int E){
  // printf("first print: %c\n", a[0]);
  long addr, tag, set;
  int hit = 0, evict = 0;
  // int hits = 0, misses = 0, evicts = 0;
  char instr[2], ignore[10], trace[20];
  strcpy(trace, a);
  sscanf(trace, "%s %lx,%s", instr, &addr, ignore);

  if (a[0] == ' ') {
    if (verbose == 1) {
      printf("test print: %s %lx, %s\n", instr, addr, ignore);
    }
    tag = getTag(addr, b, s);
    set = getSet(addr, b, s);

    printf("tag: %lx\n", tag);
    printf("set: %ld\n", set);

    // thecache.sets[16].lines[1].valid = 1;
    // printf("valid: %d\n", thecache.sets[1].lines[1].valid);

    printf("%s\n", (instr)); // why does this work but char i = (instr) not?!?!

    if (a[1] == 'L') {
      printf("seg fault 1\n");

      for (int i = 0; i < E; i++) {
        printf("seg fault 2\n");
        printf("valid, tag: %d, %ld\n", (*c).sets[set].lines[i].valid, (*c).sets[set].lines[i].tag);
        if ((*c).sets[set].lines[i].valid == 1 && (*c).sets[set].lines[i].tag == tag) {
          printf("It's a hit!\n");
          hitRate[0]++; // hit
          (*c).sets[set].lines[i].LRU = counter;
          hit = 1; // to know not to run miss condition
          evict = 1; // to know not to run evict condition
          break;
        }
      }

      if (hit == 0) {
        hitRate[1]++;
        for (int i = 0; i < E; i++) {
          if ((*c).sets[set].lines[i].valid == 0) {
            (*c).sets[set].lines[i].tag = tag;
            (*c).sets[set].lines[i].LRU = counter;
            (*c).sets[set].lines[i].valid = 1;
            evict = 1; // to know not to run evict condition
            break;
          }
        }
      }

      if (evict == 0) {
        printf("Have to evict!\n");
        hitRate[2]++;
        Line *eviction = &(*c).sets[set].lines[0];
        for(int j = 1; j < E; j++){
          if ((*eviction).LRU > (*c).sets[set].lines[j].LRU){
            eviction = &((*c).sets[set].lines[j]);
          }
        }
        (*eviction).tag = tag;
        (*eviction).LRU = counter;
      }
    }
    if (a[1] == 'S') {
      // printf("S\n");

      for (int i = 0; i < E; i++) {
        printf("valid, tag: %d, %ld\n", (*c).sets[set].lines[i].valid, (*c).sets[set].lines[i].tag);
        if ((*c).sets[set].lines[i].valid == 1 && (*c).sets[set].lines[i].tag == tag) {
          printf("It's a hit!\n");
          hitRate[0]++; // hit
          (*c).sets[set].lines[i].LRU = counter;
          hit = 1; // to know not to run miss condition
          evict = 1; // to know not to run evict condition
          break;
        }
      }

      if (hit == 0) {
        printf("It's a miss!\n");
        hitRate[1]++;
        for (int i = 0; i < E; i++) {
          if ((*c).sets[set].lines[i].valid == 0) {
            printf("Found an empty line!\n");
            (*c).sets[set].lines[i].tag = tag;
            (*c).sets[set].lines[i].LRU = counter;
            (*c).sets[set].lines[i].valid = 1;
            evict = 1; // to know not to run evict condition
            break;
          }
        }
      }

      if (evict == 0) {
        printf("Have to evict!\n");
        hitRate[2]++;
        Line *eviction = &(*c).sets[set].lines[0];
        for(int j = 1; j < E; j++){
          if ((*eviction).LRU > (*c).sets[set].lines[j].LRU){
            eviction = &((*c).sets[set].lines[j]);
          }
        }
        (*eviction).tag = tag;
        (*eviction).LRU = counter;
      }

    }
    if (a[1] == 'M') {
      // printf("M\n");
      // do L
      for (int i = 0; i < E; i++) {
        printf("valid, tag: %d, %ld\n", (*c).sets[set].lines[i].valid, (*c).sets[set].lines[i].tag);
        if ((*c).sets[set].lines[i].valid == 1 && (*c).sets[set].lines[i].tag == tag) {
          printf("It's a hit!\n");
          hitRate[0]++; // hit
          (*c).sets[set].lines[i].LRU = counter;
          hit = 1; // to know not to run miss condition
          evict = 1; // to know not to run evict condition
          break;
        }
      }

      if (hit == 0) {
        printf("It's a miss!\n");
        hitRate[1]++;
        for (int i = 0; i < E; i++) {
          if ((*c).sets[set].lines[i].valid == 0) {
            printf("Found an empty line!\n");
            (*c).sets[set].lines[i].tag = tag;
            (*c).sets[set].lines[i].LRU = counter;
            (*c).sets[set].lines[i].valid = 1;
            evict = 1; // to know not to run evict condition
            break;
          }
        }
      }

      if (evict == 0) {
        printf("Have to evict!\n");
        hitRate[2]++;
        Line *eviction = &(*c).sets[set].lines[0];
        for(int j = 1; j < E; j++){
          if ((*eviction).LRU > (*c).sets[set].lines[j].LRU){
            eviction = &((*c).sets[set].lines[j]);
          }
        }
        (*eviction).tag = tag;
        (*eviction).LRU = counter;
      }
      counter++;
      // do S
      for (int i = 0; i < E; i++) {
        printf("valid, tag: %d, %ld\n", (*c).sets[set].lines[i].valid, (*c).sets[set].lines[i].tag);
        if ((*c).sets[set].lines[i].valid == 1 && (*c).sets[set].lines[i].tag == tag) {
          printf("It's a hit!\n");
          hitRate[0]++; // hit
          (*c).sets[set].lines[i].LRU = counter;
          hit = 1; // to know not to run miss condition
          evict = 1; // to know not to run evict condition
          break;
        }
      }

      if (hit == 0) {
        printf("It's a miss!\n");
        hitRate[1]++;
        for (int i = 0; i < E; i++) {
          if ((*c).sets[set].lines[i].valid == 0) {
            printf("Found an empty line!\n");
            (*c).sets[set].lines[i].tag = tag;
            (*c).sets[set].lines[i].LRU = counter;
            (*c).sets[set].lines[i].valid = 1;
            evict = 1; // to know not to run evict condition
            break;
          }
        }
      }

      if (evict == 0) {
        printf("Have to evict!\n");
        hitRate[2]++;
        Line *eviction = &(*c).sets[set].lines[0];
        for(int j = 1; j < E; j++){
          if ((*eviction).LRU > (*c).sets[set].lines[j].LRU){
            eviction = &((*c).sets[set].lines[j]);
          }
        }
        (*eviction).tag = tag;
        (*eviction).LRU = counter;
      }
    }

  } else {
    printf("Instruction ignored\n");
  }

}

unsigned long getTag(long addr, int b, int s){
  long tag, u_addr;
  u_addr = (unsigned) addr;
  // printf("addr, u: %lx, %lx\n", addr, u_addr);
  // printf("b, s: %lx, %lx\n", b, s);
  tag = ((u_addr >> b) >> s);
  return tag;
}

unsigned long getSet(long addr, int b, int s){
  long set, u_addr;
  u_addr = (unsigned) addr;
  printf("Set params; addr: %ld, b: %d, s: %d", addr, b, s);
  set = (u_addr >> b) & ~(~0L << s);
  return set;
}

void printCache(int E, int s, Cache *c){
  //debugging purposes printing function
  //write a for loop for each set
  for (int i = 0; i < s; i++){
    printf("Printing cache set: %d\n", i);
    for (int j = 0; j < E; j++){
      //calls the lines
      printf("tag: %lx\n",(*c).sets[i].lines[j].tag);
      printf("LRU: %d\n",(*c).sets[i].lines[j].LRU);
      printf("valid: %d\n",(*c).sets[i].lines[j].valid);
    }
  }
}

// if (hit == 0){
//   for (int i = 0; i < E; i++) {
//     if ((*c).sets[set].lines[i].valid == 1 && (*c).sets[set].lines[i].tag != tag){
//       printf("Hello world!!\n");
//       Line *eviction = &(*c).sets[set].lines[0];
//       for(int j = 1; j <E; j++){
//         if ((*eviction).LRU > (*c).sets[set].lines[j].LRU){
//           (*eviction) = (*c).sets[set].lines[j];
//           (*eviction).tag = tag;
//           (*c).sets[set].lines[i].LRU = counter;
//           hitRate[1]++; // miss
//           hitRate[2]++; // evict
//           break;
//         }
//       }
//     } else {
//       printf("Your trolling\n");
//       (*c).sets[set].lines[i].tag = tag;
//       (*c).sets[set].lines[i].valid = 1;
//       (*c).sets[set].lines[i].LRU = counter;
//       hitRate[1]++; // miss
//     }
//   }
// }


// for (int i = 0; i < E; i++) {
//   if ((*c).sets[set].lines[i].valid == 1 && (*c).sets[set].lines[i].tag == tag) {
//     hitRate[0]++; // hit
//     (*c).sets[set].lines[i].LRU = counter;
//     break;
//   }
//   if ((*c).sets[set].lines[i].valid == 1 && (*c).sets[set].lines[i].tag != tag){
//     Line *eviction = &(*c).sets[set].lines[0];
//     for(int j = 1; j <E; j++){
//       if ((*eviction).LRU > (*c).sets[set].lines[j].LRU){
//         (*eviction) = (*c).sets[set].lines[j];
//         (*eviction).tag = tag;
//         hitRate[1]++; // miss
//         hitRate[2]++; // evict
//         break;
//       }
//     }
//   } else {
//     (*c).sets[set].lines[i].tag = tag;
//     hitRate[1]++; // miss
//   }
// }
