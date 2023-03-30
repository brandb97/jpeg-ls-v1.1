/*++*
 * Copyright (c) 1997 University of British Columbia. All rights reserved.
 *
 *       File:           encode.h
 *       Pourpose:       contains global functions.
 *
 *       Author:         Ismaeil R. Ismaeil, Aug. 1997
 *
 *--*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <time.h>

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1e6
#endif

extern FILE *ifp , *ofp;
extern int  **in_buf;
extern int	 **out_buf;

struct Image_Coord { int x, y; };

extern struct Image_Coord pdim, dim, x, a, b, c, d;

extern int  count;

/* MAX and MIN macros */
#define MAX(x,y) ((x > y) ? x:y)
#define MIN(x,y) ((x > y) ? y:x)
#define ABS(x) ((x < 0) ? -x:x)	


#define MAXIMUM_IMAGE_HEIGHT 65535
#define MAXIMUM_IMAGE_WIDTH 65535
#define MAXIMUM_SOURCES 16
#define MAXIMUM_COMPONENTS 256

#define MAX_C 127    /* maximum allowed value of C[0..364] */
#define MIN_C -128   /* minimum allowed value of C[0..364] */
#define NEAR  0      /*difference bound for near-lossless mode */
#define RESET 64     /* threshold value at which A, B, and N are halved */

extern long P, T1, T2, T3, MAXVAL;
extern int Ix, Ra, Rb, Rc, Rd;
extern int EOLine;
extern long RANGE;
extern int A[367], B[365], C[365], N[367], Nn[367];
extern int RUNindex, RUNval, RUNcnt;
extern int D[3], Q[3], q;
extern int SIGN;
extern int Px, Rx;
extern int Errval, MErrval, EMErrval;
extern int k, TEMP;
extern int RItype, map;
/* updated variables */
extern long LIMIT, qbpp, bpp;

