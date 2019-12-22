#pragma once

#include<fstream>
#include<armadillo>
#include<memory>
#include<vector>
using namespace arma;
using namespace std;
class DataSaver
{
private:
	fstream plik;
	fstream plotPlik;
	int r;
public:
	DataSaver(unsigned int ord);
	DataSaver();
	~DataSaver(void);
	void saveData(shared_ptr<mat> dataMatrix, int len);
	void savePlots(vector<vector<double> > vec);
	void saveAlarms(shared_ptr<vec> det1, shared_ptr<vec> det2, shared_ptr<vec> det3, int len);
	void saveSigAndDet(std::shared_ptr<arma::mat> &y, std::shared_ptr<arma::vec> detF, std::shared_ptr<arma::vec> detB, std::shared_ptr<arma::vec> detFB);
};

