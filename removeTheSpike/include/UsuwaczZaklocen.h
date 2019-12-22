#pragma once

#include<memory>
#include<armadillo>
#include<vector>
#include<algorithm>
#include<iostream>

class UsuwaczZaklocen
{
private:
	std::shared_ptr<arma::vec> fi;
	std::shared_ptr<arma::vec> jednokrokoweBledyPredykcji;
	std::shared_ptr<arma::vec> detektorZaklocen;
	std::shared_ptr<arma::vec> progDetekcyjny;
	std::shared_ptr<arma::vec> wariancja;

	double lambda;
	double lambdaZero;
	double mi;
	int r;
	int q;
	int dlSyg;
	bool alarm;
	int maxIloscAlarmow;
	std::vector<std::pair<int,int>> wektorAlarmInfo;

	void obliczPoczatkoweWartosciWariancji(arma::vec &wariancje, arma::rowvec bledyEstymacji);
	double getRownSzerOkna();
	void getWspolczynnikiLevDurb(int t, int r, std::shared_ptr<arma::mat> &y, arma::vec &wspolczynnikiLevDurb);
	arma::vec createTeta(int q, arma::vec tetaWyb);
	void updateFi(int t, std::shared_ptr<arma::mat> &frames);
	void ewls(int t, std::shared_ptr<arma::mat> &y, arma::mat &teta, arma::rowvec &bledyEstymacji, arma::mat &kowBledow, arma::vec &wektorWzmocnien);
	void inicjalizacjaKalmana(int t, int q, std::shared_ptr<arma::vec> &xAposteriori, std::shared_ptr<arma::vec> &xApriori, std::shared_ptr<arma::mat> &PqAposteriori, std::shared_ptr<arma::mat> &PqApriori, std::shared_ptr<arma::mat> &y);
	bool isModelStable(int r, int poczatekAlarmu, arma::mat teta);
	arma::vec getStableCoeffs(int t, int r, arma::vec wspolczynnikiLevDurb);
public:
	UsuwaczZaklocen();
	UsuwaczZaklocen(int dlSyg, int r);
	~UsuwaczZaklocen(void);
	void usunZakloceniaVerDwa(int dlSyg, int r, std::shared_ptr<arma::mat> &y);
	arma::vec usunZakloceniaVerTrzy(std::shared_ptr<arma::mat> &y, std::shared_ptr<arma::vec> &detektor);
	std::shared_ptr<arma::vec> getDetektorZaklocen();
	std::shared_ptr<arma::vec> getProg();
	std::shared_ptr<arma::vec> getJednkrokBledyPredykcji();
	std::shared_ptr<arma::vec> getWariancja();
	std::vector<std::pair<int,int>> getAlarmsInfo();
};

