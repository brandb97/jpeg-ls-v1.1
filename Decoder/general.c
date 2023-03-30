/*++*
 * Copyright (c) 1997 University of British Columbia. All rights reserved.
 *
 *       File:           general.c
 *       Pourpose:       contains the general decoding routins (eg. Golomb decode).
 *
 *       Author:         Ismaeil R. Ismaeil, Aug. 1997
 *
 *--*/

#include "globals.h"
#include "prototypes.h"

void SetSample()
{
  if(x.y >= dim.y){
    x.x++;
    x.y = 1;}
  else
    x.y++;

  a.x = x.x;
  a.y = c.y = x.y-1;
  b.x = c.x = d.x = x.x-1;
  b.y = x.y;
  d.y = x.y+1;
 
  Rb = out_buf[b.x][b.y];

  if(x.y == 1 && x.x > 1) {
    Ra = Rb;
    Rc = out_buf[b.x-1][b.y];
    Rd = out_buf[d.x][d.y];
  }

  else if(x.y == dim.y) {
    Ra = out_buf[a.x][a.y];
    Rc = out_buf[c.x][c.y];
    Rd = Rb;
  }
  else {
    Ra = out_buf[a.x][a.y];
    Rc = out_buf[c.x][c.y];
    Rd = out_buf[d.x][d.y];
  }

}

int GolombDecoding(int l, int limit)
{
  int val1 = 0;
  int val2 = 0;
  int val3;
  int i;
 
  val3 = limit-qbpp-1;

  while(GetFromBitStream() != 1)
    val1 +=1;
 
  if(val1<val3){
    val2 = val1 << l;
    for(i=1; i<=l; i++)
      val2 += GetFromBitStream() << l-i;
  }
  else {
    for(i=1; i<=qbpp; i++)
      val2 += GetFromBitStream() << qbpp-i;
    val2 += 1;
  }

  return val2;

}
