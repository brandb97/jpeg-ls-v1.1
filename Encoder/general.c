/*++*
 * Copyright (c) 1997 University of British Columbia. All rights reserved.
 *
 *       File:           general.c
 *       Pourpose:       contains the general encoding routins (eg. Golomb encode).
 *
 *       Author:         Ismaeil R. Ismaeil, Aug. 1997
 *
 *--*/


#include "globals.h"
#include "prototypes.h"

/********************************************************************
 Error quantization for near-lossless mode, and reconstructed value 
********************************************************************/

int Quantize(int buff)
{

  if(buff > 0)
    buff = (buff+NEAR)/(2*NEAR+1);
  else
    buff = -(NEAR-buff)/(2*NEAR+1);
  return buff;
}

/**************************************
Operation for the near lossless mode.
**************************************/

int ComputeRx()
{
  Rx = Px + SIGN*Errval*(2*NEAR+1);
  if(Rx < 0)
    Rx = 0;
  else if(Rx>MAXVAL)
    Rx = MAXVAL;
  return Rx;
}

/*************************************
Modulo reduction of the error
*************************************/

int ModRange(int buff, int range)
{
  if(buff < 0)
    buff = buff + range;
  if(buff >= ((range+1)/2))
    buff = buff - range;
  return buff;
}

/************************************
Gets the next sample to be encoded 
************************************/

void GetNextSample()
{
  count++;
  if(x.y == dim.y){
    x.x++;
    x.y = 1;}
  else
    x.y++;

  a.x = x.x;
  a.y = c.y = x.y-1;
  b.x = c.x = d.x = x.x-1;
  b.y = x.y;
  d.y = x.y+1;

  Ix = in_buf[x.x][x.y];
  Ra = in_buf[a.x][a.y];
  Rb = in_buf[b.x][b.y];
  Rc = in_buf[c.x][c.y];
  Rd = in_buf[d.x][d.y];

  if(x.y == dim.y)
    EOLine = 1;
  else
    EOLine = 0;
}

/***************************************
Golomb encode the maped error values  
***************************************/

void GolombCoding(int n, int l, int limit)
{
  int val1;
  int val2;
  int val3;
  int val4;

  val1 = n >> l;
  val2 = val1 << l;
  val3 = limit-qbpp-1;
 
  if(val1<val3){
    AppendToBitStream(0,val1);
    AppendToBitStream(1,1);
    val4 = n - val2;
    AppendToBitStream(val4,l);
  }
  else{
    AppendToBitStream(0,val3);
    AppendToBitStream(1,1);
    AppendToBitStream(n-1,qbpp);
  } 
}
