#pragma once
#include<memory>
#include<armadillo>
#include "../include/DeskryptorFragmentu.h"



class analizatorAlarmow
{
public:
	analizatorAlarmow(int r, int k, int dlSyg);
	~analizatorAlarmow(void);
	std::shared_ptr<arma::vec> analizuj();
	void wyznaczKrawedzie(std::shared_ptr<arma::vec> &detF, std::shared_ptr<arma::vec> &detB);
	void podzielNaFragmenty();
	void odwrocAlarm(std::shared_ptr<arma::vec> &det);
	void odwrocDetektor(std::shared_ptr<arma::vec> &det);
	void rozszerzAlarm(std::shared_ptr<arma::vec> &det);
	std::shared_ptr<arma::vec> getDetFromKraw(std::shared_ptr<arma::vec> &krawedzie);
	std::shared_ptr<arma::vec> polaczAlarmySuma(std::shared_ptr<arma::vec> &det1, std::shared_ptr<arma::vec> &det2);
	std::shared_ptr<arma::vec> polaczAlarmyIloczyn(std::shared_ptr<arma::vec> &det1, std::shared_ptr<arma::vec> &det2);

private:
	int delta;
	int r;
	int kmax;
	int dlSyg;
	std::shared_ptr<arma::vec> krawedzieF;	
	std::shared_ptr<arma::vec> krawedzieB;
	std::shared_ptr<arma::vec> nowyDetekt;
	std::vector<DeskryptorFragmentu> deskryptory;


	int znajdzPoczatekAlarmuF(DeskryptorFragmentu df);
	int znajdzKoniecAlarmuB(DeskryptorFragmentu df);
	bool sprCzyC(int t, std::vector<DeskryptorFragmentu> &w, std::shared_ptr<arma::vec> &det1, std::shared_ptr<arma::vec> &det2);
	bool sprCzyA(int t, std::vector<DeskryptorFragmentu> &w, std::shared_ptr<arma::vec> &det1, std::shared_ptr<arma::vec> &det2);
	bool sprCzyB(int t, std::vector<DeskryptorFragmentu> &w, std::shared_ptr<arma::vec> &det1, std::shared_ptr<arma::vec> &det2);
	bool sprCzyD(int t, std::vector<DeskryptorFragmentu> &w, std::shared_ptr<arma::vec> &det1, std::shared_ptr<arma::vec> &det2);
	void rekonstrukcjaRegulaFF(DeskryptorFragmentu df);
	void rekonstrukcjaRegulaF(DeskryptorFragmentu df);
	void poszerzAlarm(std::shared_ptr<arma::vec> &det);

	void rekonstrukcjaRegulaWypelnienie(DeskryptorFragmentu df);
};

