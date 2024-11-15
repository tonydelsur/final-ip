#include<iostream>
#include <conio2.h>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;
// constantes generales
const int VENTANA = 20;
const int VELOCIDAD_INICIAL = 200; // velocidad incial, cambia con el avance
const int ANCHO = 40;
const int ALTO = 1200;
const int VELOCIDAD_PROYECTIL = 2000;
const int TIEMPO_MENSAJE = 5000;
const int TIEMPO_INMUNIDAD = 2000;
const int TIEMPO_PANTALLA_FINAL = 5000;
int desplazamiento = 0;
int VELOCIDAD = VELOCIDAD_INICIAL;
int puntaje;
string mensaje;
bool mensajeActivo;


// construir los algoritmos con pruebas externas e internas
// ir armando las clases en la medida que las funcionalidades demuestran
// integrarse bien.
void esperar(int milisegundos) {
	clock_t inicio = clock();
	while (clock() - inicio < milisegundos * CLOCKS_PER_SEC / 1000);
}

class Principal{	
protected:	
	int posX, posY; // atributos estandares para todos los objetos en movimiento
public:
	virtual void dibujar() = 0; //dibujar objeto
	virtual void actualizar() = 0; // actualizar posicion x e y del objeto
};

//-----------------------------------------------------------------terreno
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
		gotoxy(2, VENTANA + 4);
		cout << "Recuerda no enfrentarlos directamente";
	}
	void dibujar(){
		for(int i= 0; i<20;i++){
			gotoxy(1,i+1);
			textcolor(6);
			cout<<canon[desplazamiento + VENTANA -1 -i];
			textcolor(15);
			VELOCIDAD = VELOCIDAD_INICIAL - static_cast<int>(desplazamiento / 10);
		}
	}
	
		char fondo(int _x) {
			return canon[desplazamiento][_x]; 
		}
		
};

// ----------------------------------------------------------------- Nave	
class nave : public Principal{
private:
	int vidas;
	bool inmunidad;
	int aPosX, aPosY;
	clock_t tUltimaColision;
	
	
public:
	bool dispara;
	
	nave(){
		posX = ANCHO / 2;
		posY = 20;
		aPosX = posX;
		aPosY = posY;
		vidas = 5; 
		tUltimaColision = clock();
		dispara = false;
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
			if(tecla==' '){
				dispara = true;
			} // implementacion del disparo mas adelante
		} else {
			gotoxy(aPosX, aPosY);
			putchar(' ');
		}
	}
	
		bool getDistapara(){
			return dispara;
		}
		void setDispara(bool _dispara){
			dispara = _dispara;
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
					gotoxy(2, VENTANA + 4);
					cout << "Cuidado, puedes dañar la nave";
				}
				}

			if((clock() - tUltimaColision >= TIEMPO_INMUNIDAD * CLOCKS_PER_SEC / 1000)) {
				inmunidad = false;
			} else {
				inmunidad= true;
			}
		}
};

//--------------------------------------------------------------------Nave enemiga
class naveEnemiga : public Principal {
private:
	bool activa;           // Indica si la nave está activa
	int limiteIzq;         // Límite izquierdo del movimiento lateral
	int limiteDer;         // Límite derecho del movimiento lateral
	bool moviendoDerecha;  // Dirección del movimiento lateral
	int ultimoDesplazamiento; // Último desplazamiento sincronizado
	int aPosX, aPosY; // posicion anterior de la nave
	int contadorLateral; //para contolar el retraso de movimiento lateral
	int esperarLateral; // espera forzada para el movimiento lateral
	
public:
	naveEnemiga(){
		activa=false;
		moviendoDerecha=true;
		ultimoDesplazamiento= 0;
		contadorLateral = 0;
		esperarLateral = 50; //cantidad de ciclos de espera
		// deberia utilizar un temporizador, pero por ahora va.. a disparar.
	}
	
	void activar(int _desplazamiento) {
		posY = 1;  // Aparece en la primera línea visible
		posX = ANCHO / 2;  // Centro horizontal
		limiteIzq = posX - 5;  // Límite izquierdo
		limiteDer = posX + 5;  // Límite derecho
		activa = true;
		ultimoDesplazamiento = _desplazamiento;  // avanza con el cañon
		contadorLateral = 0; // inicializacion del contador lateral para velocidad lateral
	}
	
	void desactivar() {
		activa = false;
	}
	
	void actualizarDesplazamiento(int _desplazamiento) {
		if (_desplazamiento > ultimoDesplazamiento) {
			posY++;
			ultimoDesplazamiento = _desplazamiento;
			if (posY > VENTANA) {
				desactivar();  // Desactivar si sale de la pantalla 
				gotoxy(2, VENTANA + 4);
			}
		}
	}
	
	void actualizar() override {
		if (!activa) return;
		
		// gotoxy(posX, posY - 1);
		//putchar(' ');  // solo borrador para recordar, no borra realmente
					
		aPosX = posX;
		aPosY = posY;
		
		// Movimiento lateral medio raro
		if (moviendoDerecha) {
			posX++;
			if (posX >= limiteDer) moviendoDerecha = false;
		} else {
			posX--;
			if (posX <= limiteIzq) moviendoDerecha = true;
		}
		// la nave se mueve ridiculamente rápido
		// controlar porque no se mueve a la velocidad del cañon
	}
	
	void dibujar() override {
		if (!activa) return;
		
		gotoxy(posX, posY);
		textcolor(15);
		putchar('X');  // Mostrar la nave
		gotoxy(aPosX,aPosY);
		putchar(' ');
	}
	
	int getX(){ 
		return posX; 
	}
	int getY() { 
		return posY; 
	}
	bool estaActiva() { 
		return activa; 
	}

};

//--------------------------------------------------proyectil
// implementar proyectil con temporizador como con velocidad lateral
class proyectil : public Principal {
private:
	bool activo;           // Esta activo?
	clock_t tInicio;       // Vamos a temporizar este tambien, luego pruebo de ponerlo en el main
	int velocidadVertical; // Retraso entre movimientos
public:
	proyectil(){
		activo=false;
		velocidadVertical=100;
	}
	
	void activar(int x, int y) {
		posX = x;
		posY = y - 1; // Aparece justo arriba de la nave, quizas no se implemente desde aquí luego
		activo = true;
		tInicio = clock(); // Inicia el temporizador para el movimiento
	}
	
	void desactivar() {
		activo = false;
	}
	
	void actualizar() override {
		if (!activo) return;
		
		if (clock() - tInicio >= (velocidadVertical * CLOCKS_PER_SEC / 1000)) {
			// Borrar el proyectil en la posición anterior
			gotoxy(posX, posY);
			putchar(' ');
			
			posY--;
			
			if (posY < 0) {
				desactivar(); // Desactivar si sale de la pantalla
			}
			
			tInicio = clock(); // Reinicia el temporizador
		}
	}
	
	void dibujar() override {
		if (!activo) return;
		
		gotoxy(posX, posY);
		putchar('|'); // DIBUJA PROYECTIL
	}
	
	bool estaActivo() {
		return activo;
	}
	
	int getX() {
		return posX;
	}
	
	int getY() {
		return posY;
	}
};

// --------------------------------------------------- gestor de interface
// hay muchas cosas que cambiar, en las pruebas quitaba los marcos laterales
class pantalla {
public:
	void mostrarPantallaInicial() {
		clrscr();// borrar pantalla para empezar
		textcolor(LIGHTCYAN);
		gotoxy(10, 10);
		cout << "DEFENSA EN EL GRAN CANON DE TITAN";
		textcolor(WHITE);
		gotoxy(10, 12);
		cout << "Presiona cualquier tecla para comenzar...";
		getch(); // Espera a que el jugador presione una tecla
	}
	
	void mostrarPantallaGameOver(int puntaje) {
		clrscr();
		textcolor(RED);
		gotoxy(10, 10);
		cout << "GAME OVER";
		gotoxy(10, 12);
		cout << "Puntaje final: " << puntaje;
		textcolor(WHITE);
		esperar(5000); // Espera 5 segundos
		mostrarPantallaInicial();
	}
	
	void mostrarPantallaVictoria(int puntaje) {
		clrscr();
		textcolor(GREEN);
		gotoxy(10, 10);
		cout << "VICTORIA!";
		gotoxy(10, 12);
		cout << "Puntaje final: " << puntaje;
		textcolor(WHITE);
		esperar(7000); // Espera 7 segundos
		mostrarPantallaInicial();
	}
	
	void dibujarMarco() {
		textcolor(WHITE);
		// solo agrego línea inferior
		for (int i = 1; i <= ANCHO -1; i++) {
			gotoxy(i, VENTANA + 2);
			putchar('-'); // Línea inferior
		}
		textcolor(WHITE);
	}
	
	void actualizarInterface(int vidas, int puntaje) {
		gotoxy(2, VENTANA + 3);
		cout << "VIDAS: " << vidas << "   PUNTAJE: " << puntaje;
	}
	
};

//---------------------------------------------------main
int main (int argc, char *argv[]) {
	pantalla gestorPantalla;
	terreno zona;
	nave gladiador;
	naveEnemiga enemigo;
	clock_t tInicioScroll = clock();
	clock_t tInicioLateral = clock();
	clock_t tMensaje = clock();
	proyectil proyectiles[5]; // hasta 5 proyectiles consecutivos
	mensajeActivo = false;
	bool mensajeMostrando = false;
	
	while (true) {
		// Mostrar pantalla inicial
		gestorPantalla.mostrarPantallaInicial();
		
		// Reiniciar todo el juego para que funcione infinitamente
		gladiador = nave();  // Reiniciar la nave del jugador
		enemigo = naveEnemiga();  // Reiniciar la nave enemiga
		for (int i = 0; i < 5; i++) {
			proyectiles[i] = proyectil();  // Reiniciar los proyectiles
		}
		puntaje = 0;  // Reiniciar puntaje
		desplazamiento = 0;  // Reiniciar desplazamiento
		VELOCIDAD = VELOCIDAD_INICIAL;  // Reiniciar velocidad del juego
		
		zona.inicializarCanon();  // Regenerar el terreno
		clrscr();
		
		tInicioScroll = clock();
		tInicioLateral = clock();
		tMensaje = clock();
		
		// Bucle principal del juego
		while(true){
			gestorPantalla.dibujarMarco(); // Me parece que esto deberia ser fijo y actualizar datos por eventos
			gestorPantalla.actualizarInterface(gladiador.getVidas(), puntaje);
			if(clock()-tInicioScroll>=(VELOCIDAD * CLOCKS_PER_SEC / 1000)){
				desplazamiento++;
				puntaje = puntaje + 10;
				zona.dibujar();
				enemigo.actualizarDesplazamiento(desplazamiento);  // Sincroniza con el cañón
				tInicioScroll = clock();
				if (desplazamiento % 25 == 0 && !enemigo.estaActiva()) { // activar nave cada multiplo de 25 desplazamientos
					enemigo.activar(desplazamiento);
				}
			} 
			
									   
			if(clock()-tInicioLateral>=(VELOCIDAD * 2 * CLOCKS_PER_SEC / 1000)){
				enemigo.actualizar();
				tInicioLateral = clock();
			}
			
			if(desplazamiento >= 800){
				break; // en el futuro será la pantalla de ganador
			}
			
			//enemigo.actualizar();
			enemigo.dibujar();
			gladiador.mover();
			gladiador.dibujar();
			char obstaculo = zona.fondo(gladiador.getX()); // Buscar en la última linea
			gladiador.manejarColision(obstaculo);
			if (enemigo.estaActiva() && 
				gladiador.getX() == enemigo.getX() && 
				gladiador.getY() == enemigo.getY()) {
				gladiador.manejarColision('X');
			}
				
			if(gladiador.dispara){
				for (int i = 0; i < 5; i++) {
					if (!proyectiles[i].estaActivo()) {
						proyectiles[i].activar(gladiador.getX(), gladiador.getY());
						break; // Activa un solo proyectil por pulsación
					}
				}
				gladiador.dispara = false;
				
			}
			// Actualizar proyectiles y verificar colisiones con la nave enemiga
			for (int i = 0; i < 5; i++) {
				if (proyectiles[i].estaActivo()) {
					proyectiles[i].actualizar();
					proyectiles[i].dibujar();
					
					// Verificar colisión con la nave enemiga
					if (enemigo.estaActiva() &&
						proyectiles[i].getX() == enemigo.getX() &&
						proyectiles[i].getY() == enemigo.getY()) {
						enemigo.desactivar(); // Impacto en la nave enemiga
						proyectiles[i].desactivar(); // Desactivar proyectil
						puntaje += 100; // Aumentar puntaje por impacto
						gotoxy(2, VENTANA + 4);
						cout << "Excelente Punteria              ";
						
					}
				}
			}
				
			if(desplazamiento >= 800){
				gestorPantalla.mostrarPantallaVictoria(puntaje);
				break; // en el futuro será la pantalla de ganador
			}
			if (gladiador.getVidas() <= 0) {
				gestorPantalla.mostrarPantallaGameOver(puntaje);
				break;
			}
			
		}
}
	
	return 0;
}

