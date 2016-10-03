#ifndef VENTANA_H_
#define VENTANA_H_

#include <SDL.h>
#include <SDL_image.h>
#include "PanelContenedor.h"
#include "Coordenada.h"
#include <list>
#include <SDL_opengl.h>
#include "constantes.h"
#include "client_ChatDoble.h"
#include "Reloj.h"
#include "constantes_sms.h"
#include <string>
#include "Mapa.h"
#include "hiloCron.h"

class PanelContenedor;

class Ventana
{
private:
	/* Pantalla grafica */
	SDL_Surface * pantalla;
	/* Dimensiones pantalla */
	int ancho;
	int alto;
	/* Lista de paneles contenidos */
	std::list <PanelContenedor *> _paneles;
	/* Corrige las coordenas */
	void corregirCoordenadas(ulong *x, ulong *y);
	/* Reloj que cuenta el tiempo */
	Reloj reloj;
	/* Hilo del contador de tiempo */
	HiloCron * cronometro;
	/* Muestra el dinero */
	Reloj displayDinero;
	/* Puente de comunicación */
	ChatDoble* chat;
	/* Calcula puntaje mediante una formula */
	long calcularPuntaje(long a, long b, long c, long d, long tS, long tR, int sumaPrecios);
public:
	/* Constructor y destructor */
	Ventana(int ancho, int alto, ChatDoble* chat);
	virtual ~Ventana();
	/* Recibe eventos y dibuja */
	void ejecutar (void* contrincante, void* mapa);
	/* Dibuja todos los paneles, Reloje y Plata */
	void imprimir_pantalla(std::string & tiempo, std::string & money);
	/* Agrega paneles */
	void agregarPanel(PanelContenedor *, const Coordenada);
	void agregarPanel(PanelContenedor * panel, const int lugar);
	/* Ejecuta eventos de paneles o propios */
	bool seEjecutaEventoEn(const Coordenada, int evento);
	/* Levanta a los botones porque otro se clickeo */
	void desclickearBotones(int id_excepcion);
	
	/* Eventos del mouse */
	void evento_MouseDown(Coordenada punto);
	void evento_MouseOver(Coordenada punto);
	void evento_MouseUp(Coordenada punto);
	/* variables utilitarias */
	bool salir;
	bool sms;
	bool pideSimulacion;
	long getTiempoResolucion();
	
};

#endif /*VENTANA_H_*/
