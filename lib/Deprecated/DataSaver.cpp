#include "../include/DataSaver.h"


DataSaver::DataSaver(unsigned int ord): r(ord)
{
}

DataSaver::DataSaver()
{
}

DataSaver::~DataSaver(void)
{
}

void DataSaver::saveSigAndDet(std::shared_ptr<arma::mat> &y, std::shared_ptr<arma::vec> detF, std::shared_ptr<arma::vec> detB, std::shared_ptr<arma::vec> detFB) {
	string filename = "";
	const string dStr = "s";
	const string txtStr = ".txt";
	fstream fs;
	int len = y->size();
	int liczbaPlikow = 4;
	for(int i = 0; i < liczbaPlikow; i++) {
	filename = dStr + to_string(i) + txtStr; 
	fs.open(filename, std::fstream::out | std::fstream::trunc);
	if(!fs.is_open()) {
		std::cout << "notgood" << std::endl;
	}
	for(int j = 0; j < len; j++) {
		if(i == 0) {
			fs << (*y)(j) << std::endl;
		} else if(i==1) {
			fs << (*detF)(j) << std::endl;
		} else if(i==2) {
			fs << (*detB)(j) << std::endl;
		} else if(i==3) {
			fs << (*detFB)(j) << std::endl;
		}
	}
		fs.close();
	}
}


void DataSaver::savePlots(vector<vector<double> > vec) {
	auto ile = vec.size();
	const string plot = "plot";
	const string txt = ".txt";
	for(int i = 0; i < ile; i++) {
		string filename = plot;
		filename.append(to_string(i));
		filename.append(txt);
		this->plotPlik.open(filename, std::fstream::out | std::fstream::trunc);
		if(this->plotPlik.good()) {
			for(int j = 0; j < vec.at(i).size(); j++) {
				this->plotPlik << vec.at(i).at(j) << endl;				
			}
		}
		this->plotPlik.close();
	}
}

void DataSaver::saveData(shared_ptr<mat> ptrToData, int len) {
	string filename = "";
	const string rStr = "r";
	const string txtStr = ".txt";
	for(int i = 0; i < r; i++) {
		filename = rStr + to_string(i) + txtStr; 
		plik.open(filename, std::fstream::out | std::fstream::trunc);
		for(int j = 0; j < len; j++) {
			plik << (ptrToData->row(i))(j);
			plik << endl;
		}
		plik.close();
	}
}

void DataSaver::saveAlarms(shared_ptr<vec> det1, shared_ptr<vec> det2, shared_ptr<vec> det3, int len) {
	string filename = "";
	const string dStr = "d";
	const string txtStr = ".txt";
	fstream fs;
	int liczbaAlarmow = 3;
	for(int i = 0; i < liczbaAlarmow; i++) {
	filename = dStr + to_string(i) + txtStr; 
	fs.open(filename, std::fstream::out | std::fstream::trunc);
	if(!fs.is_open()) {
		std::cout << "notgood" << std::endl;
	}
	for(int j = 0; j < len; j++) {
		if(i == 0) {
			fs << (*det1)(j) << std::endl;
		} else if(i==1) {
			fs << (*det2)(j) << std::endl;
		} else if(i==2) {
			fs << (*det3)(j) << std::endl;
		}
	}
		fs.close();
	}
}