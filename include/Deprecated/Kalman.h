#pragma once
#include<armadillo>
#include<iostream>
#include<memory>
#include<algorithm>
using namespace arma;
using namespace std;

class Kalman
{
private:
	int r;
	int m;
	int q;
	double ro;
	double gamma;
	shared_ptr<mat>	Aq;
	shared_ptr<vec>	cq;
	shared_ptr<mat>	wspAutoregresji;
	shared_ptr<mat> frames;

	shared_ptr<vec> xApriori;
	shared_ptr<vec> xAposteriori;
	shared_ptr<mat> PqApriori;
	shared_ptr<mat> PqAposteriori;
	shared_ptr<mat> error;
	shared_ptr<mat>	g;
	shared_ptr<mat> lq;


	void createAq(int t);
	void createCq();
	void createX(int t);
public:
	Kalman(int r, int m, shared_ptr<mat> wsp, shared_ptr<mat> frms);
	void kalmanuj(int pointInTime);
	shared_ptr<vec> getResult();
	~Kalman(void);
	

};

