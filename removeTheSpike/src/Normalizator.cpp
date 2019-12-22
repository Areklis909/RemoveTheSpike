#include "../include/Normalizator.h"


Normalizator::Normalizator(void)
{
}


Normalizator::~Normalizator(void)
{
}

std::shared_ptr<arma::mat> Normalizator::odejmijSygnaly(std::shared_ptr<arma::mat> &org, std::shared_ptr<arma::mat> &procsd) {
	int size = org->size();
	if(size != procsd->size() ) {
		std::cout <<"error" << std::endl;
		return nullptr;
	}
	std::vector<double> syg;
	for(int i = 0; i < size; i++) {
		syg.push_back((*org)(i) - (*procsd)(i));
	}
	std::shared_ptr<arma::mat> ret = std::make_shared<arma::mat>(syg);
	return ret;
}

void Normalizator::normalizuj(std::shared_ptr<arma::mat> &sygnal1, std::shared_ptr<arma::mat> &sygnal2) {
	double mocSygnaluCzystego = wyznaczMoc(sygnal1);
	for(int i = 0; i < sygnal2->size(); i++) {
		(*sygnal2)(i) /= sqrt(mocSygnaluCzystego);
	}
	
}

void Normalizator::dodajSygnaly(std::shared_ptr<arma::mat> &org, std::shared_ptr<arma::mat> &procsd) {
	int size = org->size();
	for(int i = 0; i < size; i++) {
		(*org)(i) += (*procsd)(i);
	}
}

void Normalizator::wyrownajSygnaly(std::shared_ptr<arma::mat> &wzorcowy, std::shared_ptr<arma::mat> &s, std::string pathToWrite) {
	int wzorcowaDlugosc = wzorcowy->size();
	std::cout << wzorcowaDlugosc << std::endl;
	// FileHandler f(pathToWrite.data(), 1);
	// std::shared_ptr<arma::mat> ptr = std::make_shared<arma::mat>(1, wzorcowaDlugosc);
	// for(int i = 0; i < wzorcowaDlugosc; i++) {
	// 	(*ptr)(i) = (*s)(i);
	// }
	// f.writeSamples(ptr);
}




double Normalizator::wyznaczMoc(std::shared_ptr<arma::mat> &sygnal) {
	int size = sygnal->size();
	double suma = 0;
	for(int i = 0; i < size; i++) {
		suma += pow((*sygnal)(i), 2);
	}
	return suma/size;
}