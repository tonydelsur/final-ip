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
	
	int getX(){
		return x;
	}
		
	int getY(){
		return y;
	}
	
	virtual void mover() = 0;
	
	
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
	int getX(){
		return x;
	}
		
		
};		

class Meteoro : public Objeto {
public:
	Meteoro(int x, int y) : Objeto(x, y, 'O') {}
	
	// metodos
	//sobreescribier el virtual
	void mover() override {
		borrar();
		if(y<25) {
			y++;
		} else {
			y=1;
		}
		
	}
};

class NaveEnemiga : public Objeto {
private:
	bool direccionDerecha; //indica sentido de movimiento
	
public:
	NaveEnemiga(int x, int y) : Objeto(x,y,'#') {}
	
	//metodo mover sobreescribiendo
	void mover() override {
		borrar();
		if(direccionDerecha){
			if(x<80){
				x++;
			} else {
				direccionDerecha = false; // cambia de sentido
			}
		} else {
			if(x>1) {
				x--;
			} else {
				direccionDerecha = true;
			}
		}
		dibujar();
	}
};

class Disparo : public Objeto {
public: 
	Disparo(int x, int y) : Objeto(x,y,'|'){}
	
	void mover() override {
		borrar();
		if(y>1) {
			y--;
		} 
		
		dibujar();
	}
};


int main (int argc, char *argv[]) {
	clrscr();
	Objeto* objetos[3];
	objetos[0] = new NaveJugador(40,24);
	objetos[1] = new NaveEnemiga(10,5);
	objetos[2] = new Meteoro(20,1);
	
	//bucle
	while (true) {
		for (int i=0;i<3;i++){
			objetos[i]->mover();
		
		if(kbhit()) {
			char tecla = getch();
			if(tecla == 32){
				if(objetos[3] == nullptr) {
					objetos[3] = new Disparo(objetos[0]->getX(),24);
				} else {
					
					if(objetos[3]->getY() == 1) {
						objetos[3]->mover();
					} else {
						delete objetos[3];
					}
					
				}
			}
			
		}
		}
	}
	
	return 0;
}

