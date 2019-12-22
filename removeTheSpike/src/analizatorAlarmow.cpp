#include "../include/analizatorAlarmow.h"
#include "../include/DeskryptorFragmentu.h"
#include "../include/DataSaver.h"

analizatorAlarmow::analizatorAlarmow(int r, int k, int dlSyg) : kmax(k), dlSyg(dlSyg), r(r)
{
	this->delta = 2;
	this->kmax += delta;
}


analizatorAlarmow::~analizatorAlarmow(void)
{
}

std::shared_ptr<arma::vec> analizatorAlarmow::analizuj() {

	for(auto d : deskryptory) {
		auto typ = d.getTyp();
		if(typ == typAlarmu::d || typ == typAlarmu::a1 || typ == typAlarmu::a2 || typ == typAlarmu::a3 || typ == typAlarmu::a4 || typ == typAlarmu::a5) {
			rekonstrukcjaRegulaFF(d);
		} else if(typ == typAlarmu::b1 || typ == typAlarmu::b2) {
			rekonstrukcjaRegulaWypelnienie(d);
		} else if(typ == typAlarmu::c1 || typ == typAlarmu::c2) {
			rekonstrukcjaRegulaF(d);
		} else {
			std::cout <<"NIEWIADOMO" << std::endl;
		}
	}

	//DataSaver ds; 
	//ds.saveAlarms(krawedzieF, krawedzieB, nowyDetekt, nowyDetekt->size());
	return (nowyDetekt);
}

void analizatorAlarmow::rekonstrukcjaRegulaFF(DeskryptorFragmentu df) {
	int poczatekPierwszegoAlarmu = df.getPoczatekAlarmu1();
	int poczatekFragmentu = df.getPoczatek();
	if((*krawedzieF)(poczatekPierwszegoAlarmu) || (*krawedzieB)(poczatekPierwszegoAlarmu)) {
		int koniecAlarmuB = znajdzKoniecAlarmuB(df);
		int poczatekAlarmuF = znajdzPoczatekAlarmuF(df);
		int ile = koniecAlarmuB-poczatekAlarmuF;
		//std::cout << poczatekAlarmuF <<" " << koniecAlarmuB <<" "<<(int)df.getTyp() << std::endl;
		for(int i = 0; i < ile; i++) {
			if(poczatekAlarmuF+i < dlSyg) {
				(*nowyDetekt)(poczatekAlarmuF+i) = 1;
			}
		}
	} 
	else {
		std::cout <<"ZLE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<< std::endl;
	}
}

int analizatorAlarmow::znajdzPoczatekAlarmuF(DeskryptorFragmentu df) {
	int poczAlm1 = df.getPoczatekAlarmu1();
	int dlugosc = df.getKoniec() - df.getPoczatek() - r;

	for(int i = 0; i < dlugosc; i++) {
		if((*krawedzieF)(poczAlm1+i) == 1) {
			return poczAlm1+i;
		}
	}
	std::cout << "BLAD POCZ " << " " << df.getKoniec() <<" " <<df.getPoczatek() <<" " << (int)df.getTyp() << std::endl;
	return 50;
}

int analizatorAlarmow::znajdzKoniecAlarmuB(DeskryptorFragmentu df) {
	int koniecAlm2 = df.getKoniec() - r;
	int dlugosc = koniecAlm2 - df.getPoczatek();
	for(int i = 0; i < dlugosc; i++) {
		if((*krawedzieB)(koniecAlm2-i) == -1) {
			return koniecAlm2-i;
		}
	}
	std::cout << "BLAD KONC " << koniecAlm2 <<" "<<df.getPoczatekAlarmu1() <<" "<< (int)df.getTyp() << std::endl;
	return koniecAlm2;
}

void analizatorAlarmow::rozszerzAlarm(std::shared_ptr<arma::vec> &det) {
	int licznikJedynek= 0;
	for(int i = 0; i < det->size(); i++) {
		if((*det)(i) == 1 && (*det)(i-1) == 0) {
			for(int j = 1; j <= delta; j++) {
				(*det)(i-j) = 1; 
			}
		}
	}
}

void analizatorAlarmow::rekonstrukcjaRegulaF(DeskryptorFragmentu df) {
	int poczatekAlarmu = df.getPoczatekAlarmu1();
	int koniecAlarmu = df.getKoniecAlarmu1();
	auto ptr = krawedzieB;
	if((*krawedzieF)(poczatekAlarmu) == 1) {
		ptr = krawedzieF;
	} else if((*krawedzieB)(poczatekAlarmu) == 1) {
		ptr = krawedzieB;
	}
	int i = 0;
	for(; i < r; i++) {
		if((*ptr)(poczatekAlarmu+2*delta+1+i) == 1) {
			break;
		}
	}
	int roznica = (i-r <= 0) ? 0 : i - r;
	for(int j = 0; j < 2*delta-roznica+1; j++) {
		if(poczatekAlarmu+j < dlSyg) {
			(*nowyDetekt)(poczatekAlarmu+j) = 1;
		}
	}
}

void analizatorAlarmow::rekonstrukcjaRegulaWypelnienie(DeskryptorFragmentu df) {
	int poczatekPierwszegoAlarmu = df.getPoczatekAlarmu1();
	int koniecDrugiegoAlarmu = df.getKoniecAlarmu2();
	int poczatekFragmentu = df.getPoczatek();
	for(int i = 0; i < koniecDrugiegoAlarmu - poczatekPierwszegoAlarmu; i++) {
		if(poczatekPierwszegoAlarmu+i < dlSyg) {
			(*nowyDetekt)(poczatekPierwszegoAlarmu+i) = 1;
		}
	}
}

void analizatorAlarmow::poszerzAlarm(std::shared_ptr<arma::vec> &det) {
	for(int t = r+delta; t < det->size(); t++) {
		if((*det)(t) == 1) {
			int i = 1;
			for(; i <= r+delta; i++) {
				if((*det)(t-i) == -1) {
					break;
				}
			}
			i = i - 1;
			if(i >= r+delta) {
				(*det)(t-delta) = 1; 
				(*det)(t) = 0;
			}
			else if(i > r && i < r+delta ){
				(*det)(t-(i-r)) = 1;
				(*det)(t) = 0;
			}
		}
	}
}

void analizatorAlarmow::odwrocAlarm(std::shared_ptr<arma::vec> &det) {
	std::vector<double> tmp;
	for(int i = 0; i < det->size(); i++) {
		tmp.push_back((*det)(i));
	}
	std::reverse(tmp.begin(), tmp.end());
	det.reset();
	det = std::make_shared<arma::vec>(tmp);
	for(int i = 0; i < det->size(); i++) {
		if((*det)(i) == 1) {
			(*det)(i) = -1;
		} else if((*det)(i) == -1) {
			(*det)(i) = 1;
		}
	}
}

void analizatorAlarmow::odwrocDetektor(std::shared_ptr<arma::vec> &det) {
	std::vector<double> tmp;
	for(int i = 0; i < det->size(); i++) {
		tmp.push_back((*det)(i));
	}
	std::reverse(tmp.begin(), tmp.end());
	det.reset();
	det = std::make_shared<arma::vec>(tmp);
}

void analizatorAlarmow::wyznaczKrawedzie(std::shared_ptr<arma::vec> &detF, std::shared_ptr<arma::vec> &detB) {
	int size = 0;
	if(detF->size() == detB->size()) {
		size = detF->size();
	} else {
		std::cout <<"COS JEST NIE TAK" << std::endl;
		return;
	}
	//odwrocDetektor(detB);

	krawedzieF = std::make_shared<arma::vec>(size);
	krawedzieB = std::make_shared<arma::vec>(size);
	krawedzieB->fill(0);
	krawedzieF->fill(0);

	for(int i = 1; i < size; i++) {
		(*krawedzieB)(i) = (*detB)(i) - (*detB)(i-1);
		(*krawedzieF)(i) = (*detF)(i) - (*detF)(i-1);
	}

	poszerzAlarm(krawedzieF);
	poszerzAlarm(krawedzieB);

	odwrocAlarm(krawedzieB);
	odwrocDetektor(detB);
}

void analizatorAlarmow::podzielNaFragmenty() {
	int size = -1;
	if(krawedzieB->size() == krawedzieF->size()) {
		size = krawedzieB->size();
		std::cout << "size: " << size << std::endl;
		nowyDetekt = std::make_shared<arma::vec>(size);
		nowyDetekt->fill(0);
	}
	std::vector<DeskryptorFragmentu> wektorDeskryptorow;
	for(int t = r; t < size; t++)  {
		if((*krawedzieF)(t) == 1 && (*krawedzieB)(t) == 1) {
			if(sprCzyA(t, wektorDeskryptorow, krawedzieF, krawedzieB)) {
				t = wektorDeskryptorow.at(wektorDeskryptorow.size()-1).getKoniec() - 1;
				continue;	
			}
			if(sprCzyA(t, wektorDeskryptorow, krawedzieB, krawedzieF)) {
				t = wektorDeskryptorow.at(wektorDeskryptorow.size()-1).getKoniec() - 1;
				continue;	
			}
		}
		if((*krawedzieF)(t) == 1) {
			if(sprCzyA(t, wektorDeskryptorow, krawedzieF, krawedzieB)) {
				t = wektorDeskryptorow.at(wektorDeskryptorow.size()-1).getKoniec() - 1;
				continue;	
			}
			if(sprCzyB(t, wektorDeskryptorow, krawedzieF, krawedzieB)) {
				t = wektorDeskryptorow.at(wektorDeskryptorow.size()-1).getKoniec() - 1;
				continue;
			}
			if(sprCzyC(t, wektorDeskryptorow, krawedzieF, krawedzieB)) {
				t = wektorDeskryptorow.at(wektorDeskryptorow.size()-1).getKoniec() - 1;
				continue;
				}
			if(sprCzyD(t, wektorDeskryptorow, krawedzieF, krawedzieB)) {
				t = wektorDeskryptorow.at(wektorDeskryptorow.size()-1).getKoniec() - 1;
				continue;
				} else {
					std::cout <<"D zwraca false!" << std::endl;
				}

		} if((*krawedzieB)(t) == 1) {
			if(sprCzyA(t, wektorDeskryptorow, krawedzieB, krawedzieF)) {
				t = wektorDeskryptorow.at(wektorDeskryptorow.size()-1).getKoniec() - 1;
				continue;
				}
			if(sprCzyB(t, wektorDeskryptorow, krawedzieB, krawedzieF)) {
				t = wektorDeskryptorow.at(wektorDeskryptorow.size()-1).getKoniec() - 1;
				continue;
			}
			if(sprCzyC(t, wektorDeskryptorow, krawedzieB, krawedzieF)) {
				t = wektorDeskryptorow.at(wektorDeskryptorow.size()-1).getKoniec() - 1;
				continue;
			}
			if(sprCzyD(t, wektorDeskryptorow, krawedzieB, krawedzieF)) {
				t = wektorDeskryptorow.at(wektorDeskryptorow.size()-1).getKoniec() - 1;
				continue;
			} else {
				std::cout <<"D zwraca false!" << std::endl;
			}
		}
	}
	deskryptory = wektorDeskryptorow;
}

bool analizatorAlarmow::sprCzyB(int t, std::vector<DeskryptorFragmentu> &w, std::shared_ptr<arma::vec> &det1, std::shared_ptr<arma::vec> &det2) {
	int poczatekAlarmu = t;
	int poczatek = t-r;
	int koniecAlm1 = -1;
	DeskryptorFragmentu d;
	d.setPoczatek(poczatek);
	d.setPoczatekAlarmu1(poczatekAlarmu);
	int k = (2*kmax > det1->size() - t) ? 2*kmax : det1->size() - t;
	for(int i = 1; i < k; i++) {
		//alarmy powinny byc rozlaczne
		if((*det2)(poczatekAlarmu+i)) {
			return false;
		}
		if((*det1)(poczatekAlarmu+i) < 0) {
			koniecAlm1 = poczatekAlarmu+i;
			d.setKoniecAlarmu1(koniecAlm1);
			break;
		}
	}

	if(koniecAlm1 < 0) {
		koniecAlm1 = poczatekAlarmu + k - r;
	}

	for(int j = 0; j <= r; j++) {
		if((*det2)(koniecAlm1+j) == 1) {
			d.setPoczatekAlarmu2(koniecAlm1+j);
			for(int i = 0; i < kmax; i++) {
				if((*det2)(koniecAlm1+j+i) == -1) {
					for(int k = 1; k <= r; k++) {
						if((*det1)(koniecAlm1+j+i+k) == 1) {
							return false;
						}
					}
					d.setKoniecAlarmu2(koniecAlm1+j+i);
					d.setKoniec(koniecAlm1+j+i+r);
					d.setType(typAlarmu::b1);
					//std::cout << "B1/B2 " << d.getPoczatek() << " " << d.getKoniec() << std::endl;
					w.push_back(d);
					return true;
				}
			}
		}
	}
	return false;
}

bool analizatorAlarmow::sprCzyA(int t, std::vector<DeskryptorFragmentu> &w, std::shared_ptr<arma::vec> &det1, std::shared_ptr<arma::vec> &det2) {

	int poczatekAlarmu = t;
	int koniecAlarmu = -1;
	DeskryptorFragmentu d;
	d.setPoczatek(poczatekAlarmu-r);
	d.setPoczatekAlarmu1(poczatekAlarmu);
	int k = (2*kmax > det1->size() - t) ? det1->size() - t - 1: kmax;
	d.setPoczatek(poczatekAlarmu-r);
	//sprawdzenie czy jest alarm z jednej strony
	for(int i = 1; i <= kmax; i++) {
		if((*det1)(t+i) < 0) {
			koniecAlarmu = t+i;
			d.setKoniecAlarmu1(koniecAlarmu);
			break;
		}
	}
	if(koniecAlarmu < 0) {
		koniecAlarmu = poczatekAlarmu + k - r;
	}
	bool nieMawSrodku = true;
	for(int i = 1; i < koniecAlarmu-poczatekAlarmu; i++) {
		if((*det2)(poczatekAlarmu+i) == 1) {
			nieMawSrodku = false;
		}
	}
	if((*det2)(poczatekAlarmu) == 1 && (*det2)(koniecAlarmu) < 0 && nieMawSrodku) {
		d.setKoniec(koniecAlarmu+r);
		d.setType(typAlarmu::a1);
		w.push_back(d);
		//std::cout <<"A1 "<< d.getPoczatek() << " " << d.getKoniec() <<std::endl;
		return true;
	}

	bool zawieraPoczatek = false;
	for(int i = 0; i < koniecAlarmu-poczatekAlarmu; i++) {
		if((*det2)(poczatekAlarmu+i) > 0) {
			zawieraPoczatek = true;
			d.setPoczatekAlarmu2(poczatekAlarmu+i);
		}
		if(((*det2)(poczatekAlarmu+i) < 0 && zawieraPoczatek)) {
			int koniecAlarmu2 = poczatekAlarmu+i;
			bool czysto = true;
			int zakres = (koniecAlarmu-poczatekAlarmu)-(koniecAlarmu2-poczatekAlarmu);
			for(int j = 1; j <= r; j++) {
				if((*det1)(koniecAlarmu+j) == 1 || (*det2)(koniecAlarmu+j) == 1) {
					czysto = false;
					break;
				}
			}
			if(czysto) {
				d.setKoniecAlarmu2(koniecAlarmu2);
				d.setType(typAlarmu::a2);
				d.setKoniec(koniecAlarmu+r);
				//std::cout << "A2/A3 " << d.getPoczatek() << " " << d.getKoniec()<< std::endl;
				w.push_back(d);
				return true;
			} else {
				break;
			}
		}
	}
	bool czysty = true;
	for(int i = 1; i < koniecAlarmu-poczatekAlarmu; i++) {
		if((*det2)(poczatekAlarmu+i) == 1) {
			int poczAlm2 = poczatekAlarmu+i;
			d.setPoczatekAlarmu2(poczAlm2);
			for(int j = 1; j < kmax; j++) {
				if((*det2)(poczAlm2+j) == -1) {
					int koncAlarmu2 = poczAlm2+j;
					d.setKoniecAlarmu2(koncAlarmu2);
					d.setKoniec(koncAlarmu2+r);
					bool c = true;
					if(koncAlarmu2 < koniecAlarmu) {
 						c = false;
					}
					for(int z = 1; z < r; z++) {
						if((*det1)(koncAlarmu2+z) || (*det2)(koncAlarmu2+z)) {
							c = false;
							break;
						}
					}
					if(c) {
						//std::cout << "A4/A5 " << d.getPoczatek() << " " << d.getKoniec() << std::endl;
						d.setType(typAlarmu::a4);
						w.push_back(d);
						return true;
					} 
				}
			}
		}
	}
	return false;
}

bool analizatorAlarmow::sprCzyC(int t, std::vector<DeskryptorFragmentu> &w, std::shared_ptr<arma::vec> &det1, std::shared_ptr<arma::vec> &det2) {

	int poczatekFragmentu = -1;
	int koniecFragmentu = -1;
	//sprawdzenie czy jest alarm z jednej strony
	poczatekFragmentu = t-r;
	int k = (kmax > det1->size() - t) ? det1->size() - t - 1: kmax;
	for(int i = 1; i <= k; i++) {
		if((*det1)(t+i) < 0) {
			koniecFragmentu = t+i+r;
			break;
		}
	}
	if(koniecFragmentu < 0) {
		koniecFragmentu = t + k;
	}
	bool alarmPrawostronny = false;
	for(int i = 0; i < koniecFragmentu - poczatekFragmentu + 1; i++) {
		if((*det2)(poczatekFragmentu+i) > 0) {
				alarmPrawostronny = true;
				break;
		}
	}
	for(int i = 0; i < (koniecFragmentu-r) - (poczatekFragmentu+r); i++) {
		if((*det2)(poczatekFragmentu+r+i) < 0) {
			alarmPrawostronny = true;
			break;
		}
	}

	if(!alarmPrawostronny) {
		DeskryptorFragmentu d;
		d.setKoniec(koniecFragmentu);
		d.setPoczatek(poczatekFragmentu);
		d.setPoczatekAlarmu1(poczatekFragmentu+r);
		d.setKoniecAlarmu1(koniecFragmentu-r);
		d.setType(typAlarmu::c1);
		w.push_back(d); 
		//std::cout << "C " <<d.getPoczatek() <<" " << d.getKoniec()  << std::endl;
		return true;
	}
	return false;
}

bool analizatorAlarmow::sprCzyD(int t, std::vector<DeskryptorFragmentu> &w, std::shared_ptr<arma::vec> &det1, std::shared_ptr<arma::vec> &det2) {
	int poczatekAlarmu = t;
	DeskryptorFragmentu d;
	d.setPoczatek(t-r);
	d.setPoczatekAlarmu1(poczatekAlarmu);
	int dlugoscFragmentu = 0;
	int aktDet = 1;
	int koniecAlarmu = -1;
	bool zawieraKrotszy = false;
	while(dlugoscFragmentu < 2*kmax) {
		for(int i = 0; i < kmax+delta+r; i++) {
			if(aktDet == 1)	{
				if((*det1)(poczatekAlarmu+i) == -1)  {
					if(poczatekAlarmu+i < koniecAlarmu) {
						zawieraKrotszy = true;
						break;
					}
					koniecAlarmu = poczatekAlarmu+i;
					dlugoscFragmentu = koniecAlarmu - d.getPoczatek();
					aktDet = 2;
					for(int k = 0; k < koniecAlarmu - poczatekAlarmu + r; k++) {
						if((*det2)(koniecAlarmu+r-k) == 1) {
							poczatekAlarmu = koniecAlarmu+r-k;
							break;
						}
					}
					break;
				} 
			} else if(aktDet == 2) {
				if((*det2)(poczatekAlarmu+i) == -1) {
					if(poczatekAlarmu+i < koniecAlarmu) {
						zawieraKrotszy = true;
						break;
					}
					koniecAlarmu = poczatekAlarmu+i;
					dlugoscFragmentu = koniecAlarmu - d.getPoczatek();
					aktDet = 1;
					for(int k = 0; k < koniecAlarmu - poczatekAlarmu + r; k++) {
						if((*det1)(koniecAlarmu+r-k) == 1) {
							poczatekAlarmu = koniecAlarmu+r-k;
							break;
						}
					}
					break;
				} 
			}
		}
		if(zawieraKrotszy) {
			zawieraKrotszy = false;
			break;
		}
		int tmpKonc = 0;
		for(int x = 0; x < kmax+delta; x++) {
			if(aktDet == 1 && (*det1)(poczatekAlarmu+x) == -1) {
				tmpKonc = poczatekAlarmu+x;
				break;
			} else if(aktDet == 2 && (*det2)(poczatekAlarmu+x) == -1) {
				tmpKonc = poczatekAlarmu+x;
				break;
			}
		}
		bool nieZawiera = true;
		for(int b = 0; b < tmpKonc - poczatekAlarmu; b++) {
			if(aktDet == 1 && (*det2)(tmpKonc-b) == 1) {
				nieZawiera = false;
				poczatekAlarmu = tmpKonc-b;
				aktDet = 2;
				break;
			} else if(aktDet == 2 && (*det1)(tmpKonc-b) == 1) {
				poczatekAlarmu = tmpKonc-b;
				aktDet = 1;
				nieZawiera = false;
				break;
			}
		}
		bool czysto = true;
		for(int z = 0; z < r; z++) {
			if((*det1)(tmpKonc+z) == 1 || (*det2)(tmpKonc+z) == 1) {
				czysto = false;
				break;
			}
		}
		if(czysto && nieZawiera) {
			koniecAlarmu = tmpKonc;
			break;
		}
	}
	for(int i = 0; i < r; i++) {
		if((*det1)(koniecAlarmu+i) == 1 || (*det2)(koniecAlarmu+i) == 1) {
			return false;
		}
	}
	if(dlugoscFragmentu >= 2*kmax) {
		dlugoscFragmentu = 2*kmax;
		d.setKoniecAlarmu2(d.getPoczatekAlarmu1()+dlugoscFragmentu);
		d.setKoniec(d.getPoczatekAlarmu1()+dlugoscFragmentu+r);
		std::cout << "Dmax: "<< d.getPoczatek() <<" "<<d.getKoniec() << std::endl;
		bool czyszcz1 = false;
		bool czyszcz2 = false;
		for(int i = 0; i < kmax; i++) {
			if((*det1)(d.getKoniecAlarmu2()+i) == 1) {
				czyszcz1 = true;
			}
			if((*det2)(d.getKoniecAlarmu2()+i) == 1) {
				czyszcz2 = true;
			}
			if(!czyszcz1 && (*det1)(d.getKoniecAlarmu2()+i) == -1) {
				(*det1)(d.getKoniecAlarmu2()+i) = 0;
			}
			if(!czyszcz2 && (*det2)(d.getKoniecAlarmu2()+i) == -1) {
				(*det2)(d.getKoniecAlarmu2()+i) = 0;
			}
			
		}
		d.setType(typAlarmu::d);
		w.push_back(d);
		//std::cout << "D! " << d.getPoczatekAlarmu1() << std::endl;
		return true;
	} else {
		d.setKoniec(koniecAlarmu+r);
		d.setKoniecAlarmu2(koniecAlarmu+r);
		d.setType(typAlarmu::d);
		//std::cout << "D! " << d.getPoczatekAlarmu1() << std::endl;
		w.push_back(d);
		return true;
	}
}

std::shared_ptr<arma::vec> analizatorAlarmow::getDetFromKraw(std::shared_ptr<arma::vec> &krawedzie) {
	int size = krawedzie->size();
	std::shared_ptr<arma::vec> det = std::make_shared<arma::vec>(size);
	det->fill(0);
	for(int i = 0; i < size; i++) {
		if((*krawedzie)(i) == 1) {
			int t = 0;
			while((*krawedzie)(i+t) != -1) {
				(*det)(i+t) = 1;
				t++;
			}
			(*det)(i+t) = 1;
			i += t + 1;
			t = 0;
		}
	}
	return det;
}

std::shared_ptr<arma::vec> analizatorAlarmow::polaczAlarmySuma(std::shared_ptr<arma::vec> &det1, std::shared_ptr<arma::vec> &det2) {
	int size = det1->size();
	auto ret = std::make_shared<arma::vec>(size);
	ret->fill(0);
	for(int i = 0; i < size; i++) {
		if((*det1)(i) > 0 || (*det2)(i) > 0)  {
			(*ret)(i) = 1;
		}
	}
	return ret;
}

std::shared_ptr<arma::vec> analizatorAlarmow::polaczAlarmyIloczyn(std::shared_ptr<arma::vec> &det1, std::shared_ptr<arma::vec> &det2) {
	int size = det1->size();
	auto ret = std::make_shared<arma::vec>(size);
	ret->fill(0);
	for(int i = 0; i < size; i++) {
		if((*det1)(i) > 0 && (*det2)(i) > 0)  {
			(*ret)(i) = 1;
		}
	}
	return ret;
}

