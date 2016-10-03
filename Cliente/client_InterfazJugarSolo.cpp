#include "client_InterfazJugarSolo.h"
#include "InterfazEdicion.h"

InterfazJugarSolo::InterfazJugarSolo(Mapa* mapa)
{
	this->mapa = mapa;
	jugando = true;
	puntos = 0;
}

bool InterfazJugarSolo::estaJugando()
{
	bool ret;
	bloquearEstado();
	ret = jugando;
	desbloquearEstado();
	return ret;
}

int InterfazJugarSolo::getPuntos()
{
	int ret;
	bloquearEstado();
	ret = puntos;
	desbloquearEstado();
	return ret;
}

void InterfazJugarSolo::setJugando(bool valor)
{
	bloquearEstado();
	jugando = valor;
	desbloquearEstado();
}
		
void InterfazJugarSolo::run()
{

	InterfazEdicion interfaz(mapa);
	interfaz.comenzar(NULL);
	
	//Guardo los puntos que hizo el jugador
	bloquearEstado();
	//set puntos
	puntos = mapa->getPuntos();
	desbloquearEstado();
	
	setJugando(false);
}

