#include<iostream>
#include <conio2.h>
#include <ctime>

using namespace std;

int main (int argc, char *argv[]) {
	clock_t horaInicial = clock();
	const double espera = 0.05;
	clock_t horaActual;
	double diferencia;
	
	for (int columna=1; columna<80;columna++){
		clrscr();
		gotoxy(columna,10);
		cprintf("O");
		gotoxy(1,1);
		do {
			horaActual = clock();
			diferencia = double(horaActual-horaInicial) / CLOCKS_PER_SEC;
		} while(diferencia < espera);
		horaInicial = horaActual;
		
	}
	
	
	return 0;
}

