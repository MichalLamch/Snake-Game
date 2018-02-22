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
	// Dynamicznie alokowana (dwuwymiarowa) tablica reprezentuj�ca plasz� z w�em oraz jab�kami
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
	// Wype�nienie planszy "pustymi" miejscami
	void wypelnij_pustymi() {
		for (int i = 0; i < size_y; i++) {
			for (int j = 0; j < size_x; j++) {
				tab[i][j] = ' ';
			}
		}
	}
	// Funkcja wpisuj�ca char'y do tablicy
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
	// Destruktor kasuj�cy tablic�
	~Plansza() {
		for (int i = 0; i < size_y; i++) {
			delete[] tab[i];
		}
		delete[] tab;
	};
	// Pobranie szeroko�ci planszy
	int Get_size_x() {
		return size_x;
	}
	// Pobranie wysoko�ci planszy
	int Get_size_y() {
		return size_y;
	}
	// Pobranie tablicy
	char** Get_plansza() {
		return tab;
	}
};
// Nadanie warto�ci wymiarom planszy
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
// Klasa Jab�ko - bonus zwi�kszaj�cy d�ugo�� w�a
class Jablko {
private:
	// Po�o�enie jab�ka oraz jego symbol
	int jablko_x;
	int jablko_y;
	char jablko = '@';
public:
	Jablko(int xx, int yy) : jablko_x(xx), jablko_y(yy) {};
	//Funkcja wywo�ywana po zdobyciu przez w�a jab�ka i zmieniaj�ca wsp�rz�dne jab�ka
	void Zmien_wsp_jablka(Plansza& pl) {
		do { // P�tla szukaj�ca wolnego miejsca na planszy dla jab�ka
			jablko_x = rand() % pl.Get_size_x();
			jablko_y = rand() % pl.Get_size_y();
		} while (pl.Get_plansza()[jablko_y][jablko_x] == '1');
	}
	// Funkcja Rysuj wywo�uj�ca mtod� rysuj klasy Plansza
	void Rysuj(Plansza& p) {
		p.Rysuj(jablko_x, jablko_y, jablko);
	}
	// Pobranie warto�ci wsp�rz�dnych obecnego jab�ka
	int Get_jablko_x() { return jablko_x; }
	int Get_jablko_y() { return jablko_y; }
};
// Klasa w��
class Snake {
private:
	// Po�o�enia g�owy w�a
	int glowa_x;
	int glowa_y;
	// Definicja kierunku ruchu, inicjalizowana w prawo
	string kierunek = "prawo";
	// Lista przechowuj�ca po�o�enie ca�ego w�a jako elementy typu Wektor2D, iterator do listy
	list<Wektor2D> polozenie_weza;
	list<Wektor2D>::iterator itr;
	// Symbol elementu w�a
	char waz = 'O';
	// Zmienna przyjmuj�ca warto�� true po wywo�aniu zmiany kierunku a nast�pnie wraca do warto�ci false po wykonaniu ruchu w ��danym kierunku
	bool zmiana = false;
	// Zmienna kontakt przyjmuj�ca warto�� true w przypadku wykrycia kolizja w�a z samym sob�
	bool kontakt = false;
public:
	Snake(Plansza& p) { // Zainicjalowanie pierwotnego po�o�enia w�a
		glowa_x = p.Get_size_x()/2-1;
		glowa_y = p.Get_size_y()/2;
		polozenie_weza.push_back(Wektor2D(glowa_x,glowa_y));
		polozenie_weza.push_back(Wektor2D(glowa_x-1, glowa_y));
		polozenie_weza.push_back(Wektor2D(glowa_x-2, glowa_y));
	}
	~Snake() { // Destruktor usuwaj�cy list� po�o�enia w�a
		polozenie_weza.clear();
	}
	// Funkcja Rysuj - rysuje w�a wywo�uj�c metod� rysuj klasy Plansza
	void Rysuj(Plansza& p) {
		for (itr = polozenie_weza.begin(); itr != polozenie_weza.end(); itr++) {
			p.Rysuj(((*itr).Get_x()), ((*itr).Get_y()), waz);
		}
	}
	// Funkcja zawieraj�ca logik� ruchu w�a
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
		// W przypadku nietrafnienia na jab�ko - usuni�cie ostatniego cz�onu w�a
		if (jb.Get_jablko_x() != glowa_x || jb.Get_jablko_y() != glowa_y) {
			polozenie_weza.pop_back();
		}
		// W przypadku trafienia na jab�ko - zmiana wsp�rz�dnych jab�ka
		else {
			jb.Zmien_wsp_jablka(p);
		}
		// Dodanie do pocz�tku listy nowego po�o�enia g�owy w�a
		polozenie_weza.push_front(Wektor2D(glowa_x, glowa_y));
		// Sprawdzenie kolizji w�a ze �ci�n�, je�li nast�pi - wywo�anie wyj�tku
		if (glowa_x < 0 || glowa_y < 0 || glowa_x >= p.Get_size_x() || glowa_y >= p.Get_size_y()) {
			throw "Przegrales, uderzono w sciane :(";
		}
		// Sprawdzenie kolizji w�a z samym sob�, je�li nast�pi - wywo�anie wyj�tku
		for (itr = next(polozenie_weza.begin(),1); itr != polozenie_weza.end(); itr++) {
			if ((*itr).Get_x() == glowa_x && (*itr).Get_y() == glowa_y) {
				kontakt = true;
			}
		}
		if (kontakt) {
			throw "Przegrales, zjadles samego siebie :v";
		}
	}
	// Zbieranie danych z klawiatury, zmiana warto�ci kierunek po naci�ni�ciu odpowiedniej strza�ki na klawiaturze
	void Zmien_kierunek() {
		zmiana = false;
		// Je�li naci�ni�to strza�k� i obecny ruch nie jest przeciwny do wywo�ywanego ORAZ wywo�ane zosta�o PIERWSZE �adanie podczas ka�dego cyklu - zmiana kierunku
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
	// Okre�lenie obecnego poziomu gry - liniowo w stosunku do d�ugo�ci w�a
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
	// Dynamiczna alokacja pami�ci
	char **pole = p1.wczytaj();
	// Utworzenie obiekt�w w�� oraz jab�ko
	Snake s1(p1);
	Jablko j1(5,5);
	// Rozpoczynamy gr�, p�tla niesko�czona b�dzie si� wykonywa� do momentu przechwycenia jednego z wyj�tk�w
	while (1) {
		// Wype�nienie planszy pustymi miejscami
		p1.wypelnij_pustymi();
		// Odczyt z klawiatury, ewentualna zmiana kierunku w�a
		s1.Zmien_kierunek();
		// Logika ruchu w�a, sprawdzenie poprawno�ci ruchu
		try {
			s1.Ruch(j1, p1);
		}
		// Wyj�tki
		catch (char* s) {
			cerr << s << endl;
			cout << " Twoj wynik to: " << s1.lvl() - 3 << endl;
			getchar();
			exit(0);
		}
		// Narysowanie jab�ka na planszy
		j1.Rysuj(p1);
		// Narysowanie w�a na planszy
		s1.Rysuj(p1);
		// Wydrukowanie gotowej planszy
		p1.drukuj();
		// Wy�wietlenie obecnego poziomu
		cout << "Poziom: " << s1.lvl()-2 << endl;
		// Okre�lenie oczekiwania na kolejne posuni�cie w�a jako iloraz sta�ej i poziomu gry
		Sleep(500/s1.lvl());
		// Czyszczenie konsoli
		system("cls");
	};
	return(0);
}