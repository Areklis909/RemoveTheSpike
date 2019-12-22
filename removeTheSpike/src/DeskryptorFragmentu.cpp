#include "../include/DeskryptorFragmentu.h"


DeskryptorFragmentu::DeskryptorFragmentu(void)
{
	poczatekFragmentu = -1;
	koniecFragmentu = -1;
	t = typAlarmu::niewiadomo;
}


DeskryptorFragmentu::~DeskryptorFragmentu(void)
{
}

int DeskryptorFragmentu::getKoniec() {
	return this->koniecFragmentu;
}

int DeskryptorFragmentu::getPoczatek() {
	return this->poczatekFragmentu;
}

typAlarmu DeskryptorFragmentu::getTyp() {
	return this->t;
}

void DeskryptorFragmentu::setKoniec(int k) {
	koniecFragmentu = k;
}

void DeskryptorFragmentu::setPoczatek(int p) {
	poczatekFragmentu = p;
}

void DeskryptorFragmentu::setType(typAlarmu type) {
	t = type;
}

void DeskryptorFragmentu::setKoniecAlarmu1(int k) {
	koniecAlarmu1 = k;
}

void DeskryptorFragmentu::setKoniecAlarmu2(int k) {
	koniecAlarmu2 = k;
}

void DeskryptorFragmentu::setPoczatekAlarmu1(int p) {
	poczatekAlarmu1 = p;
}

void DeskryptorFragmentu::setPoczatekAlarmu2(int p) {
	poczatekAlarmu2 = p;
}

int DeskryptorFragmentu::getPoczatekAlarmu1() {
	return poczatekAlarmu1;
}

int DeskryptorFragmentu::getKoniecAlarmu1() {
	return koniecAlarmu1;
}

int DeskryptorFragmentu::getPoczatekAlarmu2() {
	return poczatekAlarmu2;
}

int DeskryptorFragmentu::getKoniecAlarmu2() {
	return koniecAlarmu2;
}