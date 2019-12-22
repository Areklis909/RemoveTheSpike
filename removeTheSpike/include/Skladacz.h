#pragma once
#include<armadillo>
#include<memory>

struct Deskryptor {
	std::vector<std::pair<int,int>> v;
	std::vector<double> wariancjeT0;
};

class Skladacz
{

private:
	 int kmax;
public:
	Skladacz(int k);
	~Skladacz(void);
	void zlozSygnaly(arma::vec varF, arma::vec varB, std::shared_ptr<arma::mat> &sygF, std::shared_ptr<arma::mat> &sygB, std::shared_ptr<arma::mat> &sygnalOrg, std::shared_ptr<arma::vec> &fuzja);
	std::shared_ptr<arma::vec> wyznaczKrawedzie(std::shared_ptr<arma::vec> &det);
};

