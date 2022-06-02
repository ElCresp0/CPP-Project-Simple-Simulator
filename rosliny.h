#pragma once

class swiat;
class organizm;
class roslina;

class trawa final: public roslina {
public:
	trawa(int p, swiat* s);
	int kolizja(organizm* o) override;
	char rysowanie() override;
};

class mlecz final : public roslina {
public:
	mlecz(int p, swiat* s);
	void akcja() override;
	int kolizja(organizm* o) override;
	char rysowanie() override;
};

class guarana final : public roslina {
public:
	guarana(int p, swiat* s);
	int kolizja(organizm* o) override;
	char rysowanie() override;
};

class wilczeJagody final : public roslina {
public:
	wilczeJagody(int p, swiat* s);
	int kolizja(organizm* o) override;
	char rysowanie() override;
};

class barszcz final : public roslina {
public:
	barszcz(int p, swiat* s);
	void akcja() override;
	int kolizja(organizm* o) override;
	char rysowanie() override;
};
