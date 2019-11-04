//
//  main.cpp
//  T3
//
//  Created by LJZ on 2019/1/3.
//  Copyright © 2019年 LJZ. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <string.h>
#define N 10000
#define TOL 1e-2
#define stepsize 0.1
#define INF 0xffffff
using namespace std;

double X [N][5];
bool flag, answer_found;
double minx[5]={-1,-1,-5,0,-2};
double maxx[5]={1,1,-1,2,2};

double g(double x[5])
{
    double x1 = x[0];
    double x2 = x[1];
    double x3 = x[2];
    double x4 = x[3];
    double x5 = x[4];
    double result = pow(x1-0.718,2)+pow(double((x2+0.718)/2),2)+pow(x3-0.2,2)+pow((x4+2)/0.1,2)+pow(x5,2)+pow(x2-x3-1.5,2);
    return result;
}

double x_grad(double x[5], int i)
{
    double x1 = x[0];
    double x2 = x[1];
    double x3 = x[2];
    double x4 = x[3];
    double x5 = x[4];
    double grad = 0.000;
    switch (i)
    {
        case 1:
            grad = 2*(x1-0.718);
            break;
            
        case 2:
            grad = (2.500*x2 - 2.000*x3 -2.641);
            break;
            
        case 3:
            grad = (-2.000*x2 + 4.000*x3 + 2.600);
            break;
            
        case 4:
            grad = 200*(x4+2);
            break;
            
        case 5:
            grad = 2*x5;
            break;
            
        default:
            break;
    }
    return grad;
}

double dot(double * x, double *y, int len)
{
    double ans=0;
    for (int i=0; i<len; i++)
        ans+=x[i]*y[i];
    return ans;
}

void frank_wolfe(int k)
{
    double x[5],y[5],d[5],gradient[5];
    for (int i=0; i<5; i++)
    {
        x[i]=X[k][i];
    }
    double value=INF;
    
    for (int i=0; i<5; i++)
    {
        gradient[i]=x_grad(x, i+1);
        y[i]=gradient[i]>0? minx[i]:maxx[i];
        d[i]=y[i]-x[i];
    }
    
    if (fabs(dot(d, gradient, 5))<TOL)
    {
        flag=true;
        answer_found=true;
        cout<<"Success!"<<endl;
        cout<<"Iteration:"<<k<<endl;
        cout<<"minimal function value: "<<g(x)<<endl;
        cout<<"solution vector:";
        for (int i=0; i<5; i++)
        {
            cout<<" "<<x[i];
        }
        cout<<endl;
        return;
    }
    value = INF;
    double best_step=0.001;
    double _x[5];
    for (double h=0.0001; h<=1; h+=0.0001)
    {
        for (int i=0; i<5; i++)
            _x[i]=x[i] + h * d[i];
        double new_value = g(_x);
        if (new_value<value)
        {
            value=new_value;
            best_step=h;
        }
    }
    for (int i=0; i<5; i++)
        X[k+1][i]=x[i]+best_step*d[i];
}

int main()
{
    int h = 0;
    flag = false;
    memset(X, 0, sizeof(X));
    
      X[0][0]=0.7;
      X[0][1]=0.400;
      X[0][2]=-2.0;
      X[0][3]=1.0;
      X[0][4]=1.0;

    for (h=0; h<N; h++)
    {
        frank_wolfe(h);
        if(flag) break;
    }
    if(flag==false)
    {
        for (int h=0; h<5; h++)
            cout<<" "<<X[0][h];
        cout<<endl;
    }
    return 0;
}
