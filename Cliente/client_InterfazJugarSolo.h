#ifndef __INTERFAZ_JUGAR_SOLO_H__
#define __INTERFAZ_JUGAR_SOLO_H__

#include "common_Thread.h"
#include "Mapa.h"

class InterfazJugarSolo : public Thread
{
	private:
		bool jugando; //True si está jugando
		int puntos;
		Mapa* mapa; //Referencia al mapa
		
		/* Modifica el valor de jugando */
		void setJugando(bool valor);
		
	public:
		/* Constructor. Recibe una referencia al Mapa */
		InterfazJugarSolo(Mapa* mapa);
	
		/* Indica si está jugando o no */
		bool estaJugando();
		
		/* Encapsula la edición del escenario */
		void run();
		
		/* Devuelve los puntos que hizo el usuario en la partida */
		int getPuntos();
};

#endif

