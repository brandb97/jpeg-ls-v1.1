/*++*
* Copyright (c) 1997 University of British Columbia. All rights reserved.
*
*       File:           prototypes.h
*       Pourpose:       contains the functional prototypes for typechecking.
*
*       Author:         Ismaeil R. Ismaeil, Aug. 1997
*
*--*/

void RegularModeProcessing();
int  Quantize();
int  ModRange();
void GolombCoding();
void RunModeProcessing();
void GetNextSample();
void AppendToBitStream();
int  ComputeRx();
void SetSample();
int  GolombDecoding();

void WriteSoi();
void WriteEoi();
void WriteSof();
void WriteSos();
void ReadSoi();
void ReadSof();
void ReadSos();
