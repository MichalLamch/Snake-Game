#include <iostream>
#include <list>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;
// Klasa Plansza
class Plansza {
private:
	// Wymiary planszy
	static const int size_x;
	static const int size_y;
	// Dynamicznie alokowana (dwuwymiarowa) tablica reprezentuj¹ca plaszê z wê¿em oraz jab³kami
	char** tab;
public:
	Plansza() {};
	//Dynamiczna alokacja tablicy
	char **wczytaj()
	{
		tab = new char*[size_y];
		for (int i = 0; i < size_y; i++) {
			tab[i] = new char[size_x];
		}
		return tab;
	}
	// Wype³nienie planszy "pustymi" miejscami
	void wypelnij_pustymi() {
		for (int i = 0; i < size_y; i++) {
			for (int j = 0; j < size_x; j++) {
				tab[i][j] = ' ';
			}
		}
	}
	// Funkcja wpisuj¹ca char'y do tablicy
	void Rysuj(int wsp_x, int wsp_y, char element){
		tab[wsp_y][wsp_x] = element;
	}
	// Drukowanie tablicy w konsoli
	void drukuj() {
		for (int i = 0; i < size_y; i++) {
			for (int j = 0; j < size_x; j++) {
				cout << tab[i][j];
			}
			cout << "%" <<endl;
		}
		for (int i = 0; i < size_x+1; i++) cout << "%";
	}
	// Destruktor kasuj¹cy tablicê
	~Plansza() {
		for (int i = 0; i < size_y; i++) {
			delete[] tab[i];
		}
		delete[] tab;
	};
	// Pobranie szerokoœci planszy
	int Get_size_x() {
		return size_x;
	}
	// Pobranie wysokoœci planszy
	int Get_size_y() {
		return size_y;
	}
	// Pobranie tablicy
	char** Get_plansza() {
		return tab;
	}
};
// Nadanie wartoœci wymiarom planszy
const int Plansza::size_x = 30;
const int Plansza::size_y = 20;
// Klasa Wektor 2D 
class Wektor2D {
private:
	int x;
	int y;
public:
	Wektor2D(const int& a, const int& b) :x(a), y(b) {};
	~Wektor2D() {};
	int Get_x() { return x; };
	int Get_y() { return y; };
	void Set_x(int xx) { x = xx; };
	void Set_y(int yy) { y = yy; };
};
// Klasa Jab³ko - bonus zwiêkszaj¹cy d³ugoœæ wê¿a
class Jablko {
private:
	// Po³o¿enie jab³ka oraz jego symbol
	int jablko_x;
	int jablko_y;
	char jablko = '@';
public:
	Jablko(int xx, int yy) : jablko_x(xx), jablko_y(yy) {};
	//Funkcja wywo³ywana po zdobyciu przez wê¿a jab³ka i zmieniaj¹ca wspó³rzêdne jab³ka
	void Zmien_wsp_jablka(Plansza& pl) {
		do { // Pêtla szukaj¹ca wolnego miejsca na planszy dla jab³ka
			jablko_x = rand() % pl.Get_size_x();
			jablko_y = rand() % pl.Get_size_y();
		} while (pl.Get_plansza()[jablko_y][jablko_x] == '1');
	}
	// Funkcja Rysuj wywo³uj¹ca mtodê rysuj klasy Plansza
	void Rysuj(Plansza& p) {
		p.Rysuj(jablko_x, jablko_y, jablko);
	}
	// Pobranie wartoœci wspó³rzêdnych obecnego jab³ka
	int Get_jablko_x() { return jablko_x; }
	int Get_jablko_y() { return jablko_y; }
};
// Klasa w¹¿
class Snake {
private:
	// Po³o¿enia g³owy wê¿a
	int glowa_x;
	int glowa_y;
	// Definicja kierunku ruchu, inicjalizowana w prawo
	string kierunek = "prawo";
	// Lista przechowuj¹ca po³o¿enie ca³ego wê¿a jako elementy typu Wektor2D, iterator do listy
	list<Wektor2D> polozenie_weza;
	list<Wektor2D>::iterator itr;
	// Symbol elementu wê¿a
	char waz = 'O';
	// Zmienna przyjmuj¹ca wartoœæ true po wywo³aniu zmiany kierunku a nastêpnie wraca do wartoœci false po wykonaniu ruchu w ¿¹danym kierunku
	bool zmiana = false;
	// Zmienna kontakt przyjmuj¹ca wartoœæ true w przypadku wykrycia kolizja wê¿a z samym sob¹
	bool kontakt = false;
public:
	Snake(Plansza& p) { // Zainicjalowanie pierwotnego po³o¿enia wê¿a
		glowa_x = p.Get_size_x()/2-1;
		glowa_y = p.Get_size_y()/2;
		polozenie_weza.push_back(Wektor2D(glowa_x,glowa_y));
		polozenie_weza.push_back(Wektor2D(glowa_x-1, glowa_y));
		polozenie_weza.push_back(Wektor2D(glowa_x-2, glowa_y));
	}
	~Snake() { // Destruktor usuwaj¹cy listê po³o¿enia wê¿a
		polozenie_weza.clear();
	}
	// Funkcja Rysuj - rysuje wê¿a wywo³uj¹c metodê rysuj klasy Plansza
	void Rysuj(Plansza& p) {
		for (itr = polozenie_weza.begin(); itr != polozenie_weza.end(); itr++) {
			p.Rysuj(((*itr).Get_x()), ((*itr).Get_y()), waz);
		}
	}
	// Funkcja zawieraj¹ca logikê ruchu wê¿a
	void Ruch(Jablko& jb, Plansza &p) {
		if (kierunek == "prawo") {
			glowa_x += 1;
		}
		if (kierunek == "gora") {
			glowa_y += -1;
		}
		if (kierunek == "lewo") {
			glowa_x += -1;
		}
		if (kierunek == "dol") {
			glowa_y += 1;
		}
		// W przypadku nietrafnienia na jab³ko - usuniêcie ostatniego cz³onu wê¿a
		if (jb.Get_jablko_x() != glowa_x || jb.Get_jablko_y() != glowa_y) {
			polozenie_weza.pop_back();
		}
		// W przypadku trafienia na jab³ko - zmiana wspó³rzêdnych jab³ka
		else {
			jb.Zmien_wsp_jablka(p);
		}
		// Dodanie do pocz¹tku listy nowego po³o¿enia g³owy wê¿a
		polozenie_weza.push_front(Wektor2D(glowa_x, glowa_y));
		// Sprawdzenie kolizji wê¿a ze œci¹n¹, jeœli nast¹pi - wywo³anie wyj¹tku
		if (glowa_x < 0 || glowa_y < 0 || glowa_x >= p.Get_size_x() || glowa_y >= p.Get_size_y()) {
			throw "Przegrales, uderzono w sciane :(";
		}
		// Sprawdzenie kolizji wê¿a z samym sob¹, jeœli nast¹pi - wywo³anie wyj¹tku
		for (itr = next(polozenie_weza.begin(),1); itr != polozenie_weza.end(); itr++) {
			if ((*itr).Get_x() == glowa_x && (*itr).Get_y() == glowa_y) {
				kontakt = true;
			}
		}
		if (kontakt) {
			throw "Przegrales, zjadles samego siebie :v";
		}
	}
	// Zbieranie danych z klawiatury, zmiana wartoœci kierunek po naciœniêciu odpowiedniej strza³ki na klawiaturze
	void Zmien_kierunek() {
		zmiana = false;
		// Jeœli naciœniêto strza³kê i obecny ruch nie jest przeciwny do wywo³ywanego ORAZ wywo³ane zosta³o PIERWSZE ¿adanie podczas ka¿dego cyklu - zmiana kierunku
		if (GetAsyncKeyState(VK_UP) && kierunek != "dol" && zmiana == false) {
			kierunek = "gora";
			zmiana = true;
		}
		if (GetAsyncKeyState(VK_DOWN) && kierunek != "gora" && zmiana == false) {
			kierunek = "dol";
			zmiana = true;
		}
		if (GetAsyncKeyState(VK_LEFT) && kierunek != "prawo" && zmiana == false) {
			kierunek = "lewo";
			zmiana = true;
		}
		if (GetAsyncKeyState(VK_RIGHT) && kierunek != "lewo" && zmiana == false) {
			kierunek = "prawo";
			zmiana = true;
		}
	}
	// Okreœlenie obecnego poziomu gry - liniowo w stosunku do d³ugoœci wê¿a
	int lvl() {
		return polozenie_weza.size();
	}
	// Sprawdzanie danych
	//void Drukuj_Dane() {
	//	for (itr = polozenie_weza.begin(); itr != polozenie_weza.end(); itr++) {
	//		cout << "x =" << (*itr).Get_x() << " y =" <<  (*itr).Get_y() << endl;
	//	}
	//}
};

int main() {
	srand(time(NULL));
	// Utworzenie obiektu plansza
	Plansza p1;
	// Dynamiczna alokacja pamiêci
	char **pole = p1.wczytaj();
	// Utworzenie obiektów w¹¿ oraz jab³ko
	Snake s1(p1);
	Jablko j1(5,5);
	// Rozpoczynamy grê, pêtla nieskoñczona bêdzie siê wykonywaæ do momentu przechwycenia jednego z wyj¹tków
	while (1) {
		// Wype³nienie planszy pustymi miejscami
		p1.wypelnij_pustymi();
		// Odczyt z klawiatury, ewentualna zmiana kierunku wê¿a
		s1.Zmien_kierunek();
		// Logika ruchu wê¿a, sprawdzenie poprawnoœci ruchu
		try {
			s1.Ruch(j1, p1);
		}
		// Wyj¹tki
		catch (char* s) {
			cerr << s << endl;
			cout << " Twoj wynik to: " << s1.lvl() - 3 << endl;
			getchar();
			exit(0);
		}
		// Narysowanie jab³ka na planszy
		j1.Rysuj(p1);
		// Narysowanie wê¿a na planszy
		s1.Rysuj(p1);
		// Wydrukowanie gotowej planszy
		p1.drukuj();
		// Wyœwietlenie obecnego poziomu
		cout << "Poziom: " << s1.lvl()-2 << endl;
		// Okreœlenie oczekiwania na kolejne posuniêcie wê¿a jako iloraz sta³ej i poziomu gry
		Sleep(500/s1.lvl());
		// Czyszczenie konsoli
		system("cls");
	};
	return(0);
}