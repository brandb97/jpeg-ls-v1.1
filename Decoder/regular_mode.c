/*++*
 * Copyright (c) 1997 University of British Columbia. All rights reserved.
 *
 *       File:           regular_mode.c
 *       Pourpose:       contains the main routins used in the regular mode.
 *
 *       Author:         Ismaeil R. Ismaeil, Aug. 1997
 *
 *--*/

#include "globals.h"
#include "prototypes.h"

void RegularModeProcessing()
{
  int i, j;

  /* Quantization of the gradients */
  for(i=0; i<3; i++){
    if(D[i] <= -T3) Q[i] = -4;
    else if(D[i] <= -T2) Q[i] = -3;
    else if(D[i] <= -T1) Q[i] = -2;
    else if(D[i] < -NEAR ) Q[i] = -1;
    else if(D[i] <= NEAR) Q[i] = 0;
    else if(D[i] < T1) Q[i] = 1;
    else if(D[i] < T2) Q[i] = 2;
    else if(D[i] < T3) Q[i] = 3;
    else Q[i] = 4;
  }

  /* Quantized gradient merging */

  if(Q[0] < 0){
    Q[0] = -Q[0];
    Q[1] = -Q[1];
    Q[2] = -Q[2];
    SIGN = -1;}
  else if(Q[0]==0){
    if(Q[1] < 0){
      Q[1] = -Q[1];
      Q[2] = -Q[2];
      SIGN = -1;}
    else if(Q[1] == 0 && Q[2] < 0){
      Q[2] = -Q[2];
      SIGN = -1;}
    else SIGN = 1;}
  else SIGN = 1;

  /* The Maping step to detrmin Q */

  q = Q[0]*81 + Q[1]*9 + Q[2];

  /* Edg-detection predictor */
  
  if(Rc >= MAX(Ra,Rb))
    Px = MIN(Ra,Rb);
  else {
    if(Rc <= MIN(Ra,Rb))
      Px = MAX(Ra,Rb);
    else
      Px = Ra + Rb - Rc;
  }
 
  /* Prediction correction */
 
  if(SIGN == 1)
    Px = Px + C[q];
  else
    Px = Px - C[q];

  if(Px > MAXVAL)
    Px = MAXVAL;
  else if(Px < 0)
    Px = 0;

  /* Prediction error Decoding */

  for(k=0; (N[q]<<k) < A[q]; k++);

  /* Mapped-error Decoding */

  MErrval = GolombDecoding(k, LIMIT);

  /* Error Mapping */
  
  if((NEAR == 0) && (k==0) && (2*B[q] <= -N[q])) {
    if((MErrval&1))
      Errval = (MErrval-1) >> 1;
    else
      Errval = -(MErrval>>1)-1;
  }
  else{
    if((MErrval&1))
      Errval = - ((MErrval+1)>>1);
    else
      Errval = MErrval>>1;
  }

  /* Update variables */

  B[q] = B[q] + Errval*(2*NEAR+1);
  A[q] = A[q] + ABS(Errval);
  if (N[q] == RESET) {
    A[q] = A[q]>>1;
    B[q] = B[q]>>1;
    N[q] = N[q]>>1;
  }
  N[q] = N[q]+1;

  if(NEAR > 0)
    Errval *= (2*NEAR+1);

  if(SIGN == -1)
    Errval = -Errval;

  Rx = (Errval+Px)%(RANGE*(2*NEAR+1));

  /*if(NEAR > 0){ */

  if(Rx < -NEAR)
    Rx = Rx + RANGE*(2*NEAR+1);
  else if (Rx > MAXVAL + NEAR)
    Rx = Rx - RANGE*(2*NEAR+1);

  if(Rx<0)
    Rx = 0;
  else if(Rx > MAXVAL)
    Rx = MAXVAL;

  /* Contex-dependent bias computation */

  if(B[q] <= -N[q]) {
    B[q] = B[q] + N[q];
    if(C[q] > MIN_C)
      C[q] = C[q] - 1;
    if(B[q] <= -N[q])
      B[q] = -N[q] +1;
  }
  else if(B[q] > 0) {
    B[q] = B[q] - N[q];
    if(C[q] < MAX_C)
      C[q] = C[q] + 1;
    if(B[q] > 0)
      B[q] = 0;
  }

  out_buf[x.x][x.y] = Rx;
  return;
}
