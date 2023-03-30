
/*++*
 * Copyright (c) 1997 University of British Columbia. All rights reserved.
 *
 *       File:           decode.c
 *       Pourpose:       contains the main decoding loop.
 *
 *       Author:         Ismaeil R. Ismaeil, Aug. 1997
 *
 *--*/

#include "globals.h"
#include "prototypes.h"
#include "marker.h"
#include "read_write.h"

#define Usage "Usage: %s image1 output [OPTIONS]\n\
\n\
\timage1 - compressed\n\
\toutput - reconstructed image\n\
\n\
\n"

FILE *ifp , *ofp;
int  **in_buf;
int	 **out_buf;
int  count;
long P, T1, T2, T3, MAXVAL;
int Ix, Ra, Rb, Rc, Rd;
int EOLine;
long RANGE;
int A[367], B[365], C[365], N[367], Nn[367];
int RUNindex, RUNval, RUNcnt;
int D[3], Q[3], q;
int SIGN;
int Px, Rx;
int Errval, MErrval, EMErrval;
int k, TEMP;
int RItype, map;
/* updated variables */
long LIMIT, qbpp, bpp;
struct Image_Coord pdim, dim, x, a, b, c, d;

static int bit_set_mask[] =
  {0x00000001,0x00000002,0x00000004,0x00000008,
   0x00000010,0x00000020,0x00000040,0x00000080,
   0x00000100,0x00000200,0x00000400,0x00000800,
   0x00001000,0x00002000,0x00004000,0x00008000,
   0x00010000,0x00020000,0x00040000,0x00080000,
   0x00100000,0x00200000,0x00400000,0x00800000,
   0x01000000,0x02000000,0x04000000,0x08000000,
   0x10000000,0x20000000,0x40000000,0x80000000};

static int current_write_byte;
static int current_read_byte;
static int read_position;
static int write_position;

#define mput1()                                                         \
  {current_write_byte|=bit_set_mask[write_position--];                  \
    if (write_position<0) {putc(current_write_byte,ofp);                \
      if(current_write_byte == 255)                                     \
        write_position=6;else write_position=7; current_write_byte=0;}}

#define mput0()                                                         \
  {write_position--;if(write_position<0){putc(current_write_byte,ofp);  \
      write_position=7;                                                 \
      current_write_byte=0;}}


int main(int argc,char *argv[])
{
  char 		**dim2();
  int 		i,j;
  char 		s1[64], s2[64];
  double  start, finish;
  double 	duration;
  int     tmp;

  if((argc<3)||*argv[2]=='-'){
    printf("\n%s: Command line does not have required arguments.\n\n",argv[0]);
    printf(Usage,argv[0]);exit(-1);
  }

  /*Get image file names*/
  strcpy(s1,argv[1]);
  strcpy(s2,argv[2]);

  ifp= fopen(s1,"rb");
  ofp = fopen(s2,"wb");

  /* Read Header information */
  ReadSoi();
  ReadSof();
  ReadSos();
  MAXVAL = (long)(pow(2.0,P)-1);

  /* output note */

  fprintf(stderr,"\nPLEASE NOTE THAT THIS REALEASE HAS NOT BEEN OPTIMIZED FOR SPEED,\n");
  fprintf(stderr,"AND SHOULD NOT BE USED FOR BENCHMARKING, ETC.\n\n");

  fprintf(stderr,"dim.x = %d, dim.y = %d\n", dim.x, dim.y);
  fprintf(stderr,"P = %d, MAXVAL = %d\n", P, MAXVAL);

  pdim.x = dim.x + 2;
  pdim.y = dim.y + 2;

  out_buf = (int **) dim2(pdim.x,pdim.y,sizeof(int));

  /* set the decoding parameters */
  if(P<=8){
    T1 = 3;
    T2 = 7;
    T3 = 21;
  }
  else {
    T1 = 18;
    T2 = 67;
    T3 = 276;
  }

  /* initialize the boundary conditions */

  RANGE = MAXVAL + 1;

  /* updated variable */
  qbpp =  (long)(log(RANGE)/(log(2.0)));
  bpp  = MAX(2,(long)(log(MAXVAL+1)/log(2.0)));
  LIMIT = 2*(bpp+MAX(8,bpp));

  /* initialaize some variables */
  tmp = MAX(2,(int)((RANGE+32)/64));
  for(i=0; i< 365; i++){
    A[i] = tmp;
    N[i] = 1;
    B[i] = 0;
    Nn[i] = 0;
    C[i] = 0;}
  A[365] = tmp;
  A[366] = tmp;
  N[365] = 1;
  N[366] = 1;
  RUNindex = 0;
  Nn[365] = 0;
  Nn[366] = 0;
  x.x = 1;
  x.y = 0;

  for(i=0; i<pdim.y; i++)
    out_buf[0][i] = 0;
  out_buf[1][0] = 0;


  read_position = -1;

  printf("Start image transformation....\n");
  start = clock();

  for(;;){

    if(x.x == dim.x && x.y == dim.y )
      break;

    SetSample();
    D[0] = Rd - Rb;
    D[1] = Rb - Rc;
    D[2] = Rc - Ra;

    if (D[0] == 0 && D[1] == 0 && D[2] == 0)
      RunModeProcessing();
    else
      RegularModeProcessing();
  }

  if(P <= 8)
    write_pic(ofp,dim.x,dim.y,out_buf);
  else
    write_int_pic(ofp,dim.x,dim.y,out_buf); 
 
  fclose(ofp);
  fclose(ifp);

  finish = clock();
  duration = (finish-start)/CLOCKS_PER_SEC;
  printf("Image transformed in ....\n");
  printf("%f seconds.\n", duration);

}

/******************************************************
GetFromBitStream() gets one bit from the bit stream.
******************************************************/

int GetFromBitStream()
{
  if (read_position < 0)
    {
      if(current_read_byte == 0xff)
        read_position=6;
      else
        read_position=7;
      current_read_byte = getc(ifp);
    }
  if (current_read_byte&bit_set_mask[read_position--])
    {
      return(1);
    }
  return(0);
}
