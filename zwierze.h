#pragma once
//#include "organizm.h"
//#include "swiat.h"

class swiat;
class organizm;

class zwierze : public organizm {
public:
	zwierze(int p, swiat* s) : organizm(p, s) {};
	virtual ~zwierze() = default;
	int kolizja(organizm* o);
	void atakuj(organizm* o, int p);
	void akcja() override;
	virtual char rysowanie() override = 0;
};