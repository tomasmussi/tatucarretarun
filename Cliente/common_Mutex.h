#ifndef	__MUTEX_H__
#define	__MUTEX_H__

#include <pthread.h>

class Mutex
{
	private:
		pthread_mutex_t mut;

		/* Constructor copia privado */
		Mutex(const Mutex& m);

		/* Operador = privado */
		Mutex& operator = (const Mutex& original);
	public:
		/* Constructor */
		Mutex();

		/* Lockea el mutex */
		void lock();

		/* Deslockea el mutex */
		void unlock();
};

#endif

