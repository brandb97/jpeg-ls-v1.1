/*++*
 * Copyright (c) 1997 University of British Columbia. All rights reserved. 
 *
 * 	File: 		encode.c
 *       Pourpose:    	contains the main encoding loop.
 *                                                                              
 *       Author:		Ismaeil R. Ismaeil, Aug. 1997
 *
 *--*/

#include "globals.h"
#include "prototypes.h"
#include "marker.h"
#include "read_write.h"

#define Usage "Usage: %s image1 output nrows ncols P\n\
\n\
\timage1 - Original image\n\
\toutput - compressed bit stream\n\
\tnrows  - # of image lines\n\
\tncols  - # of pel/line\n\
\tP      - Precision \n\
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

  if((argc<6)||*argv[2]=='-'){
    fprintf(stderr, "\n%s: Command line does not have required arguments.\n\n",argv[0]);
    fprintf(stderr, Usage,argv[0]);exit(-1);
  }

  /*Get image file names*/

  strcpy(s1,argv[1]);
  strcpy(s2,argv[2]);

  if((dim.x = atof(argv[3])) <= 0){
    fprintf(stderr, "%s Error: nrows negative or zero...\n");
    exit(-1);}
  if((dim.y = atof(argv[4])) <= 0){
    fprintf(stderr, "%s Error: ncols negative or zero....\n");
    exit(-1);}
  P = atoi(argv[5]);
  /* if(P < 0 || P >> 16 ){ */
  /*   fprintf(stderr, "%s Error: Invalid Precision....P should be between 8 and 16\n"); */
  /*   exit(-1);} */
 
  pdim.x = dim.x+1;
  pdim.y = dim.y+2;

  /* output note */

  fprintf(stderr,"\nPLEASE NOTE THAT THIS REALEASE HAS NOT BEEN OPTIMIZED FOR SPEED,\n");
  fprintf(stderr,"AND SHOULD NOT BE USED FOR BENCHMARKING, ETC.\n\n");
 
  /* set the encoding parameters based on precision value */

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
  MAXVAL = (long)(pow(2.0,P)-1);
  fprintf(stderr,"Precision = %ld, MAXVAL = %ld\n", P, MAXVAL);
 
  /* Write Header Information */
 
  ofp = fopen(s2,"wb");
  WriteSoi();
  WriteSof();
  WriteSos();
 
  in_buf = (int **) dim2(pdim.x,pdim.y,sizeof(int));
 
  fprintf(stderr, "Reading input image.\n");
  start = clock();
  ifp= fopen(s1,"rb");
  if(P > 8)
    read_int_pic(ifp,dim.x,dim.y,in_buf);
  else
    read_pic(ifp,dim.x,dim.y,in_buf);
  fclose(ifp);
  finish = clock();
 
  fprintf(stderr, "input image read in %f seconds.\n",(finish-start)/CLOCKS_PER_SEC);
    
  /* initialize the boundary conditions */
 
  for(j=0; j<pdim.y; j++)
    in_buf[0][j] = 0;
  for(i=1; i<pdim.x; i++){
    in_buf[i][0] = in_buf[i-1][1];
    in_buf[i][pdim.y-1] = in_buf[i][pdim.y-2];}
 
  /*  RANGE = (int)((MAXVAL+2*NEAR)/(2*NEAR+1))+1; */
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
  count = 0;
  EOLine = 0;
  current_write_byte = 0;
  write_position = 7;
 
  fprintf(stderr, "Start image transformation....\n");
  start = clock();
 
  for(;;){
 
    if(count == dim.x*dim.y )
      break;
 
    GetNextSample();
    D[0] = Rd - Rb;
    D[1] = Rb - Rc;
    D[2] = Rc - Ra;
 
    if (D[0] == 0 && D[1] == 0 && D[2] == 0)
      RunModeProcessing();
    else
      RegularModeProcessing();
  }
 
  if(write_position < 7)
    putc(current_write_byte, ofp);
  WriteEoi();
  fclose(ofp);
 
  finish = clock();
  duration = (finish-start)/CLOCKS_PER_SEC;
  fprintf(stderr, "Image transformed in ....\n");
  fprintf(stderr, "%f seconds.\n", duration);
 
}
 
/**************************************************************
AppendToBitStream() puts n bits from b onto the writer stream.
**************************************************************/
 
void AppendToBitStream(int b,int n)
{
 
  if((b>>n) > 0 || b < 0){
    fprintf(stderr,"Error: %d Bits are not enough to represent the number %d\n", n, b);
    exit(0);
  }

  if (n > 32) {
    fprintf(stderr,
            "Error: n is so big", n);
    exit(0);
  }
 
  while(n--)
    {
      if(b&bit_set_mask[n]) {mput1();}
      else {mput0();}
    }
}
