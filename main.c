#include <stdio.h>
#include <stdlib.h>
#define SZINTERPOLATE 4
#define OUTPUTMATRIX 1
int sizeX=8;
int sizeY=8;

float cubicInterpolate (float *p, float x) 
{
  return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
};

void vectorInterpolate(float *iv, float x, int length, float *res)
{
 // Interpolates numbers in between the values of iv use 
 // cubicInterpolate at x (usually x=0.5)
 // Length should be the number of elements in iv
 // res is a pointer to an array (malloc allocated) for reporting
 // the answer
float p[SZINTERPOLATE];

  // for (int i=0; i<sizeX; i++)
  //   printf("%f \n", iv[i]);


 for (int c=0; c<length-1; c++){   
   // Interpolate from 0 to one elem before the number of columns vs-1
     // Populate input vector of length 4.
     // If in position j=0 duplicate first element
     // If in position j=sizeY-2 duplicate last element
     if (c==0){
       p[0]=iv[0];
       p[1]=iv[0];
       p[2]=iv[1];
       p[3]=iv[2];
     }
     else if(c==length-2) {
       p[0]=iv[c-1];
       p[1]=iv[c];
       p[2]=iv[c+1];
       p[3]=iv[c+1];
       
     }
     else {
       p[0]=iv[c-1];
       p[1]=iv[c];
       p[2]=iv[c+1];
       p[3]=iv[c+2];
     }
     
     res[c]= cubicInterpolate(p, 0.5);
    } // end for int c

  // for (int i=0; i<sizeX-1; i++)
  //   printf("res %f \n", res[i]);
 

 
}

void matrixBicubicInterpolation(float *elem, int sizeX, int sizeY, float* iphv)
{
 float *res;
 res = (float*) malloc((sizeX-1)*sizeof(float));
 // Returns a matrix of size (2*sizeX-1) x (2*sizeY-1) with bicubic interpolation
 float *iphoriz=(float *) malloc((2*sizeX-1)*sizeY*sizeof(float)); 
 // Array with original values and horizontal interpolated values
 //float[] iphv = new float[(2*sizeX-1)*(2*sizeY-1)]; 
 // Array with original and horizontal and vertical interpolated values
 float *iv = (float *) malloc(sizeX*sizeof(float)); 
 // input vector extracted from elem
 
 // Horizontal interpolation
  for (int row=0; row<sizeY; row++){
   for (int col=0; col<sizeX; col++)
     {
       iv[col]=elem[row*sizeX+col]; // Populate input vector for interpolation
       iphoriz[row*(2*sizeX-1)+(col*2)]=iv[col]; //populate iphoriz with original values
     }

   vectorInterpolate(iv, 0.5, sizeX, res);
   for (int col=0; col<sizeX-1; col++)
      iphoriz[row*(2*sizeX-1)+(2*col+1)]=res[col]; //populate iphoriz with interpolate values
  } // end for int row=0
  
    
  // Take original + horizontal matrix and interpolate vertical values
  for (int col=0; col<(2*sizeX-1); col++){
    
   for (int row=0; row<sizeY; row++)
     {
       iv[row]=iphoriz[row*(2*sizeX-1)+col]; // Populate input vector for interpolation
       iphv[row*( (4*sizeX) - 2 )+(col)]=iv[row]; //populate iphv with  values from iphoriz
     }

   vectorInterpolate(iv, 0.5,sizeY,res);

   for (int row=0; row<sizeY-1; row++)
      iphv[(row*2+1)*(2*sizeY-1)+(col)]=res[row]; //populate iphv with interpolate values
      
  } // end for int col=0
  

  free(res);
  free(iphoriz);
  free(iv);
}

int main(void) {

  float q[64]={21.00,20.25,20.00,20.00,20.25,20.75,20.75,21.00,21.00,21.00,20.50,21.00,20.50,21.25,21.50,21.75,20.75,20.25,20.25,20.50,21.00,20.75,21.00,22.00,20.75,20.50,21.00,20.75,20.50,20.75,20.75,21.75,20.50,21.00,20.25,20.50,21.00,20.75,21.00,21.25,20.75,21.75,20.00,21.50,21.25,20.50,21.00,21.75,21.00,21.25,21.00,20.75,21.00,21.25,21.75,22.00,21.00,21.25,21.75,22.00,21.50,21.25,21.75,22.25};
  if(OUTPUTMATRIX){
    printf("--- Original Array ---\n");
    for(int i=0; i<sizeY; i++){
      for (int j=0; j<sizeX; j++)
        printf("%lf ", q[i*sizeY+j]);
    printf("\n");
    }
  }


  float *res;
  res = (float *) malloc( (2*sizeX-1)*(2*sizeY-1)*sizeof(float));
  matrixBicubicInterpolation(q, sizeX, sizeY, res);
 
  if (OUTPUTMATRIX){
    printf("--- Bicubic Interpolation Array  ---\n");
    for(int row=0; row<(2*sizeY-1); row++){
      for (int col=0; col<(2*sizeX-1); col++)
        printf("%d %lf ",row*(2*sizeX-1)+col, res[row*(2*sizeX-1)+col]);
      printf("\n");
    }
  }

  free(res);

}


