/*****************************************************************************

* Author : Ludvig Sundström

* File Name : util.c

* Purpose : 

* Creation Date : 26-06-2015

* Last Modified : 

*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>

void rt_error(char error_text[])
{
  fprintf(stderr,"Run-time error...\n");
  fprintf(stderr,"%s\n",error_text);
  fprintf(stderr,"...now exiting to system...\n");
  exit(1);
}

float *vector(long n) 
{
  float *v;
  v = (float *) malloc((size_t) (n * sizeof(float)));
  if (v == NULL) {
    rt_error("error while allocating memory");
  }
  return v;
}
