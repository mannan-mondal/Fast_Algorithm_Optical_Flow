#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include<time.h>
#include<math.h>

#define   size_x  420 
#define   size_y  380

#define   dmax     10   
#define   k1       18   
#define   k2       18  
 
#define   w1       15
#define   w2       15


typedef struct
{
	  int imagesize_x, imagesize_y;
	  int **pixel;
}image_t;

image_t allocate_image(const int imagesize_x, const int imagesize_y);
int  minimum(const image_t temp[2*dmax+1],int x, int y,int ws,int *p);

void main()
{
  image_t   image_left,image_right,image_disp;
  image_t   Mtemp[2*dmax + 1];
  int m,n,v_left,v_right,temp,temp1;
  int i,j,d,ws1,ws2,p;
  int sum;

  clock_t t0,t1;
	 
  FILE *cpp,*cpp1,*cpp2;
  char dummy[50]="";

  cpp=fopen("VENUSL.pgm", "r+");
  cpp1=fopen("VENUSR.pgm", "r+");
  cpp2=fopen("TDVENUS_FLOW222.pgm", "w");

  fgets(dummy,50,cpp);           
  do{
    fgets(dummy,50,cpp);         
  }while(dummy[0]=='#');
  fgets(dummy,50,cpp);           

  fgets(dummy,50,cpp1);           
  do{
    fgets(dummy,50,cpp1);         
  }while(dummy[0]=='#');
  fgets(dummy,50,cpp1);           


  fprintf(cpp2,"P2\n%d %d\n255\n",size_x-2*k1,size_y-2*k2);
 
  image_left = allocate_image(size_x,size_y);
  image_right = allocate_image(size_x,size_y);
  image_disp = allocate_image(size_x,size_y);
 
  for(d=0;d<=2*dmax;d++)
  {
    Mtemp[d] = allocate_image(size_x, size_y);
  }


  for (n = 0; n < size_y; n++)
  {
    for (m = 0; m <size_x; m++)
     {
		fscanf(cpp,"%d",&temp);
		fscanf(cpp1,"%d",&temp1);

        image_left.pixel[m][n] = temp;
		image_right.pixel[m][n] = temp1;
	   
     }
  }


t0=clock();   

  ws1 = (w1/2); ws2=(w2/2); p=0;
  for (m = k1; m < size_x-k1 ; m++){  
    for (n = k2; n < size_y-k2 ; n++){      
      for (d = -dmax; d <= dmax; d++){
        sum = 0;
        for(i =- ws1; i <= ws1; i++){
          for(j = -ws2; j <= ws2; j++)
		  {

			  if( ((m+i+d)>=0) && ((m+i+d)<size_x ))
			  {
				v_left = image_left.pixel[m+i][n+j]; 
				v_right =image_right.pixel[m+i+d][n+j];  
				//if((v_left-v_right)>30)
			    sum += (v_left - v_right)*(v_left - v_right);   
			  }
			  
          }
        }		 
        Mtemp[d + dmax].pixel[m][n] = sum;
      }  
      image_disp.pixel[m][n]=minimum(Mtemp,m,n,ws1,&p);       
    }
    } 

  t1=clock();
   
  printf("  Total time= %d  microseconds\n",(t1-t0));///CLK_TCK); 

  for (n = k1; n < size_y-k1 ; n++)
  {  
    for (m = k2; m < size_x-k2 ; m++)
	{
    fprintf(cpp2,"%d ", (image_disp.pixel[m][n]));
    }
  }


}



image_t allocate_image(const int imagesize_x, const int imagesize_y)
{
  image_t result;
  int x =  0, y = 0;

  result.imagesize_x = imagesize_x;
  result.imagesize_y = imagesize_y;

  result.pixel =(int **) calloc(imagesize_x, sizeof(int*));

  for(x = 0; x < imagesize_x; x++)
  {
    result.pixel[x] =(int*) calloc(imagesize_y, sizeof(int));

    for(y = 0; y < imagesize_y; y++)
    {
      result.pixel[x][y] = 0;
    }
  }

  return result;
}


int  minimum(const image_t temp[2*dmax+1],int x, int y,int ws,int *p)
{
  int i,j,mu,a;
  double min;
 
  j=1;  
  for(i=0;i<2*dmax+1;i++){
    if(temp[i].pixel[x][y] < temp[j].pixel[x][y]) 
      j = i; 
  }

  min=temp[j].pixel[x][y];    
    for(a=0;a<2*dmax+1;a++){
     if (a!=j && temp[a].pixel[x][y]==min)
	 {
       *p+=1;
       break;
      }
    }    

   mu=abs(j-dmax);
   return (mu);
}

