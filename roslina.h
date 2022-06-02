#pragma once
//#include "organizm.h"

class swiat;
class organizm;

class roslina : public organizm {
public:
	roslina(int p, swiat* s) : organizm(p, s) {};
	virtual ~roslina() = default;
	virtual int kolizja(organizm* o) = 0;
	void akcja() override;
	virtual char rysowanie() override = 0;
};