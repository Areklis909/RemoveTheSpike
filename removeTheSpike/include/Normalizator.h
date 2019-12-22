#pragma once
#include<armadillo>
#include<memory>
#include "FileHandler.h"
class Normalizator
{
public:
	Normalizator(void);
	~Normalizator(void);
	double wyznaczMoc(std::shared_ptr<arma::mat> &sygnal);
	std::shared_ptr<arma::mat> odejmijSygnaly(std::shared_ptr<arma::mat> &org, std::shared_ptr<arma::mat> &procsd);
	void dodajSygnaly(std::shared_ptr<arma::mat> &org, std::shared_ptr<arma::mat> &procsd);
	void normalizuj(std::shared_ptr<arma::mat> &sygnal1, std::shared_ptr<arma::mat> &sygnal2);
	void wyrownajSygnaly(std::shared_ptr<arma::mat> &wzorcowy, std::shared_ptr<arma::mat> &s, std::string pathToWrite);
};

