/*++*
 * Copyright (c) 1997 University of British Columbia. All rights reserved.
 *
 *       File:           marker.c
 *       Pourpose:       contains the marker Library which used to set the header bytes...
 *
 *       Author:         Ismaeil R. Ismaeil, Aug. 1997
 *
 *--*/

#include "globals.h"
#include "marker.h"
#include "prototypes.h"

/**********************************************
WriteSoi() puts an SOI marker onto the stream.
**********************************************/

void WriteSoi()
{

  putc(MARKER_MARKER,ofp);
  putc(MARKER_SOI,ofp);
}

/**********************************************
WriteEoi() puts an EOI marker onto the stream.
***********************************************/

void WriteEoi()
{

  putc(MARKER_MARKER,ofp);
  putc(MARKER_EOI,ofp);
}

/***********************************************
WriteSof() puts an SOF marker onto the stream.
***********************************************/

void WriteSof()
{

  putc(MARKER_MARKER,ofp);
  putc(MARKER_SOF,ofp);
  putc(0,ofp);                      /* Put a 0 down onto the stream. */
  putc(0x0b,ofp);
  putc(P,ofp);
  putc(dim.x>>8,ofp); 
  putc(0x00ff&dim.x,ofp);
  putc(dim.y>>8,ofp); 
  putc(0x00ff&dim.y,ofp);
  putc(1,ofp);
  putc(1,ofp);
  putc(0x11,ofp);
  putc(0,ofp);
}


/********************************************
WriteSos() writes a start of scan marker.
*********************************************/

void WriteSos()
{

  putc(MARKER_MARKER,ofp);
  putc(MARKER_SOS,ofp);
  putc(0,ofp);
  putc(8,ofp);
  putc(1,ofp);
  putc(1,ofp);
  putc(0,ofp);
  putc(0,ofp);
  putc(0,ofp);
  putc(0,ofp);
}


/*************************************************
  ReadSoi reads a start of image from the stream.
**************************************************/

void ReadSoi()
{
  if (getc(ifp) != MARKER_MARKER){
    fprintf(stderr,"Can not recognize the header Marker SOI\n");
    exit(0);
  }

  if (getc(ifp) != MARKER_SOI){
    fprintf(stderr,"Can not recognize the header SOI\n");
    exit(0);
  }
}

/**************************************************************************
  ReadSof() reads a start of frame marker from the stream. We assume that
  the first two bytes (marker prefix) have already been stripped.
**************************************************************************/  

void ReadSof()
{

  if (getc(ifp) != MARKER_MARKER){
    fprintf(stderr,"Can not recognize the header SOF\n");
    exit(0);
  }
  if (getc(ifp) != MARKER_SOF){
    fprintf(stderr,"Can not recognize the header SOF\n");
    exit(0);
  }

  if (getc(ifp) != 0){
    fprintf(stderr,"Error reading header SOF\n");
    exit(0);
  }

  if (getc(ifp) != 0x0b){
    fprintf(stderr,"Error reading header SOF\n");
    exit(0);
  }

  P = getc(ifp);

  dim.x  = getc(ifp) << 8;
  dim.x += getc(ifp);
  dim.y  = getc(ifp) << 8;
  dim.y += getc(ifp);

  if (getc(ifp) != 1){
    fprintf(stderr,"Error reading header SOF\n");
    exit(0);
  }
  if (getc(ifp) != 1){
    fprintf(stderr,"Error reading header SOF\n");
    exit(0);
  }
  if (getc(ifp) != 0x11){
    fprintf(stderr,"Error reading header SOF\n");
    exit(0);
  }
  if (getc(ifp) != 0){
    fprintf(stderr,"Error reading header SOF\n");
    exit(0);
  }

}

/*****************************************************************
ReadSos() reads in a start of scan from the stream. The first two
bytes should have been stripped off.
*****************************************************************/

void ReadSos()
{
  if (getc(ifp) != MARKER_MARKER){
    fprintf(stderr,"Can not recognize the header SOS\n");
    exit(0);
  }
  if (getc(ifp) != MARKER_SOS){
    fprintf(stderr,"Can not recognize the header SOS\n");
    exit(0);
  }

  if (getc(ifp) != 0){
    fprintf(stderr,"Error reading header SOS\n");
    exit(0);
  }

  if (getc(ifp) != 8){
    fprintf(stderr,"Error reading header SOS\n");
    exit(0);
  }
  if (getc(ifp) != 1){
    fprintf(stderr,"Error reading header SOS\n");
    exit(0);
  }

  if (getc(ifp) != 1){
    fprintf(stderr,"Error reading header SOS\n");
    exit(0);
  }

  if (getc(ifp) != 0){
    fprintf(stderr,"Error reading header SOS\n");
    exit(0);
  }

  if (getc(ifp) != 0){
    fprintf(stderr,"Error reading header SOS\n");
    exit(0);
  }
  if (getc(ifp) != 0){
    fprintf(stderr,"Error reading header SOS\n");
    exit(0);
  }

  if (getc(ifp) != 0){
    fprintf(stderr,"Error reading header SOS\n");
    exit(0);
  }

}
