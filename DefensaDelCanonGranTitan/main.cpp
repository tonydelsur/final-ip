#include<iostream>
#include <conio2.h>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;
// constantes generales
const int VENTANA = 20;
const int VELOCIDAD = 250;
const int ANCHO = 40;
const int ALTO = 1200;
const int VELOCIDAD_PROYECTIL = 2000;
const int TIEMPO_INMUNIDAD = 2000;
const int TIEMPO_PANTALLA_FINAL = 5000;
int desplazamiento = 0;


// construir los algoritmos con pruebas externas e internas
// ir armando las clases en la medida que las funcionalidades demuestran
// integrarse bien.

class Principal{	
protected:	
	int posX, posY; // atributos estandares para todos los objetos en movimiento
public:
	virtual void dibujar() = 0; //dibujar objeto
	virtual void actualizar() = 0; // actualizar posicion x e y del objeto
};

class terreno{
protected:
	string canon[ALTO];
	char canonChar[ALTO][ANCHO];
public:
	terreno(){};
	~terreno(){};
	
	void inicializarCanon(){ //
		// Al intentar renderizar el cañon con char se hacia muy lento
		// La idea es generarlo con char y pasarlo a string
		// 1- crear el cañon con char.
		//2- pasarlo a string por líneas
		srand(time(0));
		for (int i = 0; i < ALTO; i++) {
			// determinacion del ancho del pasillo por inicio y final
			int inicio=rand()%10 +5; // ancho de la ladera izquiera
			int fin=rand()%10+25; // ancho de la ladera derecha
			int densidad = rand()%6+1; // permite variar el largo de la geografia
			for(int densidad = 0;densidad<4;densidad++){
				for (int j = 0; j < ANCHO; j++) {
					if((j<inicio-1)|| (j>fin+1)){ //laderas del cañon
						canonChar[i][j] = 176;
					}
					if ((j==inicio-1)||(j==fin+1)){ // paredes del cañon
						canonChar[i][j] = 177;
					}
					if(j>inicio && j<fin){ // interior del cañon
						canonChar[i][j] = ' ';
					}
				}
				i++;
			}
			i--;
		}
		// por cuestion de orden, la conversión la saco del bucle anterior
		for (int i = 0; i < ALTO; i++) {
			string linea = "";
			for (int j = 0; j < ANCHO; j++) {
				linea += canonChar[i][j]; // Concatenar caracteres en un string
			}
			canon[i] = linea; // Asignar el string al arreglo unidimensional
		}
		
	}
	void dibujar(){
		for(int i= 0; i<20;i++){
			gotoxy(1,i+1);
			textcolor(6);
			cout<<canon[desplazamiento + VENTANA -1 -i];
			textcolor(15);
		}
	}
	
		char fondo(int _x) {
			return canon[desplazamiento][_x]; 
		}
		
		int puntoMedio(int registro) {
			string linea = canon[registro];
			int inicioEspacios = -1;
			int finEspacios = -1;
			
			// Buscar inicio y fin de la secuencia de espacios
			for (int i = 0; i < ANCHO; i++) {
				if (linea[i] == ' ' && inicioEspacios == -1) {
					inicioEspacios = i; // Primer espacio
				}
				if (linea[i] == ' ' && inicioEspacios != -1) {
					finEspacios = i; // Último espacio 
				}
			}
			
			// Si no hay espacios en la línea
			if (inicioEspacios == -1 || finEspacios == -1) {
				return -1; // No hay secuencia de espacios
			}
			
			// Calcular la posición central
			return (inicioEspacios + finEspacios) / 2;
		}
};

class nave : public Principal{
private:
	int vidas;
	bool inmunidad;
	int aPosX, aPosY;
	clock_t tUltimaColision;
	
public:
	nave(){
		posX = ANCHO / 2;
		posY = 20;
		aPosX = posX;
		aPosY = posY;
		vidas = 5; 
		tUltimaColision = clock();
	}
		
	void mover(){ // mover con las teclas
		
		if(kbhit()){
			char tecla = getch();
			if(tecla == 75 && posX > 0){ // Izquierda
				aPosX = posX;
				posX--;
			}
			if(tecla == 77 && posX < ANCHO -1){ // Derecha
				aPosX = posX;
				posX++;
			}
			// if(tecla==' '){ } // implementacion del disparo mas adelante
		}
	}
	void dibujar() override{ //dibujar en pantalla, problema es tiempo de actualizacion
		gotoxy(aPosX,posY);
		putchar(' ');
		gotoxy(posX,posY);
		if(inmunidad){
			textcolor(LIGHTRED);
		} else {
			textcolor(WHITE);
		}
		putchar('^');
		gotoxy(41,21);
	}
	void actualizar() override{
		
	}
	int getVidas(){
		return vidas;
	}
	
	int getX(){
		return posX;
	}
		int getY(){
			return posY;
		}
	bool getInmunidad(){
		return inmunidad;
	}
		
		
		void manejarColision(char _caracter) { 
			// Si el carácter no es espacio, cuenta como colisión
			if (_caracter != ' ') {
				// Verificar si ha pasado el tiempo de inmunidad
				if (clock() - tUltimaColision >= TIEMPO_INMUNIDAD * CLOCKS_PER_SEC / 1000) {
					vidas--;
					tUltimaColision = clock();
					if (vidas < 0) {
						cout << "Perdiste" << endl; // Pantalla de Game Over
						// Puedes implementar una salida o reinicio
					}
				}
				}

			if((clock() - tUltimaColision >= TIEMPO_INMUNIDAD * CLOCKS_PER_SEC / 1000)) {
				inmunidad = false;
			} else {
				inmunidad= true;
			}
		}
};

int main (int argc, char *argv[]) {
	terreno zona;
	nave gladiador;
	clock_t tInicioScroll = clock();
	zona.inicializarCanon();
	while(true){
		
		if(clock()-tInicioScroll>=(VELOCIDAD * CLOCKS_PER_SEC / 1000)){
			desplazamiento++;
			zona.dibujar();
			tInicioScroll = clock();
		} 
		if(desplazamiento >= 800){
			break; // en el futuro será la pantalla de ganador
		}
		gladiador.mover();
		gladiador.dibujar();
		char obstaculo = zona.fondo(gladiador.getX()); // Buscar en la última linea
		gladiador.manejarColision(obstaculo);
		gotoxy(5,22);
		cout<<gladiador.getVidas()<<"   "<<obstaculo;
	}
	
	return 0;
}

