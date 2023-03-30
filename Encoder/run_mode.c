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
  int glimit;

  /* Scanning */

  RUNval = Ra;
  RUNcnt = 0;

  while(Ix == RUNval){
    RUNcnt += 1;
    Rx = RUNval;
    if(EOLine == 1)
      break;
    else
      GetNextSample();
  }

  /* Encoding of run segments of length rm */

  while(RUNcnt >= (1<<J[RUNindex])){
    AppendToBitStream(1,1);
    RUNcnt -= (1<<J[RUNindex]);
    if(RUNindex < 31){
      RUNindex_prev = RUNindex;
      RUNindex++;
    }
  }

  /* Encoding of run segments of length less than rm */

  if(Ra != Ix){
    AppendToBitStream(0,1);
    AppendToBitStream(RUNcnt, J[RUNindex]);
    if(RUNindex>0){
      RUNindex_prev = RUNindex;
      RUNindex = RUNindex - 1;
    }
  }
  else if(RUNcnt > 0)
    AppendToBitStream(1,1);

  /* Run interruption sample encoding */
 
  if(Ra != Ix){

    /* Index computation */

    if(Ra == Rb)
      RItype = 1;
    else
      RItype = 0;
 
    /* Prediction error for a run interruption sample */

    if(RItype == 1)
      Errval = Ix - Ra;
    else
      Errval = Ix - Rb;

    /* Error computation for a run interruption sample */

    if((RItype == 0) && (Ra>Rb)){
      Errval = -Errval;
      SIGN = -1;
    }
    else
      SIGN = 1;

    if(NEAR > 0){
      Errval = Quantize(Errval);
      Rx = ComputeRx();
    }
    else
      Rx = Ix;

    Errval = ModRange(Errval, RANGE);
 
    /* Computation of auxiliary variable in Golomb parameter */
  
    if(RItype == 0)
      TEMP = A[365];
    else
      TEMP = A[366]+(N[366]>>1);
 
    q = RItype + 365;

    for(k=0; (N[q]<<k)<TEMP; k++);

    /* Computation of map for Errval mapping */

    if((k==0) && (Errval>0) && (2*Nn[q]<N[q]))
      map = 1;
    else if((Errval < 0) && (2*Nn[q] >= N[q]))
      map = 1;
    else if((Errval < 0) && (k != 0))
      map = 1;
    else 
      map = 0;
  
    /* Errval mapping for run interuption sample */
  
    EMErrval = 2*(ABS(Errval)) - RItype - map;

    /* Mapped-error encoding */

    glimit = LIMIT - J[RUNindex_prev] - 1;

    GolombCoding(EMErrval, k, glimit); 

    /* update of parameters for run interruption sample */

    if(Errval < 0)
      Nn[q] = Nn[q] + 1;
    A[q] = A[q] + ((EMErrval+1-RItype)>>1);
    if(N[q] == RESET){
      A[q] = A[q]>>1;
      N[q] = N[q]>>1;
      Nn[q] = Nn[q]>>1;
    }
    N[q] = N[q] + 1;
  }

  return;
}
