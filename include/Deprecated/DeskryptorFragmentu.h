#pragma once
#include "TypAlarmu.h"

class DeskryptorFragmentu
{
public:
	DeskryptorFragmentu(void);
	~DeskryptorFragmentu(void);
	int getPoczatek();
	int getKoniec();
	int getPoczatekAlarmu1();
	int getKoniecAlarmu1();
	int getPoczatekAlarmu2();
	int getKoniecAlarmu2();
	typAlarmu getTyp();

	void setPoczatek(int p);
	void setKoniec(int k);
	void setType(typAlarmu t);
	void setPoczatekAlarmu1(int p);
	void setPoczatekAlarmu2(int p);
	void setKoniecAlarmu1(int k);
	void setKoniecAlarmu2(int k);

private:
	int poczatekFragmentu;
	int koniecFragmentu;
	int poczatekAlarmu1;
	int koniecAlarmu1;
	int poczatekAlarmu2;
	int koniecAlarmu2;
	typAlarmu t;
};

