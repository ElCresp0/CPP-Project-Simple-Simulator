#pragma once

class swiat;

class organizm {
protected:
	int sila;
	int inicjatywa;
	int polozenie; // kazdemu polu odpowiada jedna liczba naturalna zamiast dwoch wspolrzednych
	swiat* swiatRef;
public:
	organizm(int p, swiat* s);
	virtual ~organizm() = default;
	void setSila(int s);
	void setPolozenie(int p);
	
	int getSila();
	int getInicjatywa();
	int getPolozenie();

	virtual int kolizja(organizm* o) = 0;
	virtual void akcja() = 0;
	virtual char rysowanie() = 0;
};