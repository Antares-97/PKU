//
//  main.cpp
//  T2_QR
//
//  Created by LJZ on 2019/1/11.
//  Copyright © 2019年 LJZ. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <algorithm>
#include <string.h>
#include <fstream>
#include <iomanip>
#define _USE_MATH_DEFINES
#define pi 3.14159265359

using namespace std;

const int max_iter =437000;
const double TOL = 1e-3;

const double xi = 7.5;          //fm
const double V0 = -50;          //MeV
const double R0 = 2;            //fm
const double a = 1;             //fm
const double lambda0 = 5.0;
const double h2m = 20.721246;   //fm^2
const double hc = 197.32696;    //MeV * fm
const double mc2 = 939.56535;   //MeV

const double R = 50.0;          //fm
const double Z = 25.0;          //fm, z in [-Z, Z]
const int N = 60;              //radius mesh number
const int M = 60;              //height mesh number
const double r = R / (N+1);     //radius mesh step
const double z = Z / (M+1);     //height mesh step

int iter;
//double A[N*M+1][N*M+1];         //coefficient matrix
double ** A;                    //coefficient matrix
double V[N*M+1][N*M+1];         //Potential energy matrix
double E[N*M+1];                //eigenvalue-energy vector
double u[N*M+1][N*M+1];         //rotation matrix - filled with eigen vectors - also the wave function
double prec[max_iter];             //precision changing situation
double ** H;         //Householder transformation

double f(double rho, double z)
{
    return 1.0 / (1.0 + exp(-R0 / a) * cosh(sqrt( pow(rho, 2.0) + pow(z, 2.0) ) / a));
}

void initialize()
{
    //memset(A, 0, sizeof(A));
    memset(V, 0, sizeof(V));
    memset(E, 0, sizeof(E));
    memset(u, 0, sizeof(u));
    memset(prec, 0, sizeof(prec));
    for (int j=1; j<=N*M; j++)
        for (int i=1; i<=N*M; i++)
            V[i][j] = V0 * ( f( i * r, j * z + xi - Z) + f( i * r, j * z - xi - Z) );
    
    A=(double **)new double *[N*M+1]();     // 重定义了, 所以数组没有结果
    for (int k=1; k<=N*M; k++)
        A[k] = new double [N*M+1];
    
    for (int i=1; i<=N; i++)              //设置 A[1][1]~A[N][N] 矩阵块
    {
        A[i][i] = V[i][1] + 2*h2m/pow(r, 2.0) - 2/pow(z, 2);   //对角元 T 的对角线: V[i][j] + h2/m(r)2 - 2/z2
        if(i<N) A[i][i+1] = -h2m/pow(r, 2.0) + 1/(2*i*pow(r, 2.0));
        if(i>1) A[i][i-1] = -h2m/pow(r, 2.0) - 1/(2*i*pow(r, 2.0));
        A[i][i+N] = 1/pow(z, 2.0);       //设置 A[1][N+1]~A[N][2N]矩阵块: D 只有对角元素
    }
    
    for (int k=1; k<=M-2; k++)
    {
        for (int i=1; i<=N; i++)
        {
            A[i+k*N][i+k*N] = V[i][k+1] + 2*h2m/pow(r, 2.0) - 2/pow(z, 2);//A[kN][kN]~A[k+1 N][k+1 N] 矩阵块对角元
            if(i<N) A[i+k*N][i+k*N+1] = -h2m/pow(r, 2.0) + 1/(2*i*pow(r, 2.0));
            if(i>1) A[i+k*N][i+k*N-1] = -h2m/pow(r, 2.0) - 1/(2*i*pow(r, 2.0));        //两条辅对角线
            A[i+k*N][i+(k+1)*N] = 1/pow(z, 2.0);    //A[kN][k+1 N] ~ A[k+1 N][k+2 N]
            A[i+k*N][i+(k-1)*N] = 1/pow(z, 2.0);    //A[kN][k-1 N] ~ A[k+1 N][k N]
        }
    }
    
    for (int i=1; i<=N; i++)
    {
        A[i+(M-1)*N][i+(M-1)*N] = V[i][M] + 2*h2m/pow(r, 2.0) - 2/pow(z, 2);
        if(i<N) A[i+(M-1)*N][i+(M-1)*N+1] = -h2m/pow(r, 2.0) + 1/(2*i*pow(r, 2.0));
        if(i>1) A[i+(M-1)*N][i+(M-1)*N-1] = -h2m/pow(r, 2.0) - 1/(2*i*pow(r, 2.0));   //两条辅对角线
        A[i+(M-1)*N][i+(M-2)*N] = 1/pow(z, 2.0);
    }
}

double sgn(double x)
{
    if (x>0) return 1;
    else if(x<0) return -1;
    return 0;
}

double ** vector_construct_matrix(double v[], int len)      // len 是数组最大下标
{
    double ** matrix=(double **)new double *[len+1]();      // M 返回大小应为[len+1][len+1]
    for (int i=1; i<=len; i++)
    {
        matrix[i] = new double [len+1];
        for (int j=1; j<=len; j++)
        {
            matrix[i][j]=v[i]*v[j];
        }
    }
    return matrix;
}

double ** matrix_multiply(double **A, double **B, int len)
{
    double ** ans = (double **)new double *[len+1]();
    for (int i=1; i<=len; i++)
    {
        ans[i] = new double [len+1];
        for (int j=1; j<=len; j++)
        {
            ans[i][j]=0;
            for (int k=1; k<=len; k++)
            {
                ans[i][j]+=A[i][k]*B[k][j];
            }
        }
    }
    return ans;
}

void Householder()                              // k = n-2 时循环退出, Ak-1 应该是一个三对角矩阵, 目的是变成一个 Hessenberg 矩阵
{
    cout<<A[2][1]<<endl;
    double alpha = -sgn(A[2][1])/pow(z, 2.0);   //原来是乘以sqrt(sum(aj1))的,但A矩阵第一列只有D这么一个对角矩阵元需要消掉,1/z^2
    double r = sqrt((pow(alpha, 2.0)-A[2][1]*alpha)/2.0);
    double v[N*M+1];
    memset(v, 0, sizeof(v));
    
    v[2] = A[2][1] - alpha / (2*r);
    v[N+1] = A[N+1][1] / (2*r);                 //第一列只有A[N+1][1]=1/z^2不为0,其他从3到 N*M 都是0
    
    double ** P;
    P = vector_construct_matrix(v, N*M);
    cout<<"P1 calculated"<<endl;
    for (int i=1; i<=N*M; i++)
    {
        for (int j=1; j<=N*M; j++)
        {
            P[i][j] = -2 * P[i][j];
            if (i==j) P[i][j]++;                // 手动算 I-2vvT .........
        }
    }
    cout<<"P1 calculated"<<endl;
    
    A = matrix_multiply(P, A, N*M);             // A = PA
    A = matrix_multiply(A, P, N*M);             // A2 = AP
    cout<<"A calculated"<<endl;
    
    for (int k=2; k<=N*M-2; k++)                // k = 2, 3, ... , n-2
    {
        cout<<A[k+1][k]<<endl;
        alpha = -sgn(A[k+1][k])/pow(z, 2.0);
        r = sqrt((pow(alpha, 2.0)-A[k+1][k]*alpha)/2.0);
        memset(v, 0, sizeof(v));
        v[k+1] = A[k+1][k] - alpha / (2*r);
        v[N+k] = A[N+k][k] / (2*r);
        
        P = vector_construct_matrix(v, N*M);
        cout<<"P calculated"<<endl;
        for (int i=1; i<=N*M; i++)
        {
            for (int j=1; j<=N*M; j++)
            {
                P[i][j] = -2 * P[i][j];
                if (i==j) P[i][j]++;            // 手动算 I-2vvT .........
            }
        }
        cout<<"P calculated"<<endl;
        A = matrix_multiply(P, A, N*M);         // Ak = PAk
        A = matrix_multiply(A, P, N*M);         // Ak+1 = AkP
        cout<<"A calculated"<<endl;
    }
}

int main()
{
    initialize();
    printf("Initialization completed\n");
    
    iter = 0;
    
    Householder();
    
//    double v[5]={0.0,1.0,2.0,3.0,4.0};
//    double ** m;
//    m = vector_construct_matrix(v, 4);
//
//    double M [5][5];
//    memset(M, 0, sizeof(M));
//    M = matrix_multiply(m, m, 4);                   //一个二维数组会报错, 只能设成二维指针
//
//    for (int i=1; i<=4; i++)
//    {
//        for (int j=1; j<=4; j++)
//        {
//            cout<< m[i][j]<<"\t";
//        }
//        cout<<endl;
//    }
//    cout<<endl;
//    for (int i=1; i<=4; i++)
//    {
//        for (int j=1; j<=4; j++)
//        {
//            cout<< M[i][j]<<"\t";
//        }
//        cout<<endl;
//    }
    
    
    
    ofstream A_matrix;
    A_matrix.open("/Users/macbookair/iCloud/Desktop/Daily/计算物理/T2/T2_QR/A_matrix.txt",ios::trunc);
    for (int i=1; i<=N*M; i++)
    {
        for (int j=1; j<=N*M; j++)
            A_matrix<< A[i][j] << "\t";
        A_matrix<<"\n";
    }
    A_matrix.close();
//
//    ofstream Energy;
//    Energy.open("/Users/macbookair/iCloud/Desktop/Daily/计算物理/T2/T2_QR/Energy.txt",ios::trunc);
//    for (int i=1; i<=N*M; i++)
//        Energy<<E[i]<<"\n";
//    Energy.close();
//
//    ofstream wave;
//    wave.open("/Users/macbookair/iCloud/Desktop/Daily/计算物理/T2/T2_QR/wave_function.txt",ios::trunc);
//    for (int i=1; i<=N*M; i++)
//    {
//        for (int j=1; j<=N*M; j++)
//            wave<< u[i][j] << "\t";
//        wave<<"\n";
//    }
//    wave.close();
//
//    ofstream probability;
//    probability.open("/Users/macbookair/iCloud/Desktop/Daily/计算物理/T2/T2_QR/probability.txt",ios::trunc);
//    for (int i=1; i<=N*M; i++)
//    {
//        for (int j=1; j<=N*M; j++)
//            probability<< pow(u[i][j],2.0) << "\t";
//        probability<<"\n";
//    }
//    probability.close();
//
//    ofstream precision;
//    precision.open("/Users/macbookair/iCloud/Desktop/Daily/计算物理/T2/T2_QR/precision.txt",ios::trunc);
//    for (int i=1; i<=max_iter / 10; i++)
//        precision<<prec[i]<<"\n";
//    precision.close();
//
    return 0;
}
