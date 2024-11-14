#include<iostream>
#include <conio2.h>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;
// constantes generales
const int VENTANA = 20;
const int VELOCIDAD = 10;
const int VELOCIDAD_PROYECTIL = 2000;
const int TIEMPO_INMUNIDAD = 2000;
const int TIEMPO_PANTALLA_FINAL = 5000;
int desplazamiento = 0;


// construir los algoritmos con pruebas externas e internas
// ir armando las clases en la medida que las funcionalidades demuestran
// integrarse bien.

class Principal{	
protected:	
	int x, y; // atributos estandares para todos los objetos en movimiento
public:
	virtual void dibujar() = 0; //dibujar objeto
	virtual void actualizar() = 0; // actualizar posicion x e y del objeto
};

class terreno{
protected:
	static const int ANCHO = 40; // ancho de la zona de juego. Static por errores
	static const int ALTO = 1200; // largo de la zona de juego
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
			cout<<canon[desplazamiento + VENTANA -1 -i]<<desplazamiento+i;
			textcolor(15);
		}
	}
	
	
	
};
int main (int argc, char *argv[]) {
	terreno zona;
	clock_t tInicioScroll = clock();
	zona.inicializarCanon();
	while(true){
		if(clock()-tInicioScroll>=(VELOCIDAD * CLOCKS_PER_SEC / 1000)){
			desplazamiento++;
			zona.dibujar();
			tInicioScroll = clock();
		}
		if(desplazamiento >= 800){
			break;
		}
		
	}
	
	return 0;
}

