#pragma once

class organizm;

class czlowiek;
class wilk;
class owca;
class lis;
class zolw;
class antylopa;

class trawa;
class mlecz;
class guarana;
class wilczeJagody;
class barszcz;


class swiat {
public:
	class pole {
	public:
		organizm* org;
		pole() {
			org = nullptr;
		}
	};
private:
	int sizeX, sizeY;
	void ustawOrganizmy();
	void wykonajTure();
	void rysujSwiat();
	int poczatkowaIloscOrganizmow();
	int wczytajWielocyfrowe(char* s, int& i);
	void wpiszWielocyfrowe(int x, FILE* f);
	pole* plansza;
	czlowiek* gracz;

	void wykonajSymulacje();
public:
	swiat();
	swiat(char* s);
	swiat(int x, int y);
	~swiat();
	
	organizm* klonuj(organizm* o, int p, swiat* ref);
	czlowiek* getGracz() { return gracz; };
	void setGracz(czlowiek* c) { gracz = c; };
	void getSize(int& x, int& y);
	
	void getCoords(int p, int& x, int& y);
	int getPolozenie(int x, int y);
	pole* getPole(int sprawdzanePole);

	void killOrganizm(organizm* o);
	void przesunOrganizm(int newPozycja, organizm* o);
	pole* getWolnePole(int p);
	void getSasiadow(int p, int tab[4]);
	int losujRuch(int p, int range);
	void zakonczGre();
	void zapiszDoPliku();
};