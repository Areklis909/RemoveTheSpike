#include "../include/UsuwaczZaklocen.h"
#include "../include/DataSaver.h"


UsuwaczZaklocen::UsuwaczZaklocen() {
	alarm = false;
	lambda = 0.993;
	lambdaZero = 0.99;
	mi = 4;
	maxIloscAlarmow = 50;
}

UsuwaczZaklocen::UsuwaczZaklocen(int dlSyg, int r) : dlSyg(dlSyg), r(r) {	
	alarm = false;
	lambda = 0.993;
	lambdaZero = 0.99;
	mi = 4;
	maxIloscAlarmow = 50;
}


UsuwaczZaklocen::~UsuwaczZaklocen(void)
{
}

double UsuwaczZaklocen::getRownSzerOkna() {
	return (1+lambda)/(1 - lambda);
}

void UsuwaczZaklocen::ewls(int t, std::shared_ptr<arma::mat> &y, arma::mat &teta, arma::rowvec &bledyEstymacji, arma::mat &kowBledow, arma::vec &wektorWzmocnien) {
	updateFi(t, y);
	bledyEstymacji(t) =  ((*y)(t) - fi->t() * teta.col(t-1)).at(0,0); 
	auto num = kowBledow * (*fi);
	auto den = lambda + (fi->t()) * kowBledow * (*fi);
	wektorWzmocnien = num/den.at(0, 0);
	arma::mat Ir = arma::eye(r, r);
	kowBledow = (1/lambda) * (Ir - wektorWzmocnien * (*fi).t()) * kowBledow;
	teta.col(t) = teta.col(t-1) + wektorWzmocnien * bledyEstymacji(t);
}

void UsuwaczZaklocen::updateFi(int t, std::shared_ptr<arma::mat> &frames) {
	for(int i = 1; i <= r; i++) {
		if(t-i < 0) {
			(*fi)(i-1) = 0;
		} else {
			(*fi)(i-1) = (*frames)(t-i);
		}
	}	
}

void UsuwaczZaklocen::inicjalizacjaKalmana(int t, int q, std::shared_ptr<arma::vec> &xAposteriori, std::shared_ptr<arma::vec> &xApriori, std::shared_ptr<arma::mat> &PqAposteriori, std::shared_ptr<arma::mat> &PqApriori, std::shared_ptr<arma::mat> &y) {
	
	xAposteriori.reset(new arma::vec(q));
	xApriori.reset(new arma::vec(q));
	xAposteriori->fill(0);
	xApriori->fill(0);
	for(int i = 0; i < q; i++) {
		(*xAposteriori)(i) = (*y)(t - 1 - i);
	}
	PqAposteriori.reset(new arma::mat(q, q));
	PqApriori.reset(new arma::mat(q,q));
	PqAposteriori->fill(0);
	PqApriori->fill(0);
}

arma::vec UsuwaczZaklocen::createTeta(int q, arma::vec tetaWyb) {
	arma::vec tetaKal(q);
	tetaKal.fill(0);
	tetaKal(0, 0, arma::size(tetaWyb.size(), 1)) = tetaWyb;
	return tetaKal;
}

void UsuwaczZaklocen::obliczPoczatkoweWartosciWariancji(arma::vec &wariancje, arma::rowvec bledyEstymacji)  {
	wariancje(0) = 0.0;
	for(int t = 1; t < r; t++)	{
		auto okno = (1 - pow(lambdaZero, t))/(1 - lambdaZero);
		for(int i = 1; i <= t; i++) {
			wariancje(t) += pow(lambdaZero, t-i) * pow(bledyEstymacji(i), 2);
		}
		wariancje(t) /= okno;
	}
}

bool UsuwaczZaklocen::isModelStable(int r, int poczatekAlarmu, arma::mat teta) {
	arma::mat wspOdbicia(r, r);
	wspOdbicia.fill(0);

	for(int i = 0; i < r; i++) {
		wspOdbicia(r-1, i) = -teta(i, poczatekAlarmu - 1);
	}

	for(int k = r-1; k >= 1; k--) {
		for(int i = 1; i <= k; i++) {
			wspOdbicia(k - 1,i - 1) = (wspOdbicia(k, i-1) - wspOdbicia(k,k)*wspOdbicia(k,k-i))/(1-pow(wspOdbicia(k,k),2));
		}
	}
	arma::vec diagonala = wspOdbicia.diag(0);
	for(auto elem : diagonala) {
		if(abs(elem) >= 1) return false;
	}
	return true;
}

arma::vec UsuwaczZaklocen::getStableCoeffs(int t, int r, arma::vec wspolczynnikiLevDurb) {
	arma::mat R(r, r);
	R.fill(0);

	auto rownSzerOkna = getRownSzerOkna();

	for(int i = 0; i < r; i++) {
		R.diag(i).fill(wspolczynnikiLevDurb(i));
		R.diag(-i).fill(wspolczynnikiLevDurb(i));
	}
	R /= rownSzerOkna;

	arma::vec p(r);
	p.fill(0);
	for(int i = 1; i <= r; i++) {
		p(i-1) = wspolczynnikiLevDurb(i);
	}
	p /= rownSzerOkna;
	return inv(R) * p;
}


void UsuwaczZaklocen::getWspolczynnikiLevDurb(int t, int r, std::shared_ptr<arma::mat> &y, arma::vec &wspolczynnikiLevDurb) {
	double N = getRownSzerOkna();
	for(int k = 0; k <= r; k++) {
		double suma = 0;
		for(int i = 0; i <= N - k - 1; i++) {
			suma += (*y)(0, t-i) * (*y)(t-i-k);
		}
		wspolczynnikiLevDurb(k) = suma;
	}
}


void UsuwaczZaklocen::usunZakloceniaVerDwa(int dlSyg, int r, std::shared_ptr<arma::mat> &y) {
	this->r = r;
	q = r;

	arma::vec wariancjaSzumu(dlSyg);
	arma::rowvec bledyEstymacji(dlSyg);
	arma::mat teta(r, dlSyg);
	arma::mat kowBledow(r, r);
	arma::mat wspAutokorelacji(r, dlSyg);
	arma::vec wektorWzmocnien(r);
	arma::vec wspolczynnikiLevDurb(r+1);
	
	fi = std::make_shared<arma::vec>(r);

	std::shared_ptr<arma::vec> xApriori = std::make_shared<arma::vec>(q);
	std::shared_ptr<arma::vec> xAposteriori = std::make_shared<arma::vec>(q);
	std::shared_ptr<arma::mat> PqApriori = std::make_shared<arma::mat>(q, q);
	std::shared_ptr<arma::mat> PqAposteriori = std::make_shared<arma::mat>(q, q);
	std::shared_ptr<arma::vec> hq = std::make_shared<arma::vec>(q, 1);
	jednokrokoweBledyPredykcji = std::make_shared<arma::vec>(dlSyg);
	detektorZaklocen = std::make_shared<arma::vec>(dlSyg);
	progDetekcyjny = std::make_shared<arma::vec>(dlSyg);

	arma::mat var(1,1);
	var(0,0) = 0;
	xApriori->fill(0);
	xAposteriori->fill(0);
	PqAposteriori->fill(0);
	PqApriori->fill(0);
	hq->fill(0);

	fi->fill(0);
	bledyEstymacji.fill(0);
	teta.fill(0);
	kowBledow = arma::eye(r, r) * 1000;
	wariancjaSzumu.fill(0);
	wspolczynnikiLevDurb.fill(0);
	detektorZaklocen->fill(0);
	progDetekcyjny->fill(0);
	jednokrokoweBledyPredykcji->fill(0);

	int poczatekAlarmu = -100;
	int koniecAlarmu = -100;
	int liczbaZaakceptowanychProbekzRzedu = 0;
	double prog = 0;
	int iloscAlarmow = 0;
	int opoznienie = 0;
	int nrPrzebiegu = 1;
	bool poDrugimPrzebiegu = false;
	int licznik = 0;
	bool dlugiAlarm = false;
	std::cout << "Witam w Usuwaczu Zaklocen. " << std::endl;
	// inicjalizacja wariancji przed rozpocz�ciem algorytmu
	for(int i = 1; i < r; i++) {
		ewls(i, y, teta, bledyEstymacji, kowBledow, wektorWzmocnien);
	}
	obliczPoczatkoweWartosciWariancji(wariancjaSzumu, bledyEstymacji);
	for(int i = 1; i < r; i++) {
		(*progDetekcyjny)(i) = mi * sqrt(wariancjaSzumu(i-1));
	}
	arma::vec tetaWybr(r);
	tetaWybr.fill(0);
	for(int t = r; t < dlSyg; t++) {
		if(!(t%10000)) std::cout << t << std::endl;	
		if(!alarm) {
		if(opoznienie) {
			opoznienie--;
		}
		// EWLS
		ewls(t, y, teta, bledyEstymacji, kowBledow, wektorWzmocnien);
		
		if(!poDrugimPrzebiegu) {
			wariancjaSzumu(t) = wariancjaSzumu(t - 1) * lambdaZero + ((1 - lambdaZero) * pow(bledyEstymacji(t), 2));
			(*jednokrokoweBledyPredykcji)(t) = std::abs(bledyEstymacji(t));
			prog = mi * sqrt(wariancjaSzumu(t - 1));
			(*progDetekcyjny)(t) = prog;
		}
		if(poDrugimPrzebiegu) {
			wariancjaSzumu(t) = wariancjaSzumu(poczatekAlarmu-1);
		}
		if(t == koniecAlarmu + r) {
			wariancjaSzumu(t) = wariancjaSzumu(poczatekAlarmu - 1) * lambdaZero + ((1 - lambdaZero) * pow(bledyEstymacji(t), 2));
			prog = mi * sqrt(wariancjaSzumu(poczatekAlarmu - 1));
			(*progDetekcyjny)(t) = prog;
			//std::cout << "Jestem w: " << t << " i biore wariancje z t0:" << poczatekAlarmu - 1 << "ktora wynosi:" << wariancjaSzumu(poczatekAlarmu-1) << std::endl;
			poDrugimPrzebiegu = false;
			if(dlugiAlarm) {
				opoznienie = 2*r;
				dlugiAlarm = false;
			}
		}
		//prog detekcyjny
		}
		if(!opoznienie && !poDrugimPrzebiegu && (std::abs(bledyEstymacji(t)) > prog || alarm)) {
			//std::cout <<" KALMAN" <<std::endl;
			//filtr Kalmana o zmiennym rzedzie
			if(!alarm) {
				inicjalizacjaKalmana(t, q, xAposteriori, xApriori, PqAposteriori, PqApriori, y);
				poczatekAlarmu = t;
				alarm = true;
				var(0,0) = wariancjaSzumu(poczatekAlarmu - 1);
				tetaWybr = teta.col(poczatekAlarmu - 1);
			 //okreslenie stabilnosci modelu i ewentualne wyznaczenie nowych wspolczynnikow
				if(!isModelStable(r, poczatekAlarmu, teta) && t > getRownSzerOkna() - r - 1) {
					std::cout <<"UNSTABLE" << std::endl;
					getWspolczynnikiLevDurb(poczatekAlarmu - 1, r, y, wspolczynnikiLevDurb);
					tetaWybr = getStableCoeffs(t, r, wspolczynnikiLevDurb);
					//teta.col(poczatekAlarmu-1) = tetaWybr;
				}
			}
			arma::vec tetaKal = createTeta(q, tetaWybr);
			q++;
			arma::vec igrek = (tetaKal.t() * (*xAposteriori));
			double error = ((*y)(t) - igrek).at(0,0); 
			xApriori.reset(new arma::vec(q));
			(*xApriori)(0) = igrek.at(0,0);	
			(*xApriori)(1, 0, arma::size(q-1, 1)) = (*xAposteriori);
			(*hq) = (*PqAposteriori) * tetaKal;
			auto ro = tetaKal.t() * (*hq) + var;
			PqApriori.reset(new arma::mat(q,q));
			double tmp = ro.at(0, 0);
			(*PqApriori)(0, 0) = tmp;
			(*PqApriori)(1, 0, arma::size(q-1, 1)) = (*hq);
			(*PqApriori)(0, 1, arma::size(1, q-1)) = (hq->t());
			(*PqApriori)(1, 1, arma::size(q-1, q-1)) = (*PqAposteriori);
			PqAposteriori.reset(new arma::mat(q, q));
			xAposteriori.reset(new arma::vec(q));
			if(nrPrzebiegu == 1) {
			iloscAlarmow++;
			(*jednokrokoweBledyPredykcji)(t) = std::abs(error);
			double drugiProg = mi * sqrt(ro.at(0,0));
			(*progDetekcyjny)(t) = drugiProg;
			if((*jednokrokoweBledyPredykcji)(t) > drugiProg) {
				//std::cout <<"Pierwszy przebieg: det: 1, t: " << t << std::endl;
				(*xAposteriori) = (*xApriori);
				(*PqAposteriori) = (*PqApriori);
				liczbaZaakceptowanychProbekzRzedu = 0;
			} else {
				//std::cout <<"Pierwszy przebieg: det: 0, t: " << t << std::endl;
				auto lq = (*PqApriori).col(0)/tmp;
				(*xAposteriori) = (*xApriori) + (lq * error);
				(*PqAposteriori) = (*PqApriori) - (tmp * lq * lq.t());
				liczbaZaakceptowanychProbekzRzedu++;
				}
			}
			if(nrPrzebiegu == 2) {
				if((*detektorZaklocen)(t) == 1) {
					//std::cout << "Drugi przebieg: det: 1, t: " << t << std::endl;
					(*xAposteriori) = (*xApriori);
					(*PqAposteriori) = (*PqApriori);
				} else {
					//std::cout << "Drugi przebieg: det: 0, t: " << t << std::endl;
					auto lq = (*PqApriori).col(0)/tmp;
					(*xAposteriori) = (*xApriori) + (lq * error);
					(*PqAposteriori) = (*PqApriori) - (tmp * lq * lq.t());
				}
			}

			if((nrPrzebiegu == 1 && liczbaZaakceptowanychProbekzRzedu == r) || (nrPrzebiegu == 1 && iloscAlarmow == maxIloscAlarmow)) {
				if(iloscAlarmow == maxIloscAlarmow) {
					dlugiAlarm = true;
					koniecAlarmu = t+1;
				} else {
					koniecAlarmu = t - r + 1;
				}
				//std::cout <<"Koniec alarmu: " << koniecAlarmu << " Dlugosc: "<< koniecAlarmu - poczatekAlarmu <<std::endl;
				for(int i = 0; i < koniecAlarmu - poczatekAlarmu; i++) {
					(*detektorZaklocen)(poczatekAlarmu+i) = 1;
				}
				liczbaZaakceptowanychProbekzRzedu = 0;
				nrPrzebiegu = 2;
				q = r;
				inicjalizacjaKalmana(poczatekAlarmu, q, xAposteriori, xApriori, PqAposteriori, PqApriori, y);
				t = poczatekAlarmu - 1;
				iloscAlarmow = 0;
			}
			if(nrPrzebiegu == 2 && t == koniecAlarmu + r - 1) {
				alarm = false;	
				t = poczatekAlarmu - 1;
				std::vector<double> tmp;
				for(int i = 0; i < xAposteriori->size(); i++) {
					tmp.push_back((*xAposteriori)(i));
				}
				std::reverse(tmp.begin(), tmp.end());
				arma::rowvec xTransponed(tmp);
				(*y)(0, poczatekAlarmu - r, arma::size(1, xTransponed.size())) = (xTransponed);
				//std::cout << "Wklejam do: " << poczatekAlarmu - r <<" + "<< xTransponed.size() << std::endl;
				q = r;
				nrPrzebiegu = 1;
				poDrugimPrzebiegu = true;
			}
		}
	}
	//DataSaver ds(r);
	//std::shared_ptr<arma::mat> tetaPtr = std::make_shared<arma::mat>(teta);
	//ds.saveData(tetaPtr, dlSyg);
}

arma::vec UsuwaczZaklocen::usunZakloceniaVerTrzy(std::shared_ptr<arma::mat> &y, std::shared_ptr<arma::vec> &detektor) {
	q = r;
	arma::vec wariancjaSzumu(dlSyg);
	arma::rowvec bledyEstymacji(dlSyg);
	arma::mat teta(r, dlSyg);
	arma::mat kowBledow(r, r);
	arma::mat wspAutokorelacji(r, dlSyg);
	arma::vec wektorWzmocnien(r);
	arma::vec wspolczynnikiLevDurb(r+1);
	
	fi = std::make_shared<arma::vec>(r);

	std::shared_ptr<arma::vec> xApriori = std::make_shared<arma::vec>(q);
	std::shared_ptr<arma::vec> xAposteriori = std::make_shared<arma::vec>(q);
	std::shared_ptr<arma::mat> PqApriori = std::make_shared<arma::mat>(q, q);
	std::shared_ptr<arma::mat> PqAposteriori = std::make_shared<arma::mat>(q, q);
	std::shared_ptr<arma::vec> hq = std::make_shared<arma::vec>(q, 1);
	jednokrokoweBledyPredykcji = std::make_shared<arma::vec>(dlSyg);
	detektorZaklocen = std::make_shared<arma::vec>(dlSyg);
	progDetekcyjny = std::make_shared<arma::vec>(dlSyg);

	arma::mat var(1,1);
	var(0,0) = 0;
	xApriori->fill(0);
	xAposteriori->fill(0);
	PqAposteriori->fill(0);
	PqApriori->fill(0);
	hq->fill(0);

	fi->fill(0);
	bledyEstymacji.fill(0);
	teta.fill(0);
	kowBledow = arma::eye(r, r) * 1000;
	wariancjaSzumu.fill(0);
	wspolczynnikiLevDurb.fill(0);
	detektorZaklocen->fill(0);
	progDetekcyjny->fill(0);
	jednokrokoweBledyPredykcji->fill(0);

	int poczatekAlarmu = -100;
	int koniecAlarmu = -100;
	int liczbaZaakceptowanychProbekzRzedu = 0;
	double prog = 0;
	int iloscAlarmow = 0;
	int opoznienie = 0;
	bool poDrugimPrzebiegu = false;
	bool dlugiAlarm = false;
	int liczbaDobrych = 0;
	std::cout << "Witam w Usuwaczu Zaklocen. " << std::endl;
	// inicjalizacja wariancji przed rozpocz�ciem algorytmu
	for(int i = 1; i < r; i++) {
		ewls(i, y, teta, bledyEstymacji, kowBledow, wektorWzmocnien);
	}
	obliczPoczatkoweWartosciWariancji(wariancjaSzumu, bledyEstymacji);
	for(int i = 1; i < r; i++) {
		(*progDetekcyjny)(i) = mi * sqrt(wariancjaSzumu(i-1));
	}
	arma::vec tetaWybr(r);
	tetaWybr.fill(0);
	for(int t = r; t < dlSyg; t++) {
		//if(!(t%1000)) std::cout << t << std::endl;	
		if(!alarm) {
		if(opoznienie) {
			opoznienie--;
		}
		// EWLS
		ewls(t, y, teta, bledyEstymacji, kowBledow, wektorWzmocnien);
		
		if(!poDrugimPrzebiegu) {
			wariancjaSzumu(t) = wariancjaSzumu(t - 1) * lambdaZero + ((1 - lambdaZero) * pow(bledyEstymacji(t), 2));
			(*jednokrokoweBledyPredykcji)(t) = std::abs(bledyEstymacji(t));
			prog = mi * sqrt(wariancjaSzumu(t - 1));
			(*progDetekcyjny)(t) = prog;
		}
		if(poDrugimPrzebiegu) {
			wariancjaSzumu(t) = wariancjaSzumu(poczatekAlarmu-1);
		}
		if(t == koniecAlarmu + r) {
			wariancjaSzumu(t) = wariancjaSzumu(poczatekAlarmu - 1) * lambdaZero + ((1 - lambdaZero) * pow(bledyEstymacji(t), 2));
			prog = mi * sqrt(wariancjaSzumu(poczatekAlarmu - 1));
			(*progDetekcyjny)(t) = prog;
			//std::cout << "Jestem w: " << t << " i biore wariancje z t0:" << poczatekAlarmu - 1 << "ktora wynosi:" << wariancjaSzumu(poczatekAlarmu-1) << std::endl;
			poDrugimPrzebiegu = false;
			if(dlugiAlarm) {
				opoznienie = 2*r;
				dlugiAlarm = false;
			}
		}
		//prog detekcyjny
		}
		if(!opoznienie && !poDrugimPrzebiegu && (std::abs(bledyEstymacji(t)) > prog || alarm)) {
			//std::cout <<" KALMAN" <<std::endl;
			//filtr Kalmana o zmiennym rzedzie
			if(!alarm) {
				inicjalizacjaKalmana(t, q, xAposteriori, xApriori, PqAposteriori, PqApriori, y);
				poczatekAlarmu = t;
				alarm = true;
				var(0,0) = wariancjaSzumu(poczatekAlarmu - 1);
				tetaWybr = teta.col(poczatekAlarmu - 1);
			 //okreslenie stabilnosci modelu i ewentualne wyznaczenie nowych wspolczynnikow
				if(!isModelStable(r, poczatekAlarmu, teta) && t > getRownSzerOkna() - r - 1) {
					std::cout <<"UNSTABLE" << std::endl;
					getWspolczynnikiLevDurb(poczatekAlarmu - 1, r, y, wspolczynnikiLevDurb);
					tetaWybr = getStableCoeffs(t, r, wspolczynnikiLevDurb);
					//teta.col(poczatekAlarmu-1) = tetaWybr;
				}
			}
			arma::vec tetaKal = createTeta(q, tetaWybr);
			q++;
			arma::vec igrek = (tetaKal.t() * (*xAposteriori));
			double error = ((*y)(t) - igrek).at(0,0); 
			xApriori.reset(new arma::vec(q));
			(*xApriori)(0) = igrek.at(0,0);	
			(*xApriori)(1, 0, arma::size(q-1, 1)) = (*xAposteriori);
			(*hq) = (*PqAposteriori) * tetaKal;
			auto ro = tetaKal.t() * (*hq) + var;
			PqApriori.reset(new arma::mat(q,q));
			double tmp = ro.at(0, 0);
			(*PqApriori)(0, 0) = tmp;
			(*PqApriori)(1, 0, arma::size(q-1, 1)) = (*hq);
			(*PqApriori)(0, 1, arma::size(1, q-1)) = (hq->t());
			(*PqApriori)(1, 1, arma::size(q-1, q-1)) = (*PqAposteriori);
			PqAposteriori.reset(new arma::mat(q, q));
			xAposteriori.reset(new arma::vec(q));
				if((*detektor)(t) == 1) {
					//std::cout << "Drugi przebieg: det: 1, t: " << t << std::endl;
					(*xAposteriori) = (*xApriori);
					(*PqAposteriori) = (*PqApriori);
				} else {
					//std::cout << "Drugi przebieg: det: 0, t: " << t << std::endl;
					auto lq = (*PqApriori).col(0)/tmp;
					(*xAposteriori) = (*xApriori) + (lq * error);
					(*PqAposteriori) = (*PqApriori) - (tmp * lq * lq.t());
					liczbaDobrych++;
				}
				if(liczbaDobrych == r) {
					alarm = false;	
					koniecAlarmu = t-r;
					t = poczatekAlarmu - 1;
					std::vector<double> tmp;
					for(int i = 0; i < xAposteriori->size(); i++) {
						tmp.push_back((*xAposteriori)(i));
					}
					std::reverse(tmp.begin(), tmp.end());
					arma::rowvec xTransponed(tmp);
					(*y)(0, poczatekAlarmu - r, arma::size(1, xTransponed.size())) = (xTransponed);
				//std::cout << "Wklejam do: " << poczatekAlarmu - r <<" + "<< xTransponed.size() << std::endl;
					q = r;
					poDrugimPrzebiegu = true;
					liczbaDobrych = 0;
			}
		}
	}
	return wariancjaSzumu;
}

std::shared_ptr<arma::vec> UsuwaczZaklocen::getProg() {
	return progDetekcyjny;
}

std::shared_ptr<arma::vec> UsuwaczZaklocen::getDetektorZaklocen() {
	return detektorZaklocen;
}

std::shared_ptr<arma::vec> UsuwaczZaklocen::getJednkrokBledyPredykcji() {
	return jednokrokoweBledyPredykcji;
}

std::shared_ptr<arma::vec> UsuwaczZaklocen::getWariancja() {
	return wariancja;
}

std::vector<std::pair<int,int>> UsuwaczZaklocen::getAlarmsInfo() {
	return wektorAlarmInfo;
}