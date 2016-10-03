#include "common_Thread.h"

#include <signal.h>
#include <unistd.h>

Thread::Thread()
{
	vivo = true;
}


void Thread::start()
{
	corriendo = true;
	pthread_create(&hilo,NULL,static_run,this);
}


Thread::~Thread()
{
	if (corriendo)
	{
		vivo = false; //Indica al hilo que debe suicidarse.
		this->join();
	}

}


void Thread::join() const
{
	pthread_join(hilo,NULL);
}


bool Thread::estaVivo()
{
	bool retorno;
	bloquearEstado();
	retorno = vivo;
	desbloquearEstado();
	return retorno;
}
	

void* Thread::static_run (void* p)
{
	Thread* pthis = (Thread*) p;
	pthis->run();
	return NULL;
}


void Thread::terminar()
{
	bloquearEstado();
	vivo = false;
	desbloquearEstado();
}


bool Thread::estaCorriendo()
{
	bool retorno;
	bloquearEstado();
	retorno = corriendo;
	desbloquearEstado();
	return retorno;
}

void Thread::bloquearEstado()
{
	mHilo.lock();
}

void Thread::desbloquearEstado()
{
	mHilo.unlock();
}


void Thread::setCorriendo(bool valor)
{
	corriendo = valor;
}



