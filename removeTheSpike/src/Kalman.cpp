#include "../include/Kalman.h"


Kalman::Kalman(int r, int m, shared_ptr<mat> wsp, shared_ptr<mat> frms) : r(r), m(m), wspAutoregresji(wsp), frames(frms)
{
	q = (2 * r)+m;
	Aq = make_shared<mat>(q, q);
	cq = make_shared<vec>(q);

	xApriori = make_shared<vec>(q);
	xAposteriori = make_shared<vec>(q);
	PqApriori = make_shared<mat>(q, q);
	PqAposteriori = make_shared<mat>(q, q);
	error = make_shared<mat>(1, 1);
	g = make_shared<mat>(1, 1);
	lq = make_shared<mat>(q, 1);
	PqAposteriori->fill(0);
	PqApriori->fill(0);
	Aq->fill(0);
	error->fill(0);
	g->fill(0);
	lq->fill(0);
	xAposteriori->fill(0);
	xApriori->fill(0);
	cq->fill(0);

	ro = 1;
	gamma = 1;
}

Kalman::~Kalman(void)
{
}

void Kalman::createCq() {
	(*cq)(0) = 1;
}

void Kalman::createAq(int t) {
	for(int i = 0; i < r; i++) {
		(*Aq)(0, i) = (*wspAutoregresji)(i, t);
	}
	for(int i = 1; i < q; i++) {
		for(int j = 0; j < q; j++) {
			if(i-1 == j) {
				(*Aq)(i, j) = 1; 
			}
		}
	}
	//(*Aq).print("Aq: ");
}

void Kalman::createX(int t) {
	for(int i = 0; i < q; i++) {
		(*xAposteriori)(i) = (*frames)(t-i);
	}
}

void Kalman::kalmanuj(int pointInTime) {
	createCq();
	createX(pointInTime);
	createAq(pointInTime);
	for(int t = 0; t < m + r; t++) {
		(*xApriori) = (*Aq) * (*xAposteriori);
		(*PqApriori) = (*Aq) * (*PqAposteriori) * (*Aq).t() + ro * (*cq) * (*cq).t();
		(*error) = (*frames)(pointInTime + 1 + t) - (*cq).t() * (*xApriori);
		(*g) = (*cq).t() * (*PqApriori) * (*cq);
		(*lq) = (*PqApriori) * (*cq)/(*g)(0);
		if(t < m) {
			(*xAposteriori) = (*xApriori);
			(*PqAposteriori) = (*PqApriori);
		} else {
			(*xAposteriori) = (*xApriori) + (*lq) * (*error); 
			(*PqAposteriori) = (*PqApriori) - ((*g)(0) * (*lq) * (*lq).t());
		}
	}
}

shared_ptr<vec> Kalman::getResult() {
	vector<double> tmp;
	for(int i = 0; i < q; i++) {
		tmp.push_back((*xAposteriori)(i));
	}
	reverse(tmp.begin(), tmp.end());
	auto ptr = make_shared<vec>(m);
	for(int i = 0; i < r+m; i++) {
		if(i >= r) {
			cout << tmp.at(i) << " ";
		}
	}
	cout << endl;
	for(int i = r; i < q-r; i++) {
		(*ptr)(i-r) = tmp.at(i);
	}
	return ptr;
}