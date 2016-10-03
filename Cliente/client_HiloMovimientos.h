#ifndef __CHAT_HILO_MOVIMIENTOS_H__
#define __CHAT_HILO_MOVIMIENTOS_H__

#include <string>
#include "common_Thread.h"
#include "Mapa.h"

class HiloMovimientos: public Thread
{
	private:
		std::string xmlListo;
		bool listo;
		
		Mapa* mapa; //referencia al mapa

	public:
		/* Constructor. Recibe referencia al mapa */
		HiloMovimientos(Mapa* mapa);
	
		/* Se va simulando el retardo */
		void run();
		
		/* Devuelve true si hay un movimiento disponible y lo carga en el parámetro */
		bool obtenerMovimiento(std::string& xml);

};


#endif

