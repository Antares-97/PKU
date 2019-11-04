/*
Program to solve the two-dimensional Ising model
The coupling constant J = 1
Boltzmann's constant = 1, temperature has thus dimension energy
Metropolis sampling is used. Periodic boundary conditions.
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <array>
#include<stdlib.h>
#include<time.h>
#include<cmath>
//#include "lib.h"
#define random1() ((double)rand()/RAND_MAX)
using namespace std;
ofstream ofile;
// inline function for periodic boundary conditions
inline int periodic(int i, int limit, int add) {
return (i+limit+add) % (limit);
}
/*int** matrix(int n)
{	
	int** a = new int*[n];
	for(int i = 0; i < n; i++)
	{
		a[i] = new int[n];
	}
 return a;
}*/
void **matrix(int row, int col, int num_bytes)
{
int i, num;
char **pointer, *ptr;
pointer = new(nothrow) char* [row];
if(!pointer) {
cout << "Exception handling: Memory allocation failed";
cout << " for "<< row << "row addresses !" << endl;
return NULL;
}
i = (row * col * num_bytes)/sizeof(char);
pointer[0] = new(nothrow) char [i];
if(!pointer[0]) {
cout << "Exception handling: Memory allocation failed";
cout << " for address to " << i << " characters !" << endl;
return NULL;
}
ptr = pointer[0];
num = col * num_bytes;
for(i = 0; i < row; i++, ptr += num ) {
pointer[i] = ptr;
}
return (void **)pointer;
} // end: function void **matrix()
// Function to read in data from screen
void read_input(int&, int&, double&, double&, double&);
// Function to initialise energy and magnetization
void initialize(int, double, int **, double&, double&);
// The metropolis algorithm
void Metropolis(int, long&, int **, double&, double&, double *);
// prints to file the results of the calculations
void output(int, int, double, double *);
// main program
int main(int argc, char* argv[])
{
srand((int)time(0));
char *outfilename;
long idum;
int **spin_matrix, n_spins, mcs;
double w[17], average[5], initial_temp, final_temp, E, M, temp_step;
// Read in output file, abort if there are too few command-line arguments
if( argc <= 1 ){
cout << "Bad Usage: " << argv[0] <<
" read also output file on same line" << endl;
exit(1);
}
else{
outfilename=argv[1];
}
ofstream qqt;
qqt.open("spin.txt");
ofile.open(outfilename);
// Read in initial values such as size of lattice, temp and cycles
read_input(n_spins, mcs, initial_temp, final_temp, temp_step);
spin_matrix = (int**) matrix(n_spins, n_spins, sizeof(int));
//spin_matrix=matrix(n_spins);
idum = -1; // random starting point
initialize(n_spins, /*double*/ initial_temp, spin_matrix, E, M);
for ( double temp = initial_temp; temp <= final_temp; temp+=temp_step){
// initialise energy and magnetization
E=0.0;
M=0.0;
// setup array for possible energy changes
for( int de =-8; de <= 8; de++) w[de+8] = 0;
for( int de =-8; de <= 8; de+=4) w[de+8] = exp(-de/temp);
// initialise array for expectation values
for( int i = 0; i < 5; i++) average[i] = 0.0;
//initialize(n_spins, /*double*/ temp, spin_matrix, E, M);
for(int y =0; y < n_spins; y++) {
   for (int x= 0; x < n_spins; x++){
       E=E-spin_matrix[y][x]*(spin_matrix[periodic(y,n_spins,-1)][x]+spin_matrix[y][periodic(x,n_spins,-1)]);
       M=M+spin_matrix[y][x];
   }

}
// start Monte Carlo computation
   for (int cycles = 1; cycles <= mcs; cycles++){
   Metropolis(n_spins, idum, spin_matrix, E, M, w);
// update expectation values
   average[0] += E; 
   average[1] += E*E;
   average[2] += M; 
   average[3] += M*M; 
   average[4] += fabs(M);
   }
// print results
for(int i=0;i<n_spins;i++)
{
   for(int j=0;j<n_spins;j++)
   {
      qqt<<spin_matrix[i][j]<<" ";
   }
   qqt<<endl;
}

   output(n_spins, mcs, temp, average);
}
//free_matrix((void **) spin_matrix); // free memory
delete [] spin_matrix;
ofile.close(); // close output file
return 0;
}
void Metropolis(int n_spins, long& idum, int **spin_matrix, double& E, double&M, double *w)
{
// loop over all spins
//ofstream qt;
//qt.open("qt.txt");
for(int y =0; y < n_spins; y++) {
for (int x= 0; x < n_spins; x++){
// Find random position
int ix=n_spins;
int iy=n_spins;
while(ix>=n_spins||iy>=n_spins)
{
     ix = (int) (random1()*(double)n_spins);
     iy = (int) (random1()*(double)n_spins);
}
//qt<<"ix: "<<ix<<"  iy: "<<iy<<endl;
int deltaE = 2*spin_matrix[iy][ix]*(spin_matrix[iy][periodic(ix,n_spins,-1)]+spin_matrix[periodic(iy,n_spins,-1)][ix] +
spin_matrix[iy][periodic(ix,n_spins,1)] +
spin_matrix[periodic(iy,n_spins,1)][ix]);
// Here we perform the Metropolis test
if ( random1() <= w[deltaE+8] ) {
   spin_matrix[iy][ix] *= -1; // flip one spin and accept new spin config
// update energy and magnetization
   M += (double) 2*spin_matrix[iy][ix];
   E += (double) deltaE;
}
}
}
} // end of Metropolis sampling over spins
// function to initialise energy, spin matrix and magnetization
void initialize(int n_spins, double temp, int **spin_matrix,double& E, double& M)
{
// setup spin matrix and intial magnetization
for(int y =0; y < n_spins; y++) {
for (int x= 0; x < n_spins; x++){
if (temp < 1.5) spin_matrix[y][x] = 1; // spin orientation for the ground state
M += (double) spin_matrix[y][x];
}
}
// setup initial energy
for(int y =0; y < n_spins; y++) {
for (int x= 0; x < n_spins; x++){
E -= (double) spin_matrix[y][x]*(spin_matrix[periodic(y,n_spins,-1)][x]+spin_matrix[y][periodic(x,n_spins,-1)]);
}
}
}// end function initialise
void output(int n_spins, int mcs, double temperature, double *average)
{
double norm = 1/((double) (mcs)); // divided by total number of cycles
double Eaverage = average[0]*norm;
double E2average = average[1]*norm;
double Maverage = average[2]*norm;
double M2average = average[3]*norm;
double Mabsaverage = average[4]*norm;
// all expectation values are per spin, divide by 1/n_spins/n_spins
double Evariance = (E2average- Eaverage*Eaverage)/n_spins/n_spins;
double Mvariance = (M2average - Maverage*Maverage)/n_spins/n_spins;
double M2variance = (M2average - Mabsaverage*Mabsaverage)/n_spins/n_spins;
//double Mvariance = (M2average - Mabsaverage*Mabsaverage)/n_spins/n_spins;
ofile << setiosflags(ios::showpoint | ios::uppercase);
ofile << setw(15) << setprecision(8) << temperature;
ofile << setw(15) << setprecision(8) << Eaverage/n_spins/n_spins;
ofile << setw(15) << setprecision(8) << Evariance/temperature/temperature;
// ofile << setw(15) << setprecision(8) << Maverage/n_spins/n_spins;
ofile << setw(15) << setprecision(8) << M2variance/temperature;
ofile << setw(15) << setprecision(8) << Mabsaverage/n_spins/n_spins << endl;
} // end output function

//read_input(n_spins, mcs, initial_temp, final_temp, temp_step);
void read_input(int& n , int& m, double& it, double& ft, double& tp)
{
  ifstream ab;
  ab.open("input.txt");
  ab>>n;
  ab>>m;
  ab>>it;
  ab>>ft;
  ab>>tp;
  ab.close();
}

