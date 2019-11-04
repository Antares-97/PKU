#include <iomanip>
#include <fstream>
#include <array>
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<cmath>
using namespace std;
#define random(a,b) (((double)rand()/RAND_MAX)*(b-a)+a)
#define random1() ((double)rand()/RAND_MAX)

double fun(double x)
{
     double y;
     y=(x-2)*(x+3)*(x+8)*(x-9);
     return y;
}
int main()
{
     const int ct=500;
     double it,ft,tt,a,nt,xx[ct],yy[ct],pgy,pqy,res,wx,ix,p;
     int i;     
     it=1000;
     ft=1.0;
     tt=1000;
     a=0.95;
     ix=random(-10,10);
     nt=it;
     for(i=0;i<ct;i++){
        xx[i]=i*20.0/500;
	yy[i]=fun(xx[i]);
     }
     while(nt>ft)
     {
	   for(i=1;i<tt;i++){
              pgy=fun(ix);
	      wx=random(-10,10);
              pqy=fun(wx);	     
	      res=pqy-pgy;				    
	      if(res<0){		   
		ix=wx;
	      }
	      else{
                p=exp(-(res)/nt);		    
		if(random1()<p){	
		  ix=wx;		
	        }
              }
	   }
	   nt=nt*a;
    }
    cout<<"the optimization location is: "<<ix<<endl;
    cout<<"the minimum value is "<<fun(ix)<<endl;
    return 0;
}    
