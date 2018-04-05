
#include "cachelab.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *fp;
    char str[60];
    int c;
    char *cvalue = NULL;

    //printSummary(0, 0 ,0);

    fp = fopen("file.txt", "r");
    if (fp == NULL) {
      perror("Error opening file");
      return -1;
    }
    if (fgets(str, 60, fp) != NULL) {
      puts(str);
    }
    fclose(fp);

    /*
    while ((c = getopt (argc, argv, "ac:")) != -1)
      switch (c) {
       case 'h':
        // Optional help flag that prints usage info
        break;
       case 'c':
        //meh
        cvalue = optarg;
        break;
       default:
        //another thing
      }
    */
    return 0;
}
