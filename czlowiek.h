#pragma once

class swiat;
class organizm;
class zwierze;

class czlowiek final : public zwierze {
private:
	int skillAktywny;
	int skillGotowy;
	void moveTo(int x,int y);
public:
	czlowiek(int p, swiat* s);
	czlowiek(int p, swiat* s, int aktywny, int gotowy, int silaRN);
	~czlowiek();

	void aktywacja();
	
	int getAktywnosc() { return skillAktywny; };
	int getGotowosc() { return skillGotowy; };
	void akcja() override;
	char rysowanie() override;
};