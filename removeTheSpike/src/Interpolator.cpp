#include "../include/Interpolator.h"


Interpolator::Interpolator(unsigned int r, unsigned int m, shared_ptr<mat> ptr, shared_ptr<mat> ptf):r(r), m(m)
{
	ptrToData = ptr;
	ptrToFrames = ptf;
	B = make_shared<mat>(r + m, (2 * r) + m);
	Bm = make_shared<mat>(r + m, m);
	Bo = make_shared<mat>(r + m, 2 * r);
	psiM = make_shared<vec>(m);
	psiO = make_shared<vec>(2 * r);
	estymata = make_shared<rowvec>(m);

	B->fill(0.0);
	Bm->fill(0.0);
	Bo->fill(0.0);
	psiM->fill(0.0);
	psiO->fill(0.0);
	estymata->fill(0.0);
}


Interpolator::~Interpolator(void)
{
}

void Interpolator::createB(unsigned int t) {

	for(int i = 0; i < r+m; i++) {
		for(int j = 0; j <= r; j++) {
			if(j != r) {
			(*B)(i, i+j) = (*ptrToData)(r-1-j, t);
			} else {
				(*B)(i, i+j) = -1;
			}
		}
	}	
	B->print("B: ");
}

void Interpolator::createPsiO(unsigned int t) {
	(*psiO)(0, 0, size(r, 1)) = ((*ptrToFrames)(0, t+1-r, size(1, r))).t();
	(*psiO)(r, 0, size(r, 1)) = ((*ptrToFrames)(0, t+m+1, size(1, r))).t();
	psiO->print("PsiO: ");
}

void Interpolator::cutB() {
	(*Bo)(0,0, size(r+m, r)) = (*B)(0,0, size(r+m, r));
	(*Bo)(0,r, size(r+m, r)) = (*B)(0,r+m, size(r+m, r));

	(*Bm)(0,0, size(r+m, m)) = (*B)(0, r, size(r+m,m));

	Bm->print("Bm: ");
	Bo->print("Bo: ");
}

void Interpolator::createPsiM() {
	auto temp =	((*Bm).t() * (*Bm));
	auto temp2 = (*Bm).t() * (*Bo)*(*psiO);
	(*psiM) = -inv(temp) * temp2;
}

void Interpolator::interpoluj(int t) {
	createB(t);
	cutB();
	createPsiO(t);
	createPsiM();
}

shared_ptr<vec> Interpolator::getPsiM() {
	(*psiM).print("PsiM: ");
	return this->psiM;
}