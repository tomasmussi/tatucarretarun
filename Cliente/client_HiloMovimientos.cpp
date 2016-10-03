#include "client_HiloMovimientos.h"
#include "ParserXML.h"
using namespace std;

#define TIEMPO_RETARDO 1000000 //en microsegundos
#define CANT_INTERVALOS 10

HiloMovimientos::HiloMovimientos(Mapa* mapa)
{
	listo = false;
	this-> mapa = mapa;
}

void HiloMovimientos::run()
{
	ParserXML par;
	string xmlEspera;
	unsigned int intervalos = 0;
	while (estaVivo())
	{
		if (intervalos == CANT_INTERVALOS)
		{
			bloquearEstado();
			xmlListo = xmlEspera; //ya revela el movimiento
			listo = true;
			desbloquearEstado();
			intervalos = 0;
			par.obtenerXMLMemoria(xmlEspera,mapa); //obtiene un nuevo movimiento a revelar en el futuro
		}
		else
		{
			intervalos++;
			usleep(TIEMPO_RETARDO / CANT_INTERVALOS); //retardo
		}
	}
}

bool HiloMovimientos::obtenerMovimiento(std::string& xml)
{
	bool retorno = true;
	bloquearEstado();
	if (!listo)
		retorno = false;
	else
	{
		xml = this->xmlListo;
		listo = false; //empieza de nuevo
	}
	desbloquearEstado();
	return retorno;
}

