#ifndef __INTERFAZ_BATALLA_H__
#define __INTERFAZ_BATALLA_H__

#include "client_ContactaServer.h"
#include "client_HiloMovimientos.h"
#include "Mapa.h"
#include "common_Thread.h"
#include "client_ChatDoble.h"
#include <vector>
#include <string>
#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>

class InterfazBatalla: public Thread
{
	private:
		
		ContactaServer* contacto; //contacto con el servidor
		Mapa* mapa; //referencia al mapa
		ChatDoble* chat; //chat que se usa de puente con la interfaz gráfica
		HiloMovimientos* movimientos; //hilo que simula el retardo de los movimientos
		
		int puntos;
		
		bool jugando;

		//desconvierte el hexadecimal
		static char obtenerValor (char hexa);

	

	public:
		//decodifica el mensaje en hexadecimal
		static void desconvertir(std::string& resultado, const std::string& original);
	
		/* Constructor */
		InterfazBatalla(ContactaServer* contacto);
	
		/* Prepara el proceso para la batalla. Envía los archivos */
		void iniciarBatalla(ChatDoble* chat);
		
		/* Analiza el tipo de mensaje recibido y actúa en consecuencia */
		bool analizarMensajesRecibidos();

		/* Devuelve true si la batalla está en curso */
		bool estaJugando();
		
		/* Devuelve los puntos que realizó */
		int getPuntos();
		
		/* Modifica el valor de jugando */
		void setJugando(bool valor);

		/* Se efectúa la batalla */
		void run();

};

#endif

