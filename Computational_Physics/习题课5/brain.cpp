#include <iomanip>
#include <fstream>
#include <array>
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<cmath>
#include <new>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
using namespace std;
#define random(a,b) (((double)rand()/RAND_MAX)*(b-a)+a)
#define random1() ((double)rand()/RAND_MAX)
void network( double *,double *,double *,double *,double ,double ,double);	
//double value( double [] ,double [] ,double,double);
double fun(double x)
{
	     double y;
	            y = 1.0/(1.0+exp(-x));
		         return y;
}

double value( double w[],double bb[],double x,double y)
{
       double h1i,h1o,h2i,h2o,oi,res;
       h1i=w[0]*x+w[1]*y+bb[0];
       h2i=w[2]*x+w[3]*y+bb[1];
       h1o=fun(h1i);
       h2o=fun(h2i);
       oi=w[4]*h1o+w[5]*h2o+bb[2];
       res=fun(oi);
       return res;
}

/*double fun(double x)
{
     double y;
       y = 1.0/(1.0+exp(-x));
     return y;
}*/

int main()
{    
     double cct=0.0;
     ifstream fission;
     fission.open("pu2402.txt");
     if (!fission) cout<<"error"<<endl;
     int n=10000;
     int i,k;
     double ewt[6]={0},ebt[3]={0};
     double *z=new double[n];
     double *ne=new double[n];
     double *p=new double[n];
     double *ew=new double[6];
     double *eb=new double[3];
     double w[6]={0.1,0.2,0.15,0.25,0.2,0.1},bb[3]={0.1,0.1,0.1};
	  
     int j=0;
//     n=10000;
     while(!fission.eof())
     {
	     fission>>z[j]>>ne[j]>>p[j];
	     j=j+1;
     }
     n=j-1;
     fission.close();
     for(i=0;i<n;i++)
     {
	z[i]=z[i]/35.0;
	ne[i]=ne[i]/70.0;
//     }
//     network(w,ew,eb,bb,z[1],ne[1],p[1]);
//     cout<<ew[0]<<ew[3]<<eb[0]<<endl;
//     ofstream gra;
//     gra.open("bu");
     //     double w[6]={0.1,0.2,0.15,0.25,0.2,0.1},bb[3]={0.1,0.1,0.1},*ew,*eb;
     for(j=0;j<20000;j++)
     {
	for(int q=0;q<6;q++)
	{
           ewt[q]=0.0;
	}
        for(int p=0;p<3;p++)
	{
	   ebt[p]=0.0;
	}
	for(i=0;i<n;i++)
	{
           network(w,ew,eb,bb,z[i],ne[i],p[i]);
	   for(k=0;k<6;k++){
	      ewt[k]=ewt[k]+ew[k]*10/n;
//              gra<<k<<"  "<<ew[k]<<endl;
	   }
           for(k=0;k<3;k++){
	      ebt[k]=ebt[k]+eb[k]*10/n;
//	      gra<<"pian: "<<k<<"  "<<eb[k]<<endl;
	   }
//	   delete ew;
//	   ew=NULL;
//	   delete eb;
//	   eb=NULL;
        }
	for(i=0;i<6;i++)
	{
           w[i]=w[i]-ewt[i];
	}
	for(i=0;i<3;i++)
	{
           bb[i]=bb[i]-ebt[i];
	}
    }
    ofstream graph;
    graph.open("newpupu20000");
    for(i=0;i<3;i++){
	    
        cout<<bb[i]<<endl;
    }
    for(i=0;i<6;i++){
       cout<<w[i]<<endl;
    }
    cout<<ebt[0]<<endl;
    for(i=0;i<n;i++){
        cct=value(w,bb,z[i],ne[i]);
//       cout<<value(w,bb,z[1],ne[1])<<endl;
        graph<<z[i]<<"  "<<ne[i]<<"  "<<cct<<endl;
    }
    return 0;
//     for(i=0;i<n;i++){
//	cout<<z[i]<<" "<<ne[i]<<" "<<p[i]<<endl;
//    }
}
void network( double w[],double *ew,double *eb,double bb[],double x,double y,double z)
{
//        ofstream gg;
//	gg.open("cct");
        for(int i=0;i<3;i++){
		eb[i]=0.0;
	}
	for(int i=0;i<6;i++){
		ew[i]=0.0;
	}
	double h1[2],h2[2],o[2],ww[6],ee[3];
	h1[0]=w[0]*x+w[1]*y+bb[0];
	h2[0]=w[2]*x+w[3]*y+bb[1];
	h1[1]=fun(h1[0]);
	h2[1]=fun(h2[0]);
        o[0]=w[4]*h1[1]+w[5]*h2[1]+bb[2];
	o[1]=fun(o[0]);
//	cout<<o[1]<<"  "<<o[0]<<"  "<<h1[1]<<"  "<<h1[0]<<"  "<<h2[1]<<"  "<<h2[0]<<endl;
//        double ww[6],ee[3];
	ew[5]=(o[1]-z)*o[1]*(1-o[1])*h2[1];
        ew[4]=(o[1]-z)*o[1]*(1-o[1])*h1[1];
        eb[2]=ew[5]/h2[1];
	ew[0]=ew[4]*w[4]*(1-h1[1])*x;
	ew[1]=ew[4]*w[4]*(1-h1[1])*y;
	eb[0]=ew[0]/x;
        ew[2]=ew[5]*w[5]*(1-h2[1])*x;
	ew[3]=ew[5]*w[5]*(1-h2[1])*y;
	eb[1]=ew[2]/x;
//	ew=*ww[0];
//	eb=*ee[0];
//	cout<<ew[4]*w[4]*(1-h1[1])*x<<"  "<<ew[4]*w[4]*(1-h1[1])*y<<"  "<<ew[2]<<"  "<<ew[3]<<"  "<<ew[5]<<eb[0]<<endl;

}
void network6( double w[],double *ew,double *eb,double bb[],double x,double y,double z)
{
     double h[6][2],o[2],ww[18],ee[7];
     o[0]=0.0;
     for(int i=0;i<18;i++){
        ew[i]=0.0;
     }
     for(i=0;i<7;i++){
        eb=[i]=0.0;
     }
     for(int i=1;i<7;i++){
        h[i][0]=w[2*(i-1)]*x+w[2*i-1]*y+bb[i-1];
        h[i][1]=fun(h[i][0]);
     }
     for(int j=1;j<7;j++){
        o[0]=o[0]+h[j][1]*w[11+j]
     }
     o[0]=o[0]+bb[6];
     o[1]=fun(o[0]);
     //grad
     for(i=1;i<7;i++){
        ew[11+i]=(o[1]-z)*(1-o[1])*h[i][1];
     }
     eb[6]=(o[1]-z)*(1-o[1]);
     for(j=1;j<7;j++){
        ew[2*(j-1)]=ew[11+j]*w[11+j]*(1-h[j][1])*x;
        ew[2*j-1]=ew[11+j]*w[11+j]*(1-h[j][1])*y;
        eb[j]=ew[2*(j-1)]/x;
     }
}
void network44( double w[],double *ew,double *eb,double bb[],double x,double y,double z)
{
     double h[4][2],s[4][2],o[2],ww[28],ee[9];
     for(int i=0;i<28;i++){
        ew[i]=0.0;
     }
     for(i=0;i<9;i++){
        eb=[i]=0.0;
     }
     for(i=1;i<5;i++){
        h[i][0]=w[2*(i-1)]*x+w[2*i-1]*y+bb[i-1];
        h[i][1]=fun(h[i][0]);
     }
     for(i=1;i<5;i++){
        for(int j=1;j<5;j++){
           s[i][0]=s[i][0]+w[7+4*(i-1)+j]*h[j][1];
        }
        s[i][0]+=bb[3+i];
        s[i][1]=fun(s[i][0]);
     }

     for(int j=1;j<4;j++){
        o[0]=o[0]+s[j][1]*w[24+j]
     }
     o[0]=o[0]+bb[8];
     o[1]=fun(o[0]);
     for(i=1;i<5;i++){
        ew[24+i]=(o[1]-z)*(1-o[1])*s[i][1];
     }
     eb[8]=(o[1]-z)*(1-o[1]);
     for(i=1;i<5;i++){
        for(j=1;j<5;j++){
           ew[7+4*(i-1)+j]=ew[24+i]*w[24+i]*(1-s[i][1])*h[j][1];
        }
        eb[3+i]=ew[24+i]*w[24+i]*(1-s[i][1]);
    }
    for(i=1;i<5;i++){
       for(j=1;j<5;j++){
          ew[2*(i-1)]+=ew[7+i+4*(j-1)]*w[8+i+4*(j-1)]*x;
          ew[2*i-1]+=ew[7+i+4*(j-1)]*w[8+i+4*(j-1)]*y;
       }
       eb[i-1]=ew[2*(i-1)]/x;
    }
}
