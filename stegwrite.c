/*
    Copyright 2005 Eduardo Sztokbant <du@du.eti.br>

    This file is part of stegotools.

    stegotools is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    stegotools is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with stegotools; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

// ##############
// # stegwrite.c
// #     hides data in a bitmap file
// ##################################

#include <stdio.h>
#include <stdlib.h>
#include "bmpstego.h"

int bmp_write(FILE *fpread, FILE *fpwrite, unsigned char *data, int datasize,
	      int n_last_bits);
int sum_last_bits(int n);

int main(int argc, char **argv) {
  FILE *fpread;
  FILE *fpwrite;
  unsigned char *data;
  int maxdata;
  int datasize;
  char s_datasize[SIZEBYTES];
  unsigned char c;
  int i;
  int n_last_bits;

  /* usage */
  if (argc != 4) {
    printf("usage: echo \"data\" | %s <in file> <out file> <n last bits>\n",
	   argv[0]);
    printf("       cat datafile | %s <in file> <out file> <n last bits>\n",
	   argv[0]);
    return 1;
  }

  /* input file */
  if ((fpread = fopen(argv[1], "r")) == NULL) {
    printf("error opening file: %s\n", argv[1]);
    return 2;
  }

  /* output file */
  if ((fpwrite = fopen(argv[2], "w")) == NULL) {
    printf("error opening file: %s\n", argv[2]);
    fclose(fpread);
    return 2;
  }

  /* n last bits */
  n_last_bits = atoi(argv[3]);
  if (n_last_bits != 1 && n_last_bits != 2 && n_last_bits != 4) {
    printf("error: number of last bits can only be 1, 2 or 4\n");
    fclose(fpread);
    fclose(fpwrite);
    return 3;
  }

  /* calculates maxdata */
  i=0;
  fseek(fpread, BMPHEADERSIZE, SEEK_SET);
  fread(&c, sizeof(c), 1, fpread);
  while (!feof(fpread)) {
    i++;
    fread(&c, sizeof(c), 1, fpread);
  }
  maxdata=((i*n_last_bits)/8) - SIZEBYTES;
#ifdef VERBOSE
  fprintf(stderr, "max data size: %d bytes\n", maxdata);
#endif //VERBOSE

  /* returns file position indicator to start */
  fseek(fpread, 0, SEEK_SET);

  /* data is read from stdin */
  data = (char *)malloc(maxdata*sizeof(char));
  datasize=0;
  while (fread(&c, sizeof(c), 1, stdin)) {
    if (datasize == maxdata) {
      printf("warning: data truncated at byte %d\n", datasize);
      break;
    }
    data[datasize] = c;
    datasize++;
  }
  snprintf(s_datasize, SIZEBYTES, "%d", datasize);

  /* copy bmp header */
  for (i = 0; i < BMPHEADERSIZE; i++) {
    fread(&c, sizeof(c), 1, fpread);
    fprintf(fpwrite, "%c", c);
  }

  /* stores datasize */
  bmp_write(fpread, fpwrite, s_datasize, SIZEBYTES, n_last_bits);

  /* stores data */
  bmp_write(fpread, fpwrite, data, datasize, n_last_bits);

  /* copy remaining bytes */
  while (fread(&c, sizeof(c), 1, fpread)) {
    fprintf(fpwrite, "%c", c);
  }

  free(data);
  fclose(fpread);
  fclose(fpwrite);
  return 0;
}

int bmp_write(FILE *fpread, FILE *fpwrite, unsigned char *data, int datasize,
	      int n_last_bits)
{
  int i, j;
  unsigned char c, c2;
  int dataidx = 0;

  if (n_last_bits != 1 && n_last_bits != 2 && n_last_bits != 4) {
    /* error */
    printf("unexpected error calling bmp_write\n");
    free(data);
    fclose(fpread);
    fclose(fpwrite);
    exit(4);
  }

  for (i = 0; i < datasize; ++i) {
    for (j = 0; j < 8; j += n_last_bits) {
      /* read one char of the file */
      if (!fread(&c, sizeof(c), 1, fpread)) {
	printf("EOF error.\n");
	free(data);
	fclose(fpread);
	fclose(fpwrite);
	exit(2);
      }
      
      /* write data in the n_last_bits of c */
      c = c & (0xFF - sum_last_bits(n_last_bits));
      c2 = data[dataidx];
      c2 = c2 << j;
      c2 = c2 >> (8 - n_last_bits);
      c = c | c2;

      fprintf(fpwrite, "%c", c);
    }  
    
    dataidx++;
  }

  return 0;
}

int sum_last_bits(int n)
{
  if (n == 0) {
    return 0;
  } else if (n == 1) {
    return 1;
  } else {
    return 2 * (sum_last_bits(n-1)) + 1;
  }
}
