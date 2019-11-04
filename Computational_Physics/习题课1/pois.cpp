/*
Program to solve the one-dimensional Poisson equation
-u''(x) = f(x) rewritten as a set of linear equations
A u = f where A is an n x n matrix, and u and f are 1 x n vectors
In this problem f(x) = (3x+x*x)exp(x) with solution u(x) = x(1-x)exp(x)
The program reads from screen the name of the output file.
Blitz++ is used here, with arrays starting from 1 to n
*/

#include <iomanip>
#include <fstream>
#include <array>
#include <blitz/array.h>
#include <iostream>
using namespace std;
using namespace blitz;
ofstream ofile;
// Main program only, no other functions
int main(int argc, char* argv[])
{
char *outfilename;
int i, j, n;
double h, btemp;
// Read in output file, abort if there are too few command-line arguments
if( argc <= 1 ){
cout << "Bad Usage: " << argv[0] <<
" read also output file on same line" << endl;
exit(1);
}
else{
outfilename=argv[1];
}
ofile.open(outfilename);
cout << "Read in number of mesh points" << endl;
cin >> n;
h = 1.0/( (double) n+1);
// Use Blitz to allocate arrays
// Use range to change default arrays from 0:n-1 to 1:n
Range r(1,n);
Array<double,1> a(r), b(r), c(r), y(r), f(r), temp(r);
// set up the matrix defined by three arrays, diagonal, upper and lower diagonal band
b = 2.0; a = -1.0 ; c = -1.0;
// Then define the value of the right hand side f (multiplied by h*h)
for(i=1; i <= n; i++){
// Explicit expression for f, could code as separate function
f(i) = h*h*(i*h*3.0+(i*h)*(i*h))*exp(i*h);
}
// solve the tridiagonal system, first forward substitution
btemp = b(1);
for(i = 2; i <= n; i++) {
temp(i) = c(i-1) / btemp;
btemp = b(i) - a(i) * temp(i);
y(i) = (f(i) - a(i) * y(i-1)) / btemp;
}
// then backward substitution, the solution is in y()
for(i = n-1; i >= 1; i--) {
y(i) -= temp(i+1) * y(i+1);
}
// write results to the output file
for(i = 1; i <= n; i++){
ofile << setiosflags(ios::showpoint | ios::uppercase);
ofile << setw(15) << setprecision(8) << i*h;
ofile << setw(15) << setprecision(8) << y(i);
ofile << setw(15) << setprecision(8) << i*h*(1.0-i*h)*exp(i*h) <<endl;
}
ofile.close();
}
