#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

int main()
{
  mat A = randu<mat>(4,5);
  mat B = randu<mat>(4,5);
  
  cout << A*B.t() << endl;

  const int r = 4;
  mat C(r, r, fill::zeros);
  
  for(int i = 0; i < r; ++i) {
	  C.diag(i).fill(i);
	  C.diag(-i).fill(i);
  }

  C.print();


  return 0;
}
