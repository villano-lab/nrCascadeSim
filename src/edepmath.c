#include <stdlib.h>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

#include "edepmath.h"

int poissonKnuth(double lambda,mt19937 *rand){

  //random distribution
  uniform_real_distribution<double> dist(0.,1.);

  //start with Knuth's algorithm quoted here
  //https://johndcook.com/blog/2010/06/14/generating-poisson-random-values

  double p=1;
  int k=0;
  double L = exp(-lambda);

  while(p>L){
    k+=1;
    p*=dist(*rand);
  }

  return k-1;
}
//DANGEROUS NEVER use for lambda<30
int poissonAtkinson(double lambda,mt19937 *rand){
  //random distribution
  uniform_real_distribution<double> dist(0.,1.);

  //Atkinson's algorithm quoted here
  //https://johndcook.com/blog/2010/06/14/generating-poisson-random-values

  double c=0.767-(3.36/lambda);
  double beta = 3.14159265358979/sqrt(3*lambda);
  double alpha = beta*lambda;
  double k=log(c)-lambda-log(beta);

  while(1){
    double u=dist(*rand);
    double x=(alpha - log((1.0 - u)/u))/beta;
    int n = floor(x + 0.5);
    if (n < 0)
	continue;
    double v = dist(*rand);
    double y = alpha - beta*x;
    double lhs = y + log(v/pow((1.0 + exp(y)),2.0));
    double rhs = k + n*log(lambda) - lgamma(n);
    //cout << alpha << "\t" << beta << "\t" << c << "\t" << k << "\t" << lambda << endl;
    //cout << n << "\t" << x+0.5 << endl;
    //cout << lhs << "\t" << rhs << endl;
    if (lhs <= rhs)
	return n;
  }

}
//if lambda<30 use Knuth otherwise use Atkinson
int poisson(double lambda,mt19937 *rand)
{
  if(lambda<30)
    return poissonKnuth(lambda,rand);
  else
    return poissonAtkinson(lambda,rand);
}
//throw a poisson with a fano factor
int poissonFano(double lambda,double F,mt19937 *rand)
{

  //get delta shift N-MISC-16-001
  double delta = (1-F)*lambda;

  //get the modified lambda
  double lambdapr = F*lambda;

  //generate a number in poisson
  int n = poisson(lambdapr,rand);

  //subtract delta 
  return n+(int)delta;
}
