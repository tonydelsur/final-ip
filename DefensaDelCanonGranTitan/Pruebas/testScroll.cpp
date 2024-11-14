// convertir un arreglo de char aleatorio a un arreglo string 
// para facilitar el renderizado de la pantalla
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int ANCHO = 40;
int ALTO = 1600;

int main() {
	// dibujar cañon en char con algoritmo de pasillo
	char arregloChar[ALTO][ANCHO];
	srand(time(0));
	for (int i = 0; i < ALTO; i++) {
		// determinacion del ancho del pasillo por inicio y final
		int inicio=rand()%10 +5;
		int fin=rand()%10+25;
		int densidad = rand()%6+1;
		for(int densidad = 0;densidad<4;densidad++){
			for (int j = 0; j < ANCHO; j++) {
					if((j<inicio-1)|| (j>fin+1)){ //laderas del cañon
						arregloChar[i][j] = 176;
					}
					if ((j==inicio-1)||(j==fin+1)){ // paredes del cañon
						arregloChar[i][j] = 177;
					}
					if(j>inicio && j<fin){ // interior del cañon
						arregloChar[i][j] = ' ';
					}
			}
			i++;
		}
		i--;
	}
	
	// arreglo destino
	string arregloString[ALTO];
	
	// Convertir el arreglo de char a string
	for (int i = 0; i < ALTO; i++) {
		string linea = "";
		for (int j = 0; j < ANCHO; j++) {
			linea += arregloChar[i][j]; // Concatenar caracteres en un string
		}
		arregloString[i] = linea; // Asignar el string al arreglo unidimensional
	}
	
	// Imprimir el resultado para verificar
	for (int i = 0; i < 1000; i++) { // Mostrar 100 lineas
		cout << arregloString[i] << endl;
	}
	
	return 0;
}
