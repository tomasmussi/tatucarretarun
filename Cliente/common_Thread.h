#ifndef	__THREAD_H__
#define	__THREAD_H__

#include <pthread.h>
#include "common_Mutex.h"



class Thread
{
	private:
		pthread_t hilo;
		Mutex mHilo; //Evita que se lean y escriban los atributos al mismo tiempo.
		bool vivo; //True si tiene que seguir corriendo
		bool corriendo; //True si el hilo está corriendo

		/* Constructor copia privado */
		Thread(const Thread& t);

		/* Operador = privado */
		Thread& operator = (const Thread& original);

	public:
		/* Constructor */
		Thread();

		/* Destructor */
		virtual ~Thread();

		/* Le indica al hilo que debe suicidarse */
		void terminar();

		/* Devuelve true si el hilo aún está corriendo */
		bool estaCorriendo();

		/* Método estático que se empieza a correr al crearse el hilo de tipo pthread */
		static void* static_run (void* p);

		virtual void run() = 0;

		/* Espera que el hilo termine su ejecución */
		void join() const;

		/* Devuelve true si el hilo debe seguir vivo */
		bool estaVivo();

		/* Método que inicia la ejecución del hilo */
		void start();

	protected:
		/* Bloquear y desbloquear estado se utilizan para evitar que se acceda
		 * a un atributo del hilo al mismo tiempo (desde el hilo mismo y desde el
		 * hilo que lo creó */
		void bloquearEstado();
		void desbloquearEstado();

		/* Modifica el valor de corriendo */
		void setCorriendo(bool valor);

};

#endif

