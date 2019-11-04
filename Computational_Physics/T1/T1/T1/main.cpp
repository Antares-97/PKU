//
//  main.cpp
//  T1
//
//  Created by LJZ on 2019/1/4.
//  Copyright © 2019年 LJZ. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <algorithm>
#include <string.h>
#include <fstream>
#define _USE_MATH_DEFINES
#define pi 3.14159265359
#define TOL 1e-2
using namespace std;

const double R = 20.0;                  // max radius
const int N = 100.0;                    // radius mesh number
const int M = 70.0;                     // theta mesh number
const double theta = M_PI / (M+1);      // mesh theta
const double r = R / (N+1);                 // mesh radius
const int max_iter = 15000;             // number of maximum iteration

double beta[N+1];                       // beta diagonal matrix
double lambda[N+1];
double eta[N+1][M+1];                   // W_j diagonal matrix
double f[N*M+1];                        // forcing term matrix
double b[N*M+1];                        // Nonhomogeneous linear equations
double A[N*M+1][N*M+1];                 // whole coefficient matrix
double u[max_iter+1][N*M+1];            // u[k]: k-th iteration solution

void initialize()
{
    memset(eta, 0, sizeof(eta));
    memset(f, 0, sizeof(f));
    memset(b, 0, sizeof(b));
    memset(A, 0, sizeof(A));
    for (int j=1; j<=M; j++)
    {
        for (int i=1; i<=N; i++)
        {
            int m = i+(j-1)*N;
            f[m]=0.8/(1.0+exp((i*r-10.0*(1.0+0.25*sqrt(5/M_PI)*(3.0*pow(cos(j*theta), 2.0)-1.0)+0.125*sqrt(7.0/M_PI)*(5.0*pow(cos(j*theta), 3.0)-3.0*cos(j*theta))))/0.6));
            b[m]= -pow(r, 2.0)*f[m];
        }
    }

    for (int i=1; i<=N; i++)
    {
        beta[i] = 1.0/(pow(i*theta, 2.0));
        lambda[i] = double(1.0/i);
        for (int j=1; j<=M; j++)
            eta[i][j] = cos(j*theta) / (2.0*sin(j*theta)*theta*pow(i, 2.0));
    }
    
    for (int i=1; i<=N; i++)              //设置 A[1][1]~A[N][N] 矩阵块
    {
        A[i][i]=-2.0-beta[i]-eta[i][1];   //对角元 T-D-W1
        if(i<N) A[i][i+1]=1.0+lambda[i];
        if(i>1) A[i][i-1]=1.0-lambda[i];
        A[i][i+N]=beta[i]+eta[i][1];      //设置 A[1][N+1]~A[N][2N]矩阵块: D+W1 只有对角元素
    }
    
    for (int k=1; k<=M-2; k++)
    {
        for (int i=1; i<=N; i++)
        {
            A[i+k*N][i+k*N] = -2.0-2.0*beta[i];           //A[kN][kN]~A[k+1 N][k+1 N] 矩阵块对角元
            if(i<N) A[i+k*N][i+k*N+1]=1+lambda[i];
            if(i>1) A[i+k*N][i+k*N-1]=1-lambda[i];        //两条辅对角线
            A[i+k*N][i+(k+1)*N] = beta[i]+eta[i][k+1];    //A[kN][k+1 N] ~ A[k+1 N][k+2 N]
            A[i+k*N][i+(k-1)*N] = beta[i]-eta[i][k+1];    //A[kN][k-1 N] ~ A[k+1 N][k N]
        }
    }
    
    for (int i=1; i<=N; i++)
    {
        A[i+(M-1)*N][i+(M-1)*N] = -2.0-beta[i]+eta[i][M];
        if(i<N) A[i+(M-1)*N][i+(M-1)*N+1]=1+lambda[i];
        if(i>1) A[i+(M-1)*N][i+(M-1)*N-1]=1-lambda[i];   //两条辅对角线
        A[i+(M-1)*N][i+(M-2)*N] = beta[i]-eta[i][M];
    }
}

double norm(double x[], double y[], int len)
{
    double ans=0;
    for (int i=1; i<=len; i++)
        ans+=pow(x[i]-y[i], 2);

    ans = sqrt(ans);
    return ans;
}

void gauss_seidel(int k)                              // k-th iteration
{
    double x[N*M+1];
    memset(x, 0, sizeof(x));
    for (int i=1; i<=N*M; i++)
            x[i]=u[k-1][i];
    for (int m=1; m<=N*M; m++)
    {
        double sum1=0, sum2=0;
        for (int j=1; j<=m-1; j++)
            sum1+=A[m][j]*x[j];
        for (int j=m+1; j<=N*M; j++)
            sum2+=A[m][j]*u[k-1][j];
        
        x[m]=(b[m]-sum1-sum2)/A[m][m];                // Gauss-Seidel iteration algorithm
        
        u[k][m]=x[m];
    }
}

int main()
{
    ofstream Outfile;
    Outfile.open("/Users/macbookair/iCloud/Desktop/Daily/计算物理/T1/T1/solution.txt",ios::trunc);
    initialize();
    printf("Initialization completed\n");
//    for (int i=1; i<=N*M; i++)
//    {
//        for (int j=1; j<=N*M; j++)
//            Outfile<< A[i][j] << "\t";
//        Outfile<<"\n";
//    }
    memset(u, 0, sizeof(u));
    bool flag = true;
    for (int k=1; k<=max_iter; k++)
    {
        gauss_seidel(k);
        printf("%d - th iteration completed\n", k);
        if (norm(u[k], u[k-1], N*M)<TOL)
        {
            printf("Precision satisfied: Iteration: %d\n",k);
            printf("Solution: \n");
            for (int m=1; m<=N*M; m++)
            {
                printf("%f\n", u[k][m]);
                Outfile<<u[k][m]<<"\n";
            }
            flag = false;
            printf("%f\n",norm(u[k], u[k-1], N*M));
            break;
        }
    }
    if (flag)
    {
        printf("Maximum iteration exceeded!\n");
        printf("Solution: \n");
        for (int m=1; m<=N*M; m++)
        {
            printf("%f\n", u[max_iter][m]);
            Outfile<<u[max_iter][m]<<"\n";
        }
        printf("%f\n",norm(u[max_iter], u[max_iter-1], N*M));
    }
    return 0;
}
