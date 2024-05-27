//Środowisko Dev-Cpp 5.11 TDM-GCC 4.9.2 Setup, kompilator TDM-GCC 4.9.2 64-bit Release
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

const int l = 10; 		// liczba linii planszy
const int k = 10; 		// liczba kolumn planszy
const int m = 15; 		// liczba min na planszy

/*
Funkcja rysuje plansze
ze wszystkimi ikonkami - sladem po graczu,
ostrzezenami przed minami itp.
*/
void rysuj_plansze(bool koniec_gry, char *wsk_plansza) 
{
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < k; j++) {
			switch (*(wsk_plansza + i*k + j)) {
			case '#':
				cout << "|#  ";
				break;
			case '.':
				cout << "|.  ";
				break;
			case '@':
				cout << "|!# ";
				break;
			case '*':
				if (koniec_gry == false)
					cout << "|   ";
				else
					cout << "|*  ";		// kiedy gra jest zakonczona, plansza zostanie wyrysowana z zaznaczonymi wszystkimi minami
				break;
			case '&':
				cout << "|&  "; 		// kiedy gracz wejdzie na mine, plansza zostanie wyrysowana z zaznaczonymi wszystkimi minami
				break;
			case '!':
				cout << "|!  ";
				break;

			default:
				cout << "|   ";
				break;
			}
		}
		cout << "|";
		cout << endl;
		
		for (int h = 0; h < k; h++) {	// rysowanie poziomych linii między polami dla przejrzystosci planszy 
			cout << "----";
		}
		cout << endl;

	}
}

/*
Funkcja sprawdza, czy mozna umiescic miny na pozycji X Y. Jesli nie jest to mozliwe, zwraca wartosc false.
Jesli jest mozliwe, zwraca true i rozmieszcza mine.
*/
bool rozmiesc_miny(int X, int Y, char *wsk_plansza)
{
	if (*(wsk_plansza + X*k + Y) == '*') {	// jesli na polu jest juz mina, program umiesci mine na innym polu						
		return false;
	}
	if (*(wsk_plansza + X*k + Y) == 'x') {	// jesli pole jest czescia dobrej sciezki, program umiesci mine na innym polu
		return false;
	}

	*(wsk_plansza + X*k + Y) = '*';
	return true;
}

/*
Funkcja sprawdza czy zostal spelniony
warunek wygranej - czy gracz wszedł na pole
w prawym gornym rogu planszy.
*/
bool wygrana(int X, int Y)	
{
	if (X == 0 && Y == k - 1) {
		return true;
	} else
		return false;
}

/*
Funkcja probuje umiescic m min na planszy.
*/
bool losuj_pozycje(char *wsk_plansza)
{
	time_t t;
	int X;
	int Y;
	int ilosc_do_wylosowania = m;
	int ilosc_prob = 0;		//zmienna 'ilosc prob' ma liczyc ile razy program probuje umiescic jedna mine na planszy
	srand((unsigned)time(&t));

	while (ilosc_do_wylosowania > 0) {
		X = rand() % l;
		Y = rand() % k;
		ilosc_prob++;
		if (rozmiesc_miny(X, Y, wsk_plansza)) {
			ilosc_do_wylosowania--;
			ilosc_prob = 0;
		}
		if (ilosc_prob > 100) {
			cout << "Nie udalo sie rozmiescic wszystkich min. Rozmieszczono " << m - ilosc_do_wylosowania << "/" << m << " ."<< endl;
			return false;		//po 100 probach umieszczenia 1 miny na planszy funkcja zwraca wartosc false,
		}						//co zabezpiecza przed zbyt duza iloscia min na planszy
	}
	return true;

}

/* 
Funkcja sprawdza, czy gracz stanal na polu przyleglym do miny. Jesli tak, zwraca wartosc true.
*/
bool wykryto_mine(int X, int Y, char *wsk_plansza)		
{
	if (*(wsk_plansza + (X + 1)*k + Y) == '*' && X != l - 1) {
		return true;
	}
	if (*(wsk_plansza + X*k + (Y + 1)) == '*' && Y != k - 1) {
		return true;
	}
	if (*(wsk_plansza + (X - 1)*k + Y) == '*' && X != 0) {
		return true;
	}
	if (*(wsk_plansza + X*k + (Y - 1)) == '*' && Y != 0) {
		return true;
	}
	return false;
}

/* 
Funkcja zwraca false, jesli gracz wpisal kilka komend naraz. Zabezpiecza to przed wykonywaniem kilku ruchow naraz.
*/
bool jeden_znak()	
{
	int licz_znaki = 0;
	while ('\n' != getchar()) {
		licz_znaki++;
	};
	if (licz_znaki > 0) {
		return false;
	} else
		return true;
}


/* 
Funkcja zawiera sterowanie i czytanie klawiszy.
*/
void gra(char *wsk_plansza)	
{
	bool koniec_gry = false;	//
	bool zly_klawisz;			//zmienna sprawdza, czy wprowadzona zostala bledna komenda z klawiatury
	int X = l - 1;				//poczatkowa pozycja sapera
	int Y = 0;
	if (wykryto_mine(X, Y, wsk_plansza)) {
		cout << "jestes w poblizu miny" << endl;
		*(wsk_plansza + X*k + Y) = '@';
	} else
		*(wsk_plansza + X*k + Y) = '#';
	rysuj_plansze(koniec_gry, wsk_plansza);
	char ruch;

	while (ruch != 'q') {
		cout << "Wykonaj ruch (w-gora, s-dol, a-lewo, d-prawo, q-wyjscie z gry): ";
		cin >> ruch;
		if (jeden_znak() == false) {
			cout << endl << "Wciskaj tylko 1 klawisz naraz!" << endl << endl;

		} else {

			if (*(wsk_plansza + X*k + Y) == '@') {
				*(wsk_plansza + X*k + Y) = '!';
			} else
				*(wsk_plansza + X*k + Y) = '.';
			zly_klawisz = false;
			switch (ruch) {
			case 'a':		//ruch w lewo
				if (Y != 0) {
					Y--;
				} else {
					zly_klawisz = true;
					cout << endl << "Probujesz wyjsc poza krawedz planszy!" << endl << endl;
				}
				break;
			case 'd':		//ruch w prawo
				if (Y != k - 1) {
					Y++;
				} else {
					zly_klawisz = true;
					cout << endl << "Probujesz wyjsc poza krawedz planszy!" << endl << endl;
				}
				break;
			case 'w':		//ruch w gore
				if (X != 0) {
					X--;
				} else {
					zly_klawisz = true;
					cout << endl << "Probujesz wyjsc poza krawedz planszy!" << endl << endl;
				}
				break;
			case 's':		//ruch w dol
				if (X != l - 1) {
					X++;
				} else {
					zly_klawisz = true;
					cout << endl << "Probujesz wyjsc poza krawedz planszy!" << endl << endl;
				}
				break;

			case 'q':
				cout << endl << "Wybrano wyjscie z gry" << endl << endl;
				return;

			default:													//jezeli gracz wcisnal pojedynczy bledny klawisz i nie probowal wyjść poza plansze,
				zly_klawisz = true;										//program sprawdza kolejno: czy gracz wszedl na mine, czy gracz jest w poblizu miny,
				cout << endl << "Bledny ruch." << endl << endl;	  		//czy gracz doszedł na zwycięskie miejsce, a następnie wyrysowana zostaje plansza
				break;
			}
			if (zly_klawisz == false) {

				if (*(wsk_plansza + X*k + Y) == '*') {
					*(wsk_plansza + X*k + Y) = '&';
					cout << "nadepnieto na mine" << endl;
					koniec_gry = true;
					rysuj_plansze(koniec_gry, wsk_plansza);
					return;
				} else if (wykryto_mine(X, Y, wsk_plansza)) {
					cout << "jestes w poblizu miny" << endl;
					*(wsk_plansza + X*k + Y) = '@';
				} else
					*(wsk_plansza + X*k + Y) = '#';

				if (wygrana(X, Y)) {
					koniec_gry = true;
					cout << "Gratulacje! Udalo Ci sie bezpiecznie przejsc na koncowe pole!" << endl;
					rysuj_plansze(koniec_gry, wsk_plansza);
					return;
				}

				rysuj_plansze(koniec_gry, wsk_plansza);
			}
		};

	}
}

/* 
Funkcja czysci pamiec w ktorej przechowywana jest plansza.
*/
void zerowanie_planszy(char *wsk_plansza)	
{
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < k; j++) {
			*(wsk_plansza + i*k + j) = 0;
		}
	}
}

/*
Funkcja zapewnia bezpieczna sciezke dla gracza, aby mial on mozliwosc wygrania gry.
*/
void losuj_dobra_sciezke(char *wsk_plansza)	
{
	int X = l - 1;
	int Y = 0;
	int kierunek;
	time_t t;
	
	srand((unsigned)time(&t));
	(*(wsk_plansza + X*k + Y)) = 'x';
	while (true) {
		kierunek = rand() % 2;
		if (kierunek == 0 && X != 0) {		//sciezka jest tworzona poprzez losowe wybieranie kierunku w gore lub w prawo 
			X--;							//az osiagniete zostanie zwycieskie pole
		} else if (kierunek == 1 && Y != k - 1) {
			Y++;
		}
		(*(wsk_plansza + X*k + Y)) = 'x';
		if (wygrana(X, Y) == true) {
			break;
		}
	}
	
}

int main()
{
	char plansza[l][k];	//zmienna generuje i przechowuje plansze do gry
	
	zerowanie_planszy(&plansza[0][0]);
	losuj_dobra_sciezke(&plansza[0][0]);
	if (losuj_pozycje(&plansza[0][0]) == true) {
		cout << "Rozmieszczono pomyslnie wszystkie miny." << endl;
	};

	cout << "Twoim celem jest przejscie z lewego dolnego pola na prawe gorne pole omijajac miny.\nPowodzenia." << endl << endl;
	gra(&plansza[0][0]);

	system("pause");
	return 0;
}
