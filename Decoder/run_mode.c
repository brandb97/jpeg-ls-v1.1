/*++*
 * Copyright (c) 1997 University of British Columbia. All rights reserved.
 *
 *       File:           regular_mode.c
 *       Pourpose:       contains the main routins used in the run mode.
 *
 *       Author:         Ismaeil R. Ismaeil, Aug. 1997
 *
 *--*/

#include "globals.h"
#include "prototypes.h"

static int J[] = {0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,
                  4,5,5,6,6,7,7,8,9,10,11,12,13,14,15};

static int RUNindex_prev;

void RunModeProcessing()
{
  int R, i, cnt = 0, run_flag;
  int glimit;

  /* Scanning */

  for(;;){
    run_flag = 0;
    R = GetFromBitStream();
    if(R){
      if((dim.y-x.y)+1 >= (1<<J[RUNindex])){
        for(i=0; i<(1<<J[RUNindex]); i++)
          out_buf[x.x][x.y+i] = Ra;
        x.y += (1<<J[RUNindex]);
        if(RUNindex < 31){
          RUNindex_prev = RUNindex;
          RUNindex++;}
      }
      else{
        for(i=x.y; i<=dim.y; i++)
          out_buf[x.x][i] = Ra;
        x.y = dim.y;
        break;}
    }
    else{
      for(i=1; i<=J[RUNindex]; i++)
        cnt += GetFromBitStream()<<(J[RUNindex]-i);
      for(i=0; i<cnt; i++)
        out_buf[x.x][x.y+i] = Ra;
      x.y += cnt;
      if(RUNindex > 0){
        RUNindex_prev = RUNindex;
        RUNindex--;}
      run_flag = 1;
      break;
    }

    if(x.y > dim.y){
      x.y = dim.y;
      break;}
  }
 
  if(run_flag){

    /* Index computation */

    x.y--;
    SetSample();

    if(Ra == Rb)
      RItype = 1;
    else
      RItype = 0;
 
    /* Computation of auxiliary variable in Golomb parameter */
  
    if(RItype == 0)
      TEMP = A[365];
    else
      TEMP = A[366]+(N[366]>>1);
 
    q = RItype + 365;

    for(k=0; (N[q]<<k)<TEMP; k++);
 
    glimit = LIMIT - J[RUNindex_prev] - 1;
    EMErrval = GolombDecoding(k, glimit);

    if(EMErrval&1){
      if(RItype == 1)
        map = 0;
      else 
        map = 1;
      Errval = (EMErrval+1)>>1;
    }
    else{
      if(RItype == 1){
        Errval = (EMErrval+2)>>1;
        map = 1;}
      else{
        Errval = EMErrval>>1;
        map = 0;}}

    /* correct the sign of the map fun. */
    if((k == 0) && (map == 0) && (2*Nn[q] < N[q]))
      Errval = -Errval;
    else if((map == 1) && (2*Nn[q] >= N[q]))
      Errval = -Errval;
    else if((map == 1) && (k != 0))
      Errval = -Errval;
 
    /* Error computation for a run interruption sample */

    if((RItype == 0) && (Rb<Ra))
      Errval = -Errval;

    /* Adding prediction error for a run interruption sample */

    if(RItype == 1)
      Rx = (Errval + Ra)%(RANGE*(2*NEAR+1));
    else
      Rx = (Errval + Rb)%(RANGE*(2*NEAR+1));

    if(Rx < -NEAR)
      Rx = Rx + RANGE*(2*NEAR+1);
    else if (Rx > MAXVAL + NEAR)
      Rx = Rx - RANGE*(2*NEAR+1);

    if(Rx<0)
      Rx = 0;
    else if(Rx > MAXVAL)
      Rx = MAXVAL;

    out_buf[x.x][x.y] = Rx;
  
    /* update of parameters for run interruption sample */

    if(Errval < 0)
      Nn[q] = Nn[q] + 1;
    A[q] = A[q] + ((EMErrval+1-RItype)>>1);
    if(N[q] == RESET){
      A[q] = A[q]>>1;
      N[q] = N[q]>>1;
      Nn[q] = Nn[q]>>1;
    }
    N[q] = N[q] + 1; }

  return;
}
