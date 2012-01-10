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

// #############
// # stegread.c
// #     read hidden data from bitmap file
// ########################################

#include <stdio.h>
#include <stdlib.h>
#include "bmpstego.h"

int bmp_read(FILE *fpread, unsigned char *data, int datasize, int n_last_bits);

int main(int argc, char **argv) {
  FILE *fpread;
  int i;
  int msgsize;
  char s_msgsize[SIZEBYTES+1];
  int n_last_bits;
  unsigned char *data;

  /* usage */
  if (argc != 3) {
    printf("usage: %s <in file> <n last bits> > <out file>\n", argv[0]);
    return 1;
  }

  /* input file */
  if ((fpread = fopen(argv[1], "r")) == NULL) {
    printf("error opening file: %s\n", argv[1]);
    return 2;
  }

  /* n last bits */
  n_last_bits = atoi(argv[2]);
  if (n_last_bits != 1 && n_last_bits != 2 && n_last_bits != 4) {
    printf("error: number of last bits can only be 1, 2 or 4\n");
    fclose(fpread);
    return 3;
  }

  /* ignore bmp header */
  fseek(fpread, BMPHEADERSIZE, SEEK_SET);

  /* reads msgsize */
  bmp_read(fpread, s_msgsize, SIZEBYTES, n_last_bits);

  s_msgsize[SIZEBYTES+1] = '\0';
  msgsize = atoi(s_msgsize);
#ifdef VERBOSE
  fprintf(stderr, "msg size: %d bytes\n", msgsize);
#endif //VERBOSE

  /* reads msg */
  data = (unsigned char *)malloc(msgsize*sizeof(unsigned char));
  bmp_read(fpread, data, msgsize, n_last_bits);

  /* prints msg */
  for (i = 0; i < msgsize; ++i) {
    fprintf(stdout, "%c", data[i]);
  }

  fclose(fpread);
  return 0;
}

int bmp_read(FILE *fpread, unsigned char *data, int datasize, int n_last_bits)
{
  int i;
  int j;
  unsigned char c;
  unsigned char c2;

  if (n_last_bits != 1 && n_last_bits != 2 && n_last_bits != 4) {
    /* error */
    printf("unexpected error calling bmp_read\n");
    fclose(fpread);
    exit(4);
  }

  for (i = 0; i < datasize; ++i) {
    c = 0x00;
    for (j = 0; j < 8; j += n_last_bits) {
      fread(&c2, sizeof(c2), 1, fpread);

      c2 = c2 << (8 - n_last_bits);
      c2 = c2 >> j;

      c = c | c2;
    }
    data[i] = c;
  }

  return 0;
}
