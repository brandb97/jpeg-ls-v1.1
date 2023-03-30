/*++*
 * Copyright (c) 1997 University of British Columbia. All rights reserved.
 *
 *       File:           read_write.h 
 *       Pourpose:       contains the I/O functions.
 *
 *       Author:         Ismaeil R. Ismaeil, Aug. 1997
 *
 *--*/


/*************************************************
  2dmtx.c : Make matrix of given size nr x nc. 
           nr & nc : number of rows and cols
           size : sizeof(element_size)   
**************************************************/
char **dim2(int nr, int nc, unsigned size)
{
  int i;
  char **prow, *pdata;

  pdata = (char *) calloc(nr * nc,size);
  if (pdata == (char *) NULL)
  {
    fprintf(stderr,"No heap space for data\n");
    exit(1);
  }
  prow = (char **) malloc(nr * sizeof(char *));
  if (prow == (char **) NULL)
  {
    fprintf(stderr, "No heap space for row pointers\n");
    exit(1);
  }
  for (i = 0; i < nr ; i++)
  {
    prow[i] = pdata;       /* store pointers to rows */
    pdata += size * nc;    /* move to next row       */
  }
  return prow;             /* pointer to 2D array    */
}

/*********************************************
 free2.c: Free all elements of the matrix.
**********************************************/                                 
void free2c(pa)
     char **pa;
{
  free(*pa);                /* free the data */
  free(pa);                 /* free pointer to row pointers */
}
                                                                      
void free2i(pa)                                                       
     int **pa;                                                            
{                                                                     
  free(*pa);                /* free the data */                      
  free(pa);                 /* free pointer to row pointers */        
}                                                                    

void free2f(pa)
     float **pa;
{
  free(*pa);
  free(pa);
}

void free2d(pa)                                                       
     double **pa;                                                            
{ 
  free(*pa);                /* free the data */                      
  free(pa);                 /* free pointer to row pointers */        
}
                                                                      
/******************************************************************
 read_pic.c : This routine reads an image into frame buffer. 
               nl & nc : number of rows and cols      
               frame  : stored matrix (in memory)    
               fp : pointer to a file (image in)   
*******************************************************************/

void read_pic(FILE *fp, int nl, int nc, int **frame)
{
  unsigned char *c;
  int i,j;
   
  c = (unsigned char *) calloc(nc,sizeof(unsigned char));
    
  for (i = 1; i <= nl; i++)
    {
      fread(c,sizeof(unsigned char),nc,fp);
      for (j = 1; j <= nc; j++)
        {
          frame[i][j] = c[j-1];
        }
    }
  free(c);
} 

/*******************************************************************
 read_int_pic.c : This routine reads an two byte/pel image into 
                      frame buffer.
               nl & nc : number of rows and cols                    
               frame  : stored matrix (in memory)                   
               fp : pointer to a file (image in)                    
********************************************************************/

void read_int_pic(FILE *fp, int nl, int nc, int **frame)
{
  unsigned int *c;
  int i,j;

  c = (unsigned int *) calloc(nc,sizeof(unsigned int));

  for (i = 1; i <= nl; i++)
    {
      fread(c,sizeof(unsigned int),nc,fp);
      for (j = 1; j <= nc; j++)
        {
          frame[i][j] = c[j-1];
        }
    }
  free(c);
}

/*************************************************************
 write_pic.c : This routine save an image in the disk.           
                nl & nc : number of rows and cols                   
                frame : saved matrix (in disk)                       
                fp : pointer to a file (image out)                   
*************************************************************/

void write_pic(FILE *fp,  int nl, int nc, int **frame)
{
  unsigned char *c;
  int i,j;
 
  c = (unsigned char *) calloc(nc,sizeof(unsigned char));

  for (i = 1 ; i <= nl ; i++)
    {
      for (j = 1; j <= nc; j++)
        {
          if(frame[i][j] < 0)
            c[j-1] = 0;
          else if(frame[i][j] > 255)
            c[j-1] = 255;
          else
            c[j-1] = frame[i][j];
        }     
      fwrite(c,nc * sizeof(unsigned char),1,fp); 
   
    }
  free(c);
}

/************************************************************
 write_pic.c : This routine save an image in the disk. 
                nl & nc : number of rows and cols        
                frame : saved matrix (in disk)            
                fp : pointer to a file (image out)         
************************************************************/

void write_int_pic(FILE *fp, int nl, int nc, int **frame)
{
  unsigned int *c;
  int i,j;

  c = (unsigned int *) calloc(nc,sizeof(unsigned int));

  for (i = 1 ; i <= nl ; i++)
    {
      for (j = 1; j <= nc; j++)
        {
          c[j-1] = frame[i][j];
        }
      fwrite(c,sizeof(unsigned int),nc,fp);

    }
  free(c);
}
