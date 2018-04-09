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


struct Lines {
  int valid; // isValid or notIsValid
  int tag; // specifies line
  int offset;
  int offset_size; // which of the blocks does the desired data live in
  int dirty; // 0 if unmodded, 1 if modded
  int LRU; // This is the clock, least recently used
};



int main(int argc, char **argv)
{
    FILE *fp;
    char str[60];
    int c;

    //char *cvalue = NULL;

    //printSummary(0, 0 ,0);

    // printf("Trying to open file.\n");
    // fp = fopen(argv[1], "r");
    // if (fp == NULL) {
    //   perror("Error opening file");
    //   return -1;
    // }
    // if (fgets(str, 60, fp) != NULL) {
    //   puts(str);
    // }
    // fclose(fp);



    while ((c = getopt (argc, argv, "hvs:E:b:t:")) != -1)
      switch (c) {
        struct Lines *l = malloc(sizeof(*l));
        l->dirty = 0;
        l->LRU = 0;
        l->offset_size = 2 << atoi(argv[6]);
        case 'h':
          // Optional help flag that prints usage info
          printf("Usage info\n");
          break;
        case 'v':
          // Optional verbose flag that displays trace info
          break;
        case 's':
          printf("%s\n", argv[2]);
          // Number of set index bits (S = 2^s is the number of sets)
          break;
        case 'E':
          printf("%s\n", argv[4]);
          // Associativity (number of lines per set)
          break;
        case 'b':
          printf("%s\n", argv[6]);
          // Number of block bits (B = 2^b is the block size)
          // l->offset_size = 2 << atoi(argv[6]);
          break;
        case 't':
          // Name of the valgrind trace to replay
          printf("Trying to open file.\n");
          printf("%s\n", argv[8]);    //increment by two if -h and -v are implemented
          fp = fopen(argv[8], "r");
          if (fp == NULL) {
            perror("Error opening file");
            return -1;
          }
          while (fgets(str, 60, fp) != NULL) {
            puts(str);

          }

          fclose(fp);

          break;
        default:
          //another thing
          printf("Hello world.\n");
          break;
        //FREE the malloc, unleash the Kraken
        free(l);
      }

return 0;
}
