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


struct Line {
  int valid;
  int tag;
  int offset;
  int dirty;
  int LRU;
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
      case 'h':
        // Optional help flag that prints usage info
        printf("Usage info\n");
        break;
      case 'v':
        // Optional verbose flag that displays trace info
        // cvalue = optarg;
        break;
      case 's':
         // Number of set index bits (S = 2^s is the number of sets)
         break;
      case 'E':
         // Associativity (number of lines per set)
         break;
      case 'b':
         // Number of block bits (B = 2^b is the block size)
         break;
      case 't':
        // Name of the valgrind trace to replay
        printf("Trying to open file.\n");
        printf("%s\n", argv[2]);
        fp = fopen(argv[2], "r");
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
      }

return 0;
}
