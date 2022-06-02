#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <conio.h>
#include <string>

#include "swiat.h"
#include "organizm.h"
#include "zwierze.h"
#include "roslina.h"
#include "czlowiek.h"
#include "zwierzeta.h"
#include "rosliny.h"

#define MIN_LENGTH 4
#define MIN_SIZE 20
#define MAX_LENGTH 99
#define SKILL 5
#define STR_LENGTH 10000

#define STANDARDOWY_ZASIEG 1
#define ZASIEG_ANTYLOPY 2

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define CYFRY_ASCII 48
#define MAX_CYFROWOSC 9

enum wynikiKolizji {
	WYGRANA,
	DEAD,
	BOTH_DEAD,
	ODWROT,
	UCIECZKA,
	MNOZENIE
};

using namespace std;

//metody klasy swiat:
swiat::swiat(char* s) {
	int i = 0, size, aktywny, gotowy, silaRN;
	srand(NULL); 
	
	sizeX = wczytajWielocyfrowe(s, i);
	sizeY = wczytajWielocyfrowe(s, i);

	size = sizeX * sizeY;
	plansza = new pole[size];
	gracz = nullptr;
	for (int j = 0; j < size; j++) {
		if (s[i] >= CYFRY_ASCII && s[i] <= CYFRY_ASCII + 9) { // cyfry oznaczaja puste pola
			j += (wczytajWielocyfrowe(s, i) - 1);
		}
		else {
			i++;
			switch (s[i - 1]) {
			case 'C':
				aktywny = s[i++] - CYFRY_ASCII;
				gotowy = s[i++] - CYFRY_ASCII;
				cout << "pole: " << j << " i przed/po czlowieku: " << i << " / ";
				silaRN = wczytajWielocyfrowe(s, i);
				cout << i << endl;
				plansza[j].org = new czlowiek(j, this, aktywny, gotowy , silaRN);
				break;
			case 'W':
				plansza[j].org = new wilk(j, this);
				break;
			case 'O':
				plansza[j].org = new owca(j, this);
				break;
			case 'L':
				plansza[j].org = new lis(j, this);
				break;
			case 'Z':
				plansza[j].org = new zolw(j, this);
				break;
			case 'A':
				plansza[j].org = new antylopa(j, this);
				break;
			case 'T':
				plansza[j].org = new trawa(j, this);
				break;
			case 'M':
				plansza[j].org = new mlecz(j, this);
				break;
			case 'G':
				plansza[j].org = new guarana(j, this);
				break;
			case 'J':
				plansza[j].org = new wilczeJagody(j, this);
				break;
			case 'B':
				plansza[j].org = new barszcz(j, this);
				break;
			default:
				rysujSwiat();
				cout << "niepoprawny format (" << s[i - 1] << " " << s[i - 2] << "), i=" << i << ", j=" << j << "\n";
				zakonczGre();
				break;
			}
		}
	}
	cout << endl;
	if (((string)s).length() > i || gracz == nullptr) {
		cout << "niepoprawny format (length)\n";
		rysujSwiat();
		zakonczGre();
	}
	wykonajSymulacje();
	cout << "\n\nGAME OVER";
}

swiat::swiat(int x, int y) {
	while (x < MIN_LENGTH || y < MIN_LENGTH || x > MAX_LENGTH || y > MAX_LENGTH || x * y < MIN_SIZE) {
		cout << "za male wymiary (dlugosc boku: " << MIN_LENGTH << " - " << MAX_LENGTH <<", minimalny iloczyn: " << MIN_SIZE << ")\n";
		cin >> x >> y;
	}
	srand(NULL);
	sizeX = x;
	sizeY = y;
	plansza = new pole[x * y];
	gracz = nullptr; // ustawiane w konstruktorze klasy czlowiek
	ustawOrganizmy();
	wykonajSymulacje();
}

swiat::~swiat() {
	if (plansza) {
		for (int i = 0; i < sizeX * sizeY; i++) {
			if (plansza[i].org != nullptr) killOrganizm(plansza[i].org);
		}
		delete[] plansza;
	}
	exit(EXIT_SUCCESS);
}

organizm* swiat::klonuj(organizm* o, int p, swiat* ref) {
	if (o->rysowanie() == 'W') return new wilk(p, ref);
	else if (o->rysowanie() == 'O') return new owca(p, ref);
	else if (o->rysowanie() == 'L') return new lis(p, ref);
	else if (o->rysowanie() == 'A') return new antylopa(p, ref);
	else if (o->rysowanie() == 'Z') return new zolw(p, ref);
	else if (o->rysowanie() == 'T') return new trawa(p, ref);
	else if (o->rysowanie() == 'M') return new mlecz(p, ref);
	else if (o->rysowanie() == 'G') return new guarana(p, ref);
	else if (o->rysowanie() == 'J') return new wilczeJagody(p, ref);
	else if (o->rysowanie() == 'B') return new barszcz(p, ref);
}

int swiat::poczatkowaIloscOrganizmow() {
	int s = sizeX * sizeY * 3 / 5;
	if (s < MIN_SIZE) return MIN_SIZE;
	return s;
}

void swiat::ustawOrganizmy() {
	int size = sizeX * sizeY;
	int s = poczatkowaIloscOrganizmow();
	int x;
	for (int i = 0; i < s; i++) {
		x = rand() % size;
		while (plansza[x].org) x = ((x + 1) % size);
		if (i == 0) plansza[x].org = new czlowiek(x, this);
		else if (i % 10 == 0) plansza[x].org = new wilk(x, this);
		else if (i % 10 == 1) plansza[x].org = new owca(x, this);
		else if (i % 10 == 2) plansza[x].org = new lis(x, this);
		else if (i % 10 == 3) plansza[x].org = new zolw(x, this);
		else if (i % 10 == 4) plansza[x].org = new antylopa(x, this);
		else if (i % 10 == 5) plansza[x].org = new trawa(x, this);
		else if (i % 10 == 6) plansza[x].org = new mlecz(x, this);
		else if (i % 10 == 7) plansza[x].org = new guarana(x, this);
		else if (i % 10 == 8) plansza[x].org = new wilczeJagody(x, this);
		else if (i % 10 == 9) plansza[x].org = new barszcz(x, this);
	}
}

void swiat::przesunOrganizm(int newPozycja, organizm* o) {
	if (plansza[o->getPolozenie()].org != o || plansza[newPozycja].org != nullptr) {
		cout << "error: swiat::przesunOrganizm()";
		zakonczGre();
	}
	plansza[o->getPolozenie()].org = nullptr;
	plansza[newPozycja].org = o;
	o->setPolozenie(newPozycja);
}

void swiat::killOrganizm(organizm* o) {
	if (o == nullptr) return;
	for (int i = 0; i < sizeX * sizeY; i++) {
		if (plansza[i].org == o) {
			delete plansza[i].org;
			plansza[i].org = nullptr;
		}
	}
}

void swiat::getSasiadow(int p, int tab[4]) {
	int x, y;
	getCoords(p, x, y);
	int i = 0;
	tab[0] = tab[1] = tab[2] = tab[3] = -1;
	if (y > 0) tab[i++] = p - sizeX;
	if (y < sizeY - 1) tab[i++] = p + sizeX;
	if (x > 0) tab[i++] = p - 1;
	if (x < sizeX - 1) tab[i++] = p + 1;
}

void swiat::getSize(int& x, int& y) {
	x = sizeX;
	y = sizeY;
}

void swiat::getCoords(int p, int& x, int& y) {
	for (x = 0; x < sizeX; x++) {
		for (y = 0; y < sizeY; y++) {
			if (p == getPolozenie(x, y)) return;
		}
	}
	cout << "error: getCoords\n";
}

int swiat::getPolozenie(int x, int y) {
	return y * sizeX + x;
}

int swiat::losujRuch(int p, int range) {
	int x, y;
	getCoords(p, x, y);
	int i = 0;
	int dirs[4] = { 0,0,0,0 };
	if (y - range >= 0) dirs[i++] = p - (range * sizeX);
	if (y + range < sizeY) dirs[i++] = p + (range * sizeX);
	if (x - range >= 0) dirs[i++] = p - range;
	if (x + range < sizeX) dirs[i++] = p + range;
	i = rand() % i;
	return dirs[i];
}

swiat::pole* swiat::getPole(int sprawdzanePole) {
	return &(plansza[sprawdzanePole]);
}

swiat::pole* swiat::getWolnePole(int p) {
	int x, y;
	getCoords(p, x, y);
	int i = 0;
	int dirs[4];
	if (y > 0 && !(getPole(p - sizeX)->org)) dirs[i++] = p - sizeX;
	if (y < sizeY - 1 && !(getPole(p + sizeX)->org)) dirs[i++] = p + sizeX;
	if (x > 1 && !(getPole(p - 1)->org)) dirs[i++] = p - 1;
	if (x < sizeX && !(getPole(p + 1)->org)) dirs[i++] = p + 1;
	if (i == 0) return nullptr;
	i = rand() % i;
	return getPole(dirs[i]);
}

void swiat::rysujSwiat() {
	cout << flush;
	system("CLS");
	cout << "Jakub Kilianczyk 184301\n";
	cout << "obsluga symulatora: s-akcja specjalna, q-wyjscie, z-zapis stanu symulatora do pliku, ruchy strzalkami\n";
	for (int y = 0; y < sizeY; y++) {
		cout << ' ';
		for (int x = 0; x < sizeX - 1; x++) {
			if (plansza[getPolozenie(x, y)].org == nullptr) cout << "  | ";
			else cout << plansza[getPolozenie(x, y)].org->rysowanie() << " | ";
		}
		if (plansza[y * sizeX + sizeX - 1].org != nullptr) cout << plansza[y * sizeX + sizeX - 1].org->rysowanie();
		if (y < sizeY - 1) {
			cout << endl;
			cout << "_";
			for (int x = 0; x < sizeX - 1; x++) {
				cout << "_   ";
			}
			cout << "__\n\n";
		}
	}
}

void swiat::wykonajSymulacje() {
	while (gracz) {
		rysujSwiat();
		wykonajTure();
	}
	rysujSwiat();
	cout << endl << "GAME OVER";
}

void swiat::wykonajTure() {
	organizm** tab = new organizm * [sizeY * sizeX];//(organizm**)malloc(sizeX * sizeY * sizeof(organizm*));//new (organizm*)[sizeX * sizeY];
	int k = 0;
	for (int i = 7; i >= 0; i--) {
		if (i == 6) i--;
		if (i == 3) i -= 2;
		for (int j = 0; j < sizeX * sizeY; j++) {
			if (plansza[j].org && plansza[j].org->getInicjatywa() == i) tab[k++] = plansza[j].org;
		}
		while (k > 0) {
			k--;
			for (int j = 0; j < sizeX * sizeY; j++) {
				if (plansza[j].org == tab[k]) {
					tab[k]->akcja();
					break;
				}
			}
		}
	}
	delete[] tab;
}

int swiat::wczytajWielocyfrowe(char* s, int& i) {
	int wynik = 0;
	int cyfrowosc = s[i++] - CYFRY_ASCII;
	int modulo = 1;
	for (int j = 0; j < cyfrowosc; j++) modulo *= 10;
	for (int j = cyfrowosc; j >= 0; j--) {
		wynik += modulo * (s[i++] - CYFRY_ASCII);
		modulo /= 10;
	}

	return wynik;
}

void swiat::wpiszWielocyfrowe(int x, FILE* f) {
	int cyfry[MAX_CYFROWOSC + 1]; // cyfry[0] - cyfra jednosci x, cyfry[1] - cyfra dziesiatek x, itd. do duzych liczb
	unsigned long long modulo = 10000000000;
	int i;
	
	if (x > modulo) {
		cout << "blad: za duza liczba w argumencie funkcji wpiszWielocyfrowe";
		zakonczGre();
	}
	modulo = 1;
	for (i = 0; i <= MAX_CYFROWOSC; i++) {
		cyfry[i] = 0;
		modulo *= 10;
		while (x % modulo != 0) {
			x -= (modulo / 10);
			cyfry[i]++;
		}
	}
	
	i = MAX_CYFROWOSC;
	while (cyfry[i] == 0 && i > 0) i--;
	fputc(CYFRY_ASCII + i, f); //informacja o pozycji najbardziej znaczacej cyfry w systemie dziesietnym
	for (; i >= 0; i--) {
		fputc(CYFRY_ASCII + cyfry[i], f); //wpisywanie kolejnych liczb
	}
}

void swiat::zakonczGre() {
	delete this;
}

void swiat::zapiszDoPliku() {
	string nazwa;
	FILE* f;
	int x = 0;

	do {
		cout << "podaj nazwe pliku do zapisu\n";
		cin >> nazwa;
		nazwa.append(".txt");
		f = fopen(nazwa.c_str(), "w");
		if (f == NULL) {
			cout << "otwarcie lub utworzenie pliku nie powiodlo sie :(\nsprawdz, czy nazwa pliku jest prawidlowa\n";
		}
	} while (f == NULL);
	
	wpiszWielocyfrowe(sizeX, f);
	wpiszWielocyfrowe(sizeY, f);

	for (int i = 0; i < sizeX * sizeY; i++) {
		if (plansza[i].org == nullptr) {
			x = i;
			while (plansza[i].org == nullptr) i++;
			wpiszWielocyfrowe((i - x), f);
			i--;
		}
		else {
			fputc(plansza[i].org->rysowanie(), f);
			if (plansza[i].org->rysowanie() == 'C') {
				fputc(CYFRY_ASCII + ((czlowiek*)plansza[i].org)->getAktywnosc(), f);
				fputc(CYFRY_ASCII + ((czlowiek*)plansza[i].org)->getGotowosc(), f);
				wpiszWielocyfrowe(plansza[i].org->getSila(), f);
			}
		}
	}
	fclose(f);
}

//metody klasy organizm:
organizm::organizm(int p, swiat* s) {
	polozenie = p;
	swiatRef = s;
}

//organizm::~organizm() {};

void organizm::setSila(int s) {
	sila = s;
}

void organizm::setPolozenie(int p) {
	polozenie = p;
}

int organizm::getSila() {
	return sila;
}

int organizm::getInicjatywa() {
	return inicjatywa;
}

int organizm::getPolozenie() {
	return polozenie;
}


//metody klasy zwierze:
void zwierze::akcja() {
	int newPozycja = (swiatRef->losujRuch(polozenie, STANDARDOWY_ZASIEG));
	if (swiatRef->getPole(newPozycja)->org == nullptr) {
		swiatRef->przesunOrganizm(newPozycja, this);
	}
	else atakuj(swiatRef->getPole(newPozycja)->org, newPozycja);
}

void zwierze::atakuj(organizm* o, int p) {
	int wynik = o->kolizja(this);
	switch (wynik) {
	case WYGRANA:
		swiatRef->killOrganizm(o);
		swiatRef->przesunOrganizm(p, this);
		break;
	case DEAD:
		swiatRef->killOrganizm(this);
		break;
	case BOTH_DEAD:
		swiatRef->killOrganizm(o);
		swiatRef->killOrganizm(this);
		break;
	case ODWROT:
		return;
	case UCIECZKA:
		swiatRef->przesunOrganizm(p, this);
		break;
	case MNOZENIE:
		return; //obsluzone w kolizji drugiego organizmu
	default:
		cout << "blad: kolizja";
		swiatRef->zakonczGre();
	}
}

int zwierze::kolizja(organizm* o) {
	if (this->rysowanie() == o->rysowanie()) {
		swiat::pole* domDziecka = swiatRef->getWolnePole(polozenie);
		if (domDziecka != nullptr) {
			int i = 0;
			while (swiatRef->getPole(i) != domDziecka) i++;
			domDziecka->org = swiatRef->klonuj(this, i, swiatRef);
		}
		return MNOZENIE;
	}
	else {
		if (sila < o->getSila()) return WYGRANA;
		else if (sila == o->getSila()) {
			int los = rand() % 2;
			if (los == 0) return WYGRANA;
			else return DEAD;
		}
		else return DEAD;
	}
}

//metody klasy roslina:
//roslina::~roslina() {};
void roslina::akcja() {
	int prawdopodobienstwoZasiania = rand() % 10;
	if (prawdopodobienstwoZasiania > 7) {
		swiat::pole* p = swiatRef->getWolnePole(polozenie);
		if (p) {
			int i = 0;
			while (swiatRef->getPole(i) != p) i++;
			(p->org) = swiatRef->klonuj(this, i, swiatRef);
			p->org->setPolozenie(i);
		}
	}
}


//metody klasy czlowiek:
czlowiek::czlowiek(int p, swiat* s) : zwierze(p, s) {
	skillAktywny = 0;
	skillGotowy = 0;
	sila = 5;
	inicjatywa = 4;
	if (swiatRef->getGracz() == nullptr) swiatRef->setGracz(this);
	else {
		cout << "\nna swiecie moze byc tylko jeden czlowiek!";
		swiatRef->zakonczGre();
	}
}

czlowiek::czlowiek(int p, swiat* s, int aktywny, int gotowy, int silaRN) : zwierze(p, s) {
	if (silaRN > 10 || silaRN < 5 || aktywny > 5 || aktywny < 0 || gotowy > 10 || gotowy < 0) {
		cout << "blad odczytu stanu gry\nsila: " << silaRN << ", aktywny: " << aktywny << ", gotowy: " << gotowy;
		s->zakonczGre();
	}
	skillAktywny = aktywny;
	skillGotowy = gotowy;
	sila = silaRN;
	inicjatywa = 4;
	if (swiatRef->getGracz() == nullptr) swiatRef->setGracz(this);
	else {
		cout << "\nna swiecie moze byc tylko jeden czlowiek!";
		swiatRef->zakonczGre();
	}
}

czlowiek::~czlowiek() {
	swiatRef->setGracz(nullptr);
	//swiatRef->zakonczGre();
}

char czlowiek::rysowanie() {
	return 'C';
}

void czlowiek::moveTo(int x, int y) {
	int p = swiatRef->getPolozenie(x, y);
	if (swiatRef->getPole(p)->org == nullptr) {
		swiatRef->przesunOrganizm(p, this);
	}
	else {
		cout << "atakuje: " << swiatRef->getPole(p)->org->rysowanie() << "; ";
		if (swiatRef->getPole(p)->org->rysowanie() == 'G') cout << "zjedzono guarane: sila wzrasta o 3pkt; ";
		else if (swiatRef->getPole(p)->org->rysowanie() == 'J') cout << "zjedzono wilcze jagody: koniec gry; ";
		atakuj(swiatRef->getPole(p)->org, p);
	}
}

void czlowiek::aktywacja() {
	if (skillAktywny == 0 && skillGotowy == 0) {
		skillAktywny = SKILL;
		skillGotowy = SKILL;
		sila += SKILL;
		cout << "aktywowano umiejetnosc; ";
	}
	else cout << "nie mozna jeszcze aktywowac umiejetnosci; ";
}

void czlowiek::akcja() {
	char input;
	int x, y, sizeX, sizeY;

	if (skillAktywny > 0) {
		sila--;
		skillAktywny--;
	}
	else if (skillGotowy > 0) skillGotowy--;

	swiatRef->getSize(sizeX, sizeY);
	swiatRef->getCoords(polozenie, x, y);

	cout << endl << "sila: " << sila << "; ";
	cout << "koordynaty: " << x + 1 << ", " << y + 1 << "; ";

	while (true) {
		input = _getch();
		if (input == KEY_UP && y > 0) {
			moveTo(x, y - 1);
			break;
		}
		else if (input == KEY_DOWN && y < sizeY - 1) {
			moveTo(x, y + 1);
			break;
		}
		else if (input == KEY_LEFT && x > 0) {
			moveTo(x - 1, y);
			break;
		}
		else if (input == KEY_RIGHT && x < sizeX - 1) {
			moveTo(x + 1, y);
			break;
		}
		else if (input == 'q') {
			swiatRef->zakonczGre();
		}
		else if (input == 's') aktywacja();
		else if (input == 'z') swiatRef->zapiszDoPliku();
		//else cout << "nie ma komendy '" << input << "'\n";
	}
}


//metody klas zwierzat:
//wilk:
wilk::wilk(int p, swiat* s) : zwierze(p, s) {
	sila = 9;
	inicjatywa = 5;
}

char wilk::rysowanie() {
	return 'W';
}

//owca:
owca::owca(int p, swiat* s) : zwierze(p, s) {
	sila = 4;
	inicjatywa = 4;
}

char owca::rysowanie() {
	return 'O';
}

//lis:
lis::lis(int p, swiat* s) : zwierze(p, s) {
	sila = 3;
	inicjatywa = 7;
}

char lis::rysowanie() {
	return 'L';
}

void lis::akcja() {
	int sasiedzi[4];
	swiatRef->getSasiadow(polozenie, sasiedzi);
	int j = rand() % 4;
	for (int i = 0; i < 4; i++) {
		if (sasiedzi[j] != -1) {
			if (swiatRef->getPole(sasiedzi[j])->org == nullptr) {
				swiatRef->przesunOrganizm(sasiedzi[j], this);
			}
			else if (swiatRef->getPole(sasiedzi[j])->org->getSila() <= sila) {
				atakuj(swiatRef->getPole(sasiedzi[j])->org, sasiedzi[j]);
			}
		}
		j = ((j + 1) % 4);
	}
}

//zolw:
zolw::zolw(int p, swiat* s) : zwierze(p, s) {
	sila = 2;
	inicjatywa = 1;
}

char zolw::rysowanie() {
	return 'Z';
}

void zolw::akcja() {
	int prawdopodobienstwo = rand() % 4;
	if (prawdopodobienstwo == 0) {
		zwierze::akcja();
	}
}

int zolw::kolizja(organizm* o) {
	if (o->rysowanie() == rysowanie()) return zwierze::kolizja(o);
	else if (o->getSila() < 5) return ODWROT;
	else return zwierze::kolizja(o);
}

//antylopa:
antylopa::antylopa(int p, swiat* s) : zwierze(p, s) {
	sila = 4;
	inicjatywa = 4;
}

char antylopa::rysowanie() {
	return 'A';
}

void antylopa::akcja() {
	int newPozycja = (swiatRef->losujRuch(getPolozenie(), ZASIEG_ANTYLOPY));
	if (swiatRef->getPole(newPozycja)->org == nullptr) {
		swiatRef->przesunOrganizm(newPozycja, this);
	}
	else atakuj(swiatRef->getPole(newPozycja)->org, newPozycja);
}

int antylopa::kolizja(organizm* o) {
	if (rysowanie() == o->rysowanie()) {
		return zwierze::kolizja(o);
	}
	int prawdopodobienstwo = rand() % 2;
	if (prawdopodobienstwo == 0) {
		swiat::pole* p = swiatRef->getWolnePole(polozenie);
		if (p != nullptr) {
			int i = 0;
			while (swiatRef->getPole(i) != p) i++;
			swiatRef->przesunOrganizm(i, this);
			return UCIECZKA;
		}
		else return zwierze::kolizja(o);
	}
	else return zwierze::kolizja(o);
}


//metody klas roslin:
//trawa:
trawa::trawa(int p, swiat* s) : roslina(p, s) {
	sila = 0;
	inicjatywa = 0;
}
//trawa::~trawa() { cout << "trawa is ded\n"; }
char trawa::rysowanie() { return 'T'; }

int trawa::kolizja(organizm* o) {
	return WYGRANA;
}

//mlecz:
mlecz::mlecz(int p, swiat* s) : roslina(p, s) {
	sila = 0;
	inicjatywa = 0;
}

char mlecz::rysowanie() { return 'M'; }

void mlecz::akcja() {
	for (int i = 0; i < 3; i++) roslina::akcja();
}

int mlecz::kolizja(organizm* o) {
	return WYGRANA;
}

//guarana:
guarana::guarana(int p, swiat* s) : roslina(p, s) {
	sila = 0;
	inicjatywa = 0;
}

char guarana::rysowanie() { return 'G'; }

int guarana::kolizja(organizm* o) {
	o->setSila(o->getSila() + 3);
	return WYGRANA;
}

//wilcze jagody:
wilczeJagody::wilczeJagody(int p, swiat* s) : roslina(p, s) {
	sila = 99;
	inicjatywa = 0;
}

char wilczeJagody::rysowanie() { return 'J'; }

int wilczeJagody::kolizja(organizm* o) {
	return BOTH_DEAD;
	//swiatRef->killOrganizm(o);
}

//barszcz:
barszcz::barszcz(int p, swiat* s) : roslina(p, s) {
	sila = 10;
	inicjatywa = 0;
}

char barszcz::rysowanie() { return 'B'; }

void barszcz::akcja() {
	roslina::akcja();
	int sasiedzi[4];
	swiatRef->getSasiadow(polozenie, sasiedzi);
	for (int i = 0; i < 4; i++) {
		if (sasiedzi[i] != -1 && swiatRef->getPole(sasiedzi[i])->org != nullptr && swiatRef->getPole(sasiedzi[i])->org->getInicjatywa() > 0)
			swiatRef->killOrganizm(swiatRef->getPole(sasiedzi[i])->org);
	}
}

int barszcz::kolizja(organizm* o) {
	return BOTH_DEAD;
	//swiatRef->killOrganizm(o);
}



swiat::swiat() {
	char input;
	srand(NULL);
	cout << "Jakub Kilianczyk 184301\nsymulator\nn-utworz nowy swiat\nw-wczytaj zapisany swiat\nq-zamknij symulator\n";

	while (true) {
		cin >> input;
		if (input == 'n') {
			int x, y;
			cout << "podaj wymiary planszy (x,y dlugosc boku: " << MIN_LENGTH << " - " << MAX_LENGTH << ", minimalny iloczyn: " << MIN_SIZE << ")\n";
			cin >> x >> y;
			*this = swiat(x, y);
			//swiat sw(x, y);
			return;
		}
		else if (input == 'w') {
			string str;
			cout << "podaj nazwe pliku z zapisanym stanem gry\n";
			cin >> str;
			str.append(".txt");
			FILE* f = fopen(str.c_str(), "r");
			if (!f) {
				cout << "fail";
				zakonczGre();
			}
			else {
				char s[STR_LENGTH];
				fgets(s, STR_LENGTH - 1, f);
				//fscanf(f, str.c_str());
				*this = swiat(s);
				fclose(f);
				return;
			}
		}
		else if (input == 'q') return;
		else cout << "nieprawidlowa komenda\n";
	}



	/*sizeX = sizeY = 5;
	plansza = new pole[5 * 5];
	plansza[0].org = new czlowiek(0, this);
	//plansza[10].org = new wilk(10, this);
	plansza[5].org = new antylopa(5, this);
	plansza[10].org = new lis(10, this);
	//plansza[45].org = new zolw(50, this);
	//plansza[2].org = new trawa(2, this);
	//plansza[20].org = new guarana(20, this);
	plansza[15].org = new wilczeJagody(15, this);
	plansza[24].org = new barszcz(24, this);
	plansza[20].org = new barszcz(20, this);*/
	
	//*this = swiat(9, 9);
	//wykonajSymulacje();
}


int main() {
	swiat sw;
	
	return 0;
}