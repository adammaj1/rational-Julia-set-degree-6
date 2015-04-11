/*

  Adam Majewski
  adammaj1 aaattt o2 dot pl  // o like oxygen not 0 like zero 
  fraktal.republika.pl

  c console progam 

ON THURSTON’S PULLBACK MAP by XAVIER BUFF, ADAM EPSTEIN, SARAH KOCH, AND KEVIN PILGRIM
see figure 4 on the page 19

How to compute iteration : 
z:x+y*%i;
 z1:z^2*(3-z^4)/2;
 realpart(z1);
 ((x^2−y^2)*(−y^4+6*x^2*y^2−x^4+3)−2*x*y*(4*x*y^3−4*x^3*y))/2
 imagpart(z1);
 (2*x*y*(−y^4+6*x^2*y^2−x^4+3)+(x^2−y^2)*(4*x*y^3−4*x^3*y))/2 

a= (x^2−y^2)
b=(−y^4+6*x^2*y^2−x^4+3)
c= 2*x*y
d= (4*x*y^3−4*x^3*y)

so 

re(z1) = (a*b-c*d)/2
im(z1) = (c*b+a*d)/2

there are 2 finite superattracting fixed points :
za=0
zb=1

aaa



 



 

  gcc r.c -lm -Wall -march=native 
  time ./a.out

  m
*/

#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // strcat
#include <math.h> // M_PI; needs -lm also 
#include <complex.h>

/* --------------------------------- global variables and consts ------------------------------------------------------------ */



// radius of the target set ( circle around alfa fixed point ); it is related with iHeight
// so changing iHeight needs change of iMaxDistance2fixed
#define iMaxDistance2fixed  10 // distance point to alfa fixed point in pixels  150 when iHeight=1000; 280 when iHeight=2000
int iMaxDistance2fixed2;
double dMaxDistance2fixed2; // = (iMaxDistance2fixed*PixelWidth)^2
double dMaxDistance2fixed;

// virtual 2D array and integer ( screen) coordinate
// Indexes of array starts from 0 not 1 
//unsigned int ix, iy; // var
static unsigned int ixMin = 0; // Indexes of array starts from 0 not 1
static unsigned int ixMax ; //
static unsigned int iWidth ; // horizontal dimension of array

static unsigned int iyMin = 0; // Indexes of array starts from 0 not 1
static unsigned int iyMax ; //

static unsigned int iHeight = 4000; //  
// The size of array has to be a positive constant integer 
static unsigned int iSize ; // = iWidth*iHeight; 

// memmory 1D array 

unsigned char *data;
unsigned char *edge;

// unsigned int i; // var = index of 1D array
//static unsigned int iMin = 0; // Indexes of array starts from 0 not 1
static unsigned int iMax ; // = i2Dsize-1  = 
// The size of array has to be a positive constant integer 
// unsigned int i1Dsize ; // = i2Dsize  = (iMax -iMin + 1) =  ;  1D array with the same size as 2D array

/* world ( double) coordinate = dynamic plane */
static   const double ZxMin=-1.8;
static  const double ZxMax=1.8;
static  const double ZyMin=-1.8;
static  const double ZyMax=1.8;
static  double PixelWidth; // =(ZxMax-ZxMin)/ixMax;
static  double PixelHeight; // =(ZyMax-ZyMin)/iyMax;
static  double ratio ;
 

// complex numbers of parametr plane 
double Cx; // c =Cx +Cy * i
double Cy;
double complex c; // parameter of function fc(z)=z^2 + c


static unsigned long int iterMax  = 1000; //iHeight*100;

static double ER = 2.0; // Escape Radius for bailout test 
static double ER2;

/* colors = shades of gray from 0 to 255 */
// 8 bit color = int number from 0 to 255
unsigned char iColorsOfInterior[2]={200,230}; // NumberOfPetal of colors = iPeriodChild
static unsigned char iColorOfExterior = 245;
static unsigned char iColorOfUnknown = 100;

long int iUknownPixels=0;



/* ------------------------------------------ functions -------------------------------------------------------------*/



      

//------------------complex numbers -----------------------------------------------------



// from screen to world coordinate ; linear mapping
// uses global cons
double GiveZx(unsigned int ix)
{ return (ZxMin + ix*PixelWidth );}

// uses globaal cons
double GiveZy(unsigned int iy)
{ return (ZyMax - iy*PixelHeight);} // reverse y axis

/* -----------  array functions = drawing -------------- */

/* gives position of 2D point (ix,iy) in 1D array  ; uses also global variable iWidth */
unsigned int Give_i(unsigned int ix, unsigned int iy)
{ return ix + iy*iWidth; }

// plots raster point (ix,iy) 
int iDrawPoint(unsigned char A[], unsigned int ix, unsigned int iy, unsigned char iColor)
{ 

  /* i =  Give_i(ix,iy) compute index of 1D array from indices of 2D array */
  A[Give_i(ix,iy)] = iColor;

  return 0;
}

// draws point to memmory array data
// uses complex type so #include <complex.h> and -lm 
int dDrawPoint(unsigned char A[], complex double point,unsigned char iColor )
{

  unsigned int ix, iy; // screen coordinate = indices of virtual 2D array
  //unsigned int i; // index of 1D array
  
  ix = (creal(point)- ZxMin)/PixelWidth; 
  iy = (ZyMax - cimag(point))/PixelHeight; // inverse Y axis 
  iDrawPoint(A, ix, iy, iColor);
  return 0;
}

 





//;;;;;;;;;;;;;;;;;;;;;;  setup ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

int setup()
{

  
 
  
  
  printf("setup\n");
 
    /* 2D array ranges */
  
  iWidth = iHeight;
  iSize = iWidth*iHeight; // size = number of points in array 
  // iy
  iyMax = iHeight - 1 ; // Indexes of array starts from 0 not 1 so the highest elements of an array is = array_name[size-1].
  //ix
  
  ixMax = iWidth - 1;

  /* 1D array ranges */
  // i1Dsize = i2Dsize; // 1D array with the same size as 2D array
  iMax = iSize-1; // Indexes of array starts from 0 not 1 so the highest elements of an array is = array_name[size-1].

  /* Pixel sizes */
  PixelWidth = (ZxMax-ZxMin)/ixMax; //  ixMax = (iWidth-1)  step between pixels in world coordinate 
  PixelHeight = (ZyMax-ZyMin)/iyMax;
  ratio = ((ZxMax-ZxMin)/(ZyMax-ZyMin))/((float)iWidth/(float)iHeight); // it should be 1.000 ...
  
  

  // for numerical optimisation in iteration
  ER2 = ER * ER;
  iMaxDistance2fixed2 =iMaxDistance2fixed * iMaxDistance2fixed;
  dMaxDistance2fixed2 = iMaxDistance2fixed2*PixelWidth*PixelWidth; // dMaxDistance2fixed^2
  dMaxDistance2fixed = sqrt(dMaxDistance2fixed2); // maybe it should be in reversed order ??
    
  
  /* create dynamic 1D arrays for colors ( shades of gray ) */
  data = malloc( iSize * sizeof(unsigned char) );
  edge = malloc( iSize * sizeof(unsigned char) );

  if (edge== NULL || data == NULL)
    {
      fprintf(stderr," Could not allocate memory");
      getchar(); 
      return 1;
    }

  

 
  

  
   
  
  printf(" end of setup \n");
  
  return 0;

} // ;;;;;;;;;;;;;;;;;;;;;;;;; end of the setup ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

unsigned char ComputeColor(unsigned int ix, unsigned int iy, int IterationMax)
{ 
  // check behavour of z under fc(z)=z^2+c
  // using 2 target set:
  // 1. exterior or circle (center at origin and radius ER ) 
  // as a target set containing infinity = for escaping points ( bailout test)
  // for points of exterior of julia set
  // 2. interior of circle with center = alfa and radius dMaxDistance2fixed
  // as a target set for points of interior of Julia set 
  //  Z= Zx+ZY*i;

  double Zx2, Zy2;
  int i=0;
  //int j; // iteration = fc(z)
  double d2 ; /* d2= (distance from z to zb)^2   */
  double Zxt,Zyt ; // 
  double Zx, Zy;
  double a,b,c,d; // temporary variables
  int iDistance;
  
  
  // from screen to world coordinate 
  Zx = GiveZx(ix);
  Zy = GiveZy(iy);
  /* distance from z to zb=1  */
  Zxt=Zx-1.0;
  Zyt=Zy;
  d2=Zxt*Zxt +Zyt*Zyt;
  if (d2<dMaxDistance2fixed2) 
    {
      iDistance = (int)(sqrt(d2)/PixelWidth);
      if (iDistance<iMaxDistance2fixed)
	{ 
	  return  iColorsOfInterior[1];
	}
    }
    
   
   /* distance from z to za=0  */
  d2=Zx*Zx +Zy*Zy;
  if (d2<dMaxDistance2fixed2) 
    {
      iDistance = (int)(sqrt(d2)/PixelWidth);
      if (iDistance<iMaxDistance2fixed)
	{ 
	  return  iColorsOfInterior[0];
	}
    }
    
  // if not inside target set around 
  while (1 )
    { // then iterate 
      
      Zx2 = Zx*Zx; 
      Zy2 = Zy*Zy;
       
      // bailout test 
      if (Zx2 + Zy2 > ER2) return iColorOfExterior; // if escaping stop iteration
       
      // if not escaping or not attracting then iterate = check behaviour
      // new_z  = f(z) = z^2*(3-z^4)/2 
      a= Zx*Zx-(Zy*Zy);
      b=-(Zy*Zy*Zy*Zy)+6*Zx*Zx*Zy*Zy-(Zx*Zx*Zx*Zx)+3;
      c= 2*Zx*Zy;
      d= 4*Zx*Zy*Zy*Zy-4*Zx*Zx*Zx*Zy;
      Zx = (a*b-c*d)/2;
      Zy = (c*b+a*d)/2;
      //
      i+=1;
	 
     /* distance from z to zb=1  */
  Zxt=Zx-1.0;
  Zyt=Zy;
  d2=Zxt*Zxt +Zyt*Zyt;
  if (d2<dMaxDistance2fixed2) 
    {
      iDistance = (int)(sqrt(d2)/PixelWidth);
      if (iDistance<iMaxDistance2fixed)
	{ 
	  return  iColorsOfInterior[1];
	}
    }
    
   /* distance from z to za=0  */
  d2=Zx*Zx +Zy*Zy;
  if (d2<dMaxDistance2fixed2) 
    {
      iDistance = (int)(sqrt(d2)/PixelWidth);
      if (iDistance<iMaxDistance2fixed)
	{ 
	  return  iColorsOfInterior[0];
	}
    }
    
      if (i > IterationMax) break;
      
      
    }

  // pixel is not escaping to infinity or not attracting to fixed attractore : 
  // change parameters : iterMax, distance ...
  iUknownPixels+=1;
  return  iColorOfUnknown ;   //
}

// plots raster point (ix,iy) 
int PlotPoint(unsigned char A[] , unsigned int ix, unsigned int iy, int IterationMax)
{
  unsigned i; /* index of 1D array */
  unsigned char iColor;
  

  i = Give_i(ix,iy); /* compute index of 1D array from indices of 2D array */
  iColor = ComputeColor(ix, iy, IterationMax);
  A[i] = iColor;

  return 0;
}

// fill array 
// uses global var :  ...
// scanning complex plane 
int ComputeFatouComponents(unsigned char A[], int IterationMax )
{
  unsigned int ix, iy; // pixel coordinate 

  printf("compute image \n");
  // for all pixels of image 
  for(iy = iyMin; iy<=iyMax; ++iy) 
    { printf(" %d z %d\n", iy, iyMax); //info 
      for(ix= ixMin; ix<=ixMax; ++ix) PlotPoint(A, ix, iy, IterationMax ) ; //  
    } 
   
  return 0;
}

int ComputeBoundariesIn(unsigned char A[])
{
 
  unsigned int iX,iY; /* indices of 2D virtual array (image) = integer coordinate */
  unsigned int i; /* index of 1D array  */
  /* sobel filter */
  unsigned char G, Gh, Gv; 
  // boundaries are in edge array ( global var )
 
 
 
 
  printf(" find boundaries in A array using  Sobel filter\n");   
  // #pragma omp parallel for schedule(dynamic) private(i,iY,iX,Gv,Gh,G) shared(iyMax,ixMax, ER2)
  for(iY=1;iY<iyMax-1;++iY){ 
    for(iX=1;iX<ixMax-1;++iX){ 
      Gv= A[Give_i(iX-1,iY+1)] + 2*A[Give_i(iX,iY+1)] + A[Give_i(iX-1,iY+1)] - A[Give_i(iX-1,iY-1)] - 2*A[Give_i(iX-1,iY)] - A[Give_i(iX+1,iY-1)];
      Gh= A[Give_i(iX+1,iY+1)] + 2*A[Give_i(iX+1,iY)] + A[Give_i(iX-1,iY-1)] - A[Give_i(iX+1,iY-1)] - 2*A[Give_i(iX-1,iY)] - A[Give_i(iX-1,iY-1)];
      G = sqrt(Gh*Gh + Gv*Gv);
      i= Give_i(iX,iY); /* compute index of 1D array from indices of 2D array */
      if (G==0) {edge[i]=255;} /* background */
      else {edge[i]=0;}  /* boundary */
    }
  }
 
   
 
  return 0;
}

int CopyBoundariesTo(unsigned char A[])
{
 
  unsigned int iX,iY; /* indices of 2D virtual array (image) = integer coordinate */
  unsigned int i; /* index of 1D array  */
 
 
  printf("copy boundaries from edge array to data array \n");
  for(iY=1;iY<iyMax-1;++iY)
    for(iX=1;iX<ixMax-1;++iX)
      {i= Give_i(iX,iY); if (edge[i]==0) A[i]=0;}
 
 
 
  return 0;
}



 
 
int DrawCriticalOrbit(unsigned char A[], unsigned int IterMax)
{
  // integer = pixel coordinate
  unsigned int ix, iy; 
  // double = world coordinate
  // critical point Z= Zx+ZY*i;
  double Zx=0.0;
  double Zy=0.0; 
  double Zx2=0.0;
  double Zy2=0.0;
  
  unsigned int i; /* index of 1D array */
  unsigned int j; // number of iteration
 
 
  // draw critical point  
  ix = (int)((Zx-ZxMin)/PixelWidth);
  iy = (int)((ZyMax-Zy)/PixelHeight); // reverse y axis
  i = Give_i(ix, iy); /* compute index of 1D array from indices of 2D array */
  A[i]=255-A[i]; 
 
  // iterate
  for (j = 1; j <= IterMax; j++) //larg number of iteration s
    {  Zx2 = Zx*Zx; 
      Zy2 = Zy*Zy;
 
      // bailout test 
      if (Zx2 + Zy2 > ER2) return iColorOfExterior; // if escaping stop iteration
 
      // if not escaping iterate
      // Z(n+1) = Zn * Zn  + C
      Zy = 2*Zx*Zy + Cy; 
      Zx = Zx2 - Zy2 + Cx;
      //compute integer coordinate  
      ix = (int)((Zx-ZxMin)/PixelWidth);
      iy = (int)((ZyMax-Zy)/PixelHeight); // reverse y axis
      i = Give_i(ix, iy); /* compute index of 1D array from indices of 2D array */
      A[i]=255-A[i];   // mark the critical orbit
 
    }
  return 0;
}

// Check Orientation of image : mark first quadrant 
// it should be in the upper right position
// uses global var :  ...
int CheckOrientation(unsigned char A[] )
{
  unsigned int ix, iy; // pixel coordinate 
  double Zx, Zy; //  Z= Zx+ZY*i;
  unsigned i; /* index of 1D array */
  for(iy=iyMin;iy<=iyMax;++iy) 
    {
      Zy = GiveZy(iy);
      for(ix=ixMin;ix<=ixMax;++ix) 
	{

	  // from screen to world coordinate 
	  Zx = GiveZx(ix);
	  i = Give_i(ix, iy); /* compute index of 1D array from indices of 2D array */
	  if (Zx>0 && Zy>0) A[i]=255-A[i];   // check the orientation of Z-plane by marking first quadrant */

	}
    }
   
  return 0;
}

 

// save "A" array to pgm file 
int SaveArray2PGMFile( unsigned char A[], double k)
{
  
  FILE * fp;
  const unsigned int MaxColorComponentValue=255; /* color component is coded from 0 to 255 ;  it is 8 bit color file */
  char name [30]; /* name of file */
  sprintf(name,"%.0f", k); /*  */
  char *filename =strcat(name,".pgm");
  char *comment="# Numerical approximation of  Julia set for f(z)= z^2*(3-z^4)/2; Adam Majewski";/* comment should start with # */

  /* save image to the pgm file  */      
  fp= fopen(filename,"wb"); /*create new file,give it a name and open it in binary mode  */
  fprintf(fp,"P5\n %s\n %u %u\n %u\n",comment,iWidth,iHeight,MaxColorComponentValue);  /*write header to the file*/
  fwrite(A,iSize,1,fp);  /*write A array to the file in one step */
  printf("File %s saved. \n", filename);
  fclose(fp);

  return 0;
}

int info()
{
  // diplay info messages
  printf("Numerical approximation of  Julia set for f(z)= z^2*(3-z^4)/2; \n");
  printf("Image Width = %f \n", ZxMax-ZxMin);
  printf("PixelWidth = %f \n", PixelWidth);
  printf("size of target set in screen units = iMaxDistance2fixed  = %d pixels \n", iMaxDistance2fixed); 
  printf("size of target set in world units = dMaxDistance2fixed  = %f ; \n", dMaxDistance2fixed);
  printf("Maximal number of iterations = iterMax = %ld \n", iterMax);
  printf("ratio of image  = %f ; it should be 1.000 ...\n", ratio);
  printf("Unknown pixels = %ld ; it should be 0 ...\n", iUknownPixels);
  return 0;
}

/* -----------------------------------------  main   -------------------------------------------------------------*/
int main()
{
  setup();
 

 
   
 
  ComputeFatouComponents(data, iterMax);
  SaveArray2PGMFile( data, iHeight+0); // save array data (components of Fatou set ) to pgm file

  ComputeBoundariesIn(data);
  SaveArray2PGMFile( edge, iHeight+1); // save array edge (Julia set ) to pgm file

  

  CopyBoundariesTo(data);
  SaveArray2PGMFile( data, iHeight+2); // save array data (Julia set and components ) to pgm file

  

  
  DrawCriticalOrbit(data, 100000);
  SaveArray2PGMFile(data, iHeight+3); // save array rays ( = rays + axis + trap + critical orbit) to pgm file

  

  printf(" allways free memory  to avoid buffer overflow \n");
  free(data);
  free(edge);

  
  info();

  return 0;
}
