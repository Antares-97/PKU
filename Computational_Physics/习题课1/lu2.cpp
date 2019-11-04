#include <iostream>
#include <iomanip>
#include <fstream>
#include <new>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
//#include "lib.h"
using namespace std;

void lu(double **,double **,double **, int);
void inve(double **,double **,double **,int);
void show(double **aa,int n)
{
  int i,j;
  for(j = 0; j < n; j++) {
    for(i = 0; i < n; i++){
       cout<<setiosflags(ios::showpos)<<setiosflags(ios::fixed)<<setprecision(3)<<aa[j][i]<<"   ";
    }
    cout<<endl;
 }
}
double** matrix(int n)
{	
	double** a = new double*[n];
	for(int i = 0; i < n; i++)
	{
		a[i] = new double[n];
	}
 return a;
}

int main()
{
int i, j, k, result,col,row,n = 3;
double  sum,**matr,**u,**l,**ai;
//a[3][3] = {{1.0, 3.0, 4.0},
//{3.0, 4.0, 6.0},
//{4.0, 6.0, 8.0}};
char ch;
ifstream ifile;
ifile.open("array3.txt");
col=0;
row=0;
while(!ifile.eof() ){
    ch=ifile.get();
    if(ch== '\n'){
      row +=1;
    }
    if(ch== ' '){
     col +=1;
    }
}
ifile.close();
col=col/row+1;
cout<<"the row is "<<row<<endl;
cout<<"the column is "<<col<<endl;
cout<<"the input matrix is: "<<endl;
ifile.open("array3.txt");
double ab[row][col];
for(i=0;i<row;i++){
   for(j=0;j<col;j++){
      ifile>>ab[i][j];
   }
}
for(j = 0; j < row; j++) {
  for(i = 0; i < col; i++){
       cout<<setiosflags(ios::showpos)<<setiosflags(ios::fixed)<<setprecision(3)<<ab[j][i]<<"   ";
    }
    cout<<endl;
 }
cout<<endl;
n=row;
matr=matrix(n);
u=matrix(n);
l=matrix(n);
ai=matrix(n);
for(i=0;i<row;i++){
   matr[i]=ab[i];
}
//cout<<*(matr[1])<<"  "<<matr[1][2]<<"  "<<matr[0][0]<<endl;
lu(matr,u,l,n); // calculate and return inverse matrix
inve(ai,u,l,n);
//cout<<*(matr[1])<<"  "<<matr[1][2]<<endl;
show(matr,n);
cout<<"output corresponding U matrix: "<<endl;
show(u,n);
cout<<"output corresponding L matrix: "<<endl;
show(l,n);
cout<<"output corresponding inverse matrix: "<<endl;
show(ai,n);
return 0;
} // End: function main()

void lu(double **matrix,double **u,double **l,int n)
{
double d, *col;//, **u,**l;
int length=n;
int i,j;
// allocate space in memory
//y = (double **) a(n, n, sizeof(double));
//u=matrix(n);
//l=matrix(n);
for(j = 0; j < n; j++) {
   for(i = 0; i < n; i++){ 
      u[i][j]=0;
      l[i][j]=0;
   }
}
//y=(double **)cc;
/*for(j = 0; j < n; j++) {
   for(i = 0; i < n; i++) a[i][j]=y[i][j];
}*/
for(i=0;i<n;i++){
   l[i][i]=1;
}
for(int a = 0; a < length; a++){			
   for(int b = 0; b < length; b++){				
      double sum = 0.0;				
      if(a <= b){					
        for(int i = 0; i < a; i++){						
           sum += l[a][i] * u[i][b];					
        }					
        u[a][b] = matrix[a][b] - sum;				
      }
      else{					
        for(int i = 0; i < b; i++){						
            sum += l[a][i] * u[i][b];					
        }					
        l[a][b] = (matrix[a][b] - sum) / u[b][b];				
      }			
   }		
}
}

void inve(double **ai,double **u,double **l,int n)
{
 int i,j,q;
 double **ee,**dd;
 dd=matrix(n);
 ee=matrix(n);
 for(j = 0; j < n; j++) {
   for(i = 0; i < n; i++){
      if(i==j){
        dd[i][i]=1;
        ee[i][i]=1.0/u[i][i];
      }
      else{
      dd[i][j]=0;
      ee[i][j]=0;
      }
   }
 }

 for(i=0;i<n;i++){
    for(j=0;j<n;j++){
       double sum=0;
       if(i<j){
         for(int q=0;q<j;q++){
            sum +=ee[i][q]*u[q][j];
         }
         ee[i][j]=-ee[j][j]*sum;
       }
       else if(i>j){
         for(int q=j+1;q<i;q++){
            sum += dd[q][j]*l[i][q];
         }
         dd[i][j]=-l[i][j]-sum;
       }
    }
 }
show(ee,n);
show(dd,n);
 for(i=0;i<n;i++){
    for(j=0;j<n;j++){
       for(q=0;q<n;q++){
          ai[i][j] +=ee[i][q]*dd[q][j];
       }
    }
 }
 }
