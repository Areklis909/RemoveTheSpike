#pragma once

#include<iostream>
#include<memory>
#include<armadillo>

using namespace std;
using namespace arma;



class Interpolator
{
private:
	unsigned int m;
	unsigned int r;
	shared_ptr<mat> ptrToData;
	shared_ptr<mat> ptrToFrames;
	shared_ptr<mat> B;
	shared_ptr<mat> Bm;
	shared_ptr<mat> Bo;
	shared_ptr<vec> psiM;
	shared_ptr<vec> psiO;
	shared_ptr<rowvec> estymata;
	void createB(unsigned int t);
	void createPsiO(unsigned int t);
	void cutB();
	void createPsiM();
	

public:
	Interpolator(unsigned int r, unsigned int m, shared_ptr<mat> ptr, shared_ptr<mat> ptf);
	~Interpolator(void);
	void interpoluj(int t);
	shared_ptr<vec> getPsiM();
};

