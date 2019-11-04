//
//  main.cpp
//  T2
//
//  Created by LJZ on 2019/1/10.
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

const int max_iter =500000;
const double TOL = 1e-5;

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
const int N = 60;               //radius mesh number
const int M = 60;               //height mesh number
const double r = R / (N+1);     //radius mesh step
const double z = Z / (M+1);     //height mesh step

double A[N*M+1][N*M+1];         //coefficient matrix
double V[N*M+1][N*M+1];         //Potential energy matrix
double E[N*M+1];                //eigenvalue-energy vector
double u[N*M+1][N*M+1];         //rotation matrix - filled with eigen vectors - also the wave function
double H[N*M+1][N*M+1];         //Householder transformation

double f(double rho, double z)
{
    return 1.0 / (1.0 + exp(-R0 / a) * cosh(sqrt( pow(rho, 2.0) + pow(z, 2.0) ) / a));
}

void initialize()
{
    memset(A, 0, sizeof(A));
    memset(V, 0, sizeof(V));
    memset(E, 0, sizeof(E));
    memset(u, 0, sizeof(u));
    for (int j=1; j<=N*M; j++)
        for (int i=1; i<=N*M; i++)
            V[i][j] = V0 * ( f( i * r, j * z + xi - Z) + f( i * r, j * z - xi - Z) );
    
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

int iter;

void jacobi()
{
    double max;
    int row;
    int col;
    for (int i=1; i<=N*M; i++)
        u[i][i]=1;
    while (true)
    {
        max = fabs(A[1][2]);
        row = 1;
        col = 2;
        for (int i=1; i<=N*M; i++)
        {
            for (int j=1; j<=N*M; j++)
            {
                if (i!=j && fabs(A[i][j])>max)
                {
                    max = fabs(A[i][j]);
                    row = i;
                    col = j;
                }
            }
        }
        if (fabs(max) < TOL)
        {

            for (int i=1; i<=N*M; i++)
            {
                E[i]=A[i][i];
                printf("%f\n",E[i]);
            }
            printf("Precision statisfied!\n iteration: %d\n",iter);
            printf("Final Precision: %f\n", max);
            return;
        }
        if (iter>max_iter)
        {
            for (int i=1; i<=N*M; i++)
            {
                E[i]=A[i][i];
                printf("%f\n",E[i]);
            }
            printf("maximum iteration exceeded!\n");
            printf("Final Precision: %f\n", max);
            return;
        }
        
        double theta;
        if (A[row][row] == A[col][col])
            theta = M_PI/4;
        else
            theta = 0.5 * atan(2 * A[row][col] / (A[row][row] - A[col][col]));
        double aii = A[row][row];
        double ajj = A[col][col];
        double aij = A[row][col];
        double sin_theta = sin(theta);
        double cos_theta = cos(theta);
        double sin_2theta = sin(2 * theta);
        double cos_2theta = cos(2 * theta);
        A[row][row] = aii * cos_theta * cos_theta + ajj * sin_theta * sin_theta + aij * sin_2theta;
        A[col][col] = aii * sin_theta * sin_theta + ajj * cos_theta * cos_theta - aij * sin_2theta;
        A[row][col] = 0.5 * (ajj - aii) * sin_2theta + aij * cos_2theta;
        A[col][row] = A[row][col];
        for (int k = 1; k <= N*M; k++)
        {
            if (k != row && k != col)
            {
                double arowk = A[row][k];
                double acolk = A[col][k];
                A[row][k] = arowk * cos_theta + acolk * sin_theta;
                A[k][row] = A[row][k];
                A[col][k] = acolk * cos_theta - arowk * sin_theta;
                A[k][col] = A[col][k];
            }
        }
        if (iter == 0)
        {
            u[row][row] = cos_theta;
            u[row][col] = -sin_theta;
            u[col][row] = sin_theta;
            u[col][col] = cos_theta;
        }
        else
        {
            for (int k = 1; k <= N*M; k++)
            {
                double pki = u[k][row];
                double pkj = u[k][col];
                u[k][row] = pki * cos_theta + pkj * sin_theta;
                u[k][col] = pkj * cos_theta - pki * sin_theta;
            }
        }
        printf("%d - th iteration completed. Precision: %f\n",iter,max);
        iter++;
    }
}

int main()
{
    initialize();
    printf("Initialization completed\n");
    
    iter = 0;
    jacobi();
    
    ofstream A_matrix;
    A_matrix.open("/Users/macbookair/iCloud/Desktop/Daily/计算物理/T2/A_matrix_60.txt",ios::trunc);
    for (int i=1; i<=N*M; i++)
    {
        for (int j=1; j<=N*M; j++)
            A_matrix<< A[i][j] << "\t";
        A_matrix<<"\n";
    }
    A_matrix.close();
    
    ofstream Energy;
    Energy.open("/Users/macbookair/iCloud/Desktop/Daily/计算物理/T2/Energy_60.txt",ios::trunc);
    for (int i=1; i<=N*M; i++)
        Energy<<E[i]<<"\n";
    Energy.close();
    
    ofstream wave;
    wave.open("/Users/macbookair/iCloud/Desktop/Daily/计算物理/T2/wave_function_60.txt",ios::trunc);
    for (int i=1; i<=N*M; i++)
    {
        for (int j=1; j<=N*M; j++)
            wave<< u[i][j] << "\t";
        wave<<"\n";
    }
    wave.close();
    
    ofstream probability;
    probability.open("/Users/macbookair/iCloud/Desktop/Daily/计算物理/T2/probability_60.txt",ios::trunc);
    for (int i=1; i<=N*M; i++)
    {
        for (int j=1; j<=N*M; j++)
            probability<< pow(u[i][j],2.0) << "\t";
        probability<<"\n";
    }
    probability.close();
    
    return 0;
}
