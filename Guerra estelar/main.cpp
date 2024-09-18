#include<iostream>
#include <conio2.h>
using namespace std;


class Objeto {
protected:
	int x,y; //coordenadas
	char chasis; //forma o caracter que represante al objeto
	
public:
	Objeto(int x, int y, char rep) : x(x), y(y), chasis(rep) {}
	
	//métodos en común
	void dibujar(){
		gotoxy(x, y);
		cprintf("%c",chasis);
	}
		
	void borrar() {
		gotoxy(x, y);
		cprintf(" ");
	}
	
};

class NaveJugador : public Objeto {
public:
		NaveJugador(int x, int y) : Objeto(x, y, '^') {}  
		
	// Metodos de nuestra nave de jugador
	void mover() {
			if (kbhit()) {
				char tecla = getch(); //reconocer la tecla o capturar
				borrar(); //borramos nuestra nave de jugador
				 if (tecla == 75 && x>1) { // flecha izquierda y no toca el borde
					 x--;
				 } else if (tecla == 77 && x<80) { // flecha derecha y no toca el borde
					 x++;
				 } else if (tecla == 72 && y>1) { // flecha arriba y no toca el borde superior
					 y--;
				 } else if (tecla == 80 && y< 25) { // flecha abajo y no toca el borde inferior
					 y++;
				 }
				 dibujar(); //dibuja la nave en la nueva posicion
			}
		}
		
		
};		

int main (int argc, char *argv[]) {
	clrscr();
	NaveJugador nave(40,12);
	nave.dibujar();
	
	//bucle
	while (true) {
		nave.mover();
		gotoxy(1,1);
	}
	
	return 0;
}

