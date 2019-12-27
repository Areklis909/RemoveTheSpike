#include "../include/Skladacz.h"


Skladacz::Skladacz(int k) : kmax(k) 
{
}


Skladacz::~Skladacz(void)
{
}

std::shared_ptr<arma::vec> Skladacz::wyznaczKrawedzie(std::shared_ptr<arma::vec> &det) {
	int size = det->size();
	auto krawedzie = std::make_shared<arma::vec>(size);
	krawedzie->fill(0);
	for(int i = 1; i < size; i++) {
		(*krawedzie)(i) = (*det)(i) - (*det)(i-1);
	}
	return krawedzie;
}

void Skladacz::zlozSygnaly(arma::vec varF, arma::vec varB, std::shared_ptr<arma::mat> &sygF, std::shared_ptr<arma::mat> &sygB, std::shared_ptr<arma::mat> &sygnalOrg, std::shared_ptr<arma::vec> &fuzja) {
	int dlSyg = fuzja->size();
	auto krawedzie = wyznaczKrawedzie(fuzja);
	for(int t = 0; t < dlSyg; t++) {
		int poczatek = -1;
		int koniec = -1;
		if((*krawedzie)(t) == 1) {
			poczatek = t;
			for(int i = 0; i < 2*kmax + 4; i++) {
				if((*krawedzie)(poczatek+i) == -1) {
					koniec = poczatek + i;
					double roF = varF(poczatek-1);
					double roB = varB(dlSyg - koniec - 1);
					//double roB = varB(koniec - 1);
					double wagaF = roB/(roF+roB);
					double wagaB = roF/(roF+roB);
					
					for(int j = 0; j < koniec - poczatek; j++) {
						(*sygnalOrg)(poczatek+j) = wagaF * (*sygF)(poczatek+j) + wagaB * (*sygB)(dlSyg - (poczatek+j)-1); 
					}
					break;
				} 
			}
		} 
	}
}
