#ifndef __HILO_CRON_H__
#define __HILO_CRON_H__

#include "common_Thread.h"
#include <iostream>
#include "cronometro.h"


class HiloCron: public Thread
{

	private:
		Cronometro* cronometro;

	public:

		/* Constructor */
		HiloCron();

		/* Destructor */
		virtual ~HiloCron();
	
		long getMilisegundos();

		/* Función que ejecuta el hilo. */
		void run();


};

#endif

