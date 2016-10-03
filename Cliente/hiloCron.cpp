#include "hiloCron.h"
using namespace std;

HiloCron::HiloCron()
{
}


HiloCron::~HiloCron()
{
}


void HiloCron::run()
{
	cronometro = new Cronometro();
	cronometro->iniciar();
	while (estaVivo())
	{
	sleep(0);
	}
	cronometro->detener();
	delete(cronometro);
	cronometro = NULL;
}

long HiloCron::getMilisegundos(){
	if (cronometro) return cronometro->getMilisegundos();
	return -1;
}



