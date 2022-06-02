#pragma once

class swiat;
class organizm;
class zwierze;

class wilk final: public zwierze {
public:
	wilk(int p, swiat* s);
	char rysowanie() override;
};

class owca final : public zwierze {
public:
	owca(int p, swiat* s);
	char rysowanie() override;
};

class lis final : public zwierze {
public:
	lis(int p, swiat* s);
	void akcja() override;
	char rysowanie() override;
};

class zolw final : public zwierze {
public:
	void akcja() override;
	zolw(int p, swiat* s);
	int kolizja(organizm* o) override;
	char rysowanie() override;
};

class antylopa final : public zwierze {
public:
	void akcja() override;
	antylopa(int p, swiat* s);
	int kolizja(organizm* o) override;
	char rysowanie() override;
};