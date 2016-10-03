#include "server_HiloBatalla.h"
#include <iostream>
using namespace std;

HiloBatalla::HiloBatalla(HiloManejadorCliente* jug1, HiloManejadorCliente* jug2, std::string escenario, Mutex& m, std::vector<Mensaje*>& mensajes) : mensajesRecibidosAdministrador (mensajes), mMensajesRecibidosAdministrador(m)
{
	//inicializa atributos
	
	this->jug1 = jug1;
	this->jug2 = jug2;
	this->escenario = escenario;
	this->jugando = true;
	
	jugador1conectado = true;
	jugador2conectado = true;
	puntosJug1 = -1;
	puntosJug2 = -1;
}

bool HiloBatalla::iniciarPartida()
{

	bool listo1 = false;
	bool listo2 = false;
	
	jug1->setJugando(true);
	jug2->setJugando(true);

	//Redirecciona el flujo de los mensajes
	jug1->setMensajesRecibidos(mensajesRecibidos);
	jug2->setMensajesRecibidos(mensajesRecibidos);


	//Envía los archivos correspondientes al escenario elegido
	enviarEscenario();
	
	//Espera a que los dos estén listos pues recibieron los archivos.
	Mensaje* mensaje;
	while ((!listo1) || (!listo2))
	{

		mMensajesRecibidosAdministrador.lock();

		if (!mensajesRecibidos.empty())
		{
			mensaje = mensajesRecibidos[0];
			
			mensajesRecibidos.erase(mensajesRecibidos.begin());
		
			//Pregunta por tipo listo
			if (mensaje->getTipo() == 'L')
			{

				//Cuál está listo?
				if ((mensaje->getNombre()).compare(jug1->getNombre()) == 0)
					listo1 = true;
				else
					listo2 = true;
			}

			
			delete mensaje;
		}
		mMensajesRecibidosAdministrador.unlock();

		sleep(0);

	}

		
	//Envía la señal de que pueden jugar
	jug1->enviarMensaje("J");
	jug2->enviarMensaje("J");
	
	return true;	
}

bool HiloBatalla::enviarEscenario()
{
	//Envía fondo
	jug1->enviarArchivo(escenario+".jpg");
	jug2->enviarArchivo(escenario+".jpg");

	//Envía posiciones
	jug1->enviarArchivo(escenario+".xml");
	jug2->enviarArchivo(escenario+".xml");

	return true;
}

bool HiloBatalla::activo()
{
	bool ret;
	bloquearEstado();
	ret = jugando;
	desbloquearEstado();
	return ret;
}

void HiloBatalla::run()
{
	bool finPartida = false;
	bool listo1 = false;
	bool listo2 = false;

	Mensaje* mensaje;

	//Ejecuta la partida
	while ((!finPartida) && (estaVivo()))
	{

		mMensajesRecibidosAdministrador.lock();

		if (!mensajesRecibidos.empty())
		{
			mensaje = mensajesRecibidos[0];
			mensajesRecibidos.erase(mensajesRecibidos.begin());
		
			switch (mensaje->getTipo())
			{
				case 'C':
					//Mensaje de chat. Debe enviarse a los dos con distintos encabezados
					obtenerJugador(mensaje->getNombre())->enviarMensaje("Ctu: " + mensaje->getTexto());
					obtenerRival(mensaje->getNombre())->enviarMensaje("Cel: " + mensaje->getTexto());
					break;
				case 'M':
					//Envia un movimiento.
					obtenerRival(mensaje->getNombre())->enviarMensaje("M" + mensaje->getTexto());
					break;
				case 'L':
					//Un jugador listo para simular
					if (listo1)
						listo2 = true;
					else
					{
						listo1 = true;
						obtenerJugador(mensaje->getNombre())->enviarMensaje("X");
					}
					break;
				case 'D':
					//se desconectó uno de los jugadores
					
					//informar al otro
					obtenerRival(mensaje->getNombre())->enviarMensaje("H");
					//ajustar el resultado
					if ((mensaje->getNombre()).compare(jug1->getNombre()) == 0)
						jugador1conectado = false;
					else
						jugador2conectado = false;
					finPartida = true;
					break;
				case 'H':
					//abandonó uno de los jugadores 
					//informar al otro
					obtenerRival(mensaje->getNombre())->enviarMensaje("H");
					finPartida = true;
					break;
			}
		
			delete mensaje;
		}
	
		mMensajesRecibidosAdministrador.unlock();
		
		if ((listo1) && (listo2))
		{
			//envía simulación
			jug1->enviarMensaje("S");
			jug2->enviarMensaje("S");
			
			//espero respuesta de cada uno
			if (analizarResultado())
				finPartida = true; //alguien gano
			else
			{
				listo1 = false;
				listo2 = false;
			}
			
		}
		
		sleep(0);
	
	}
	
	if ((estaVivo()) && (listo1) && (listo2))
	{
		string leyendaResultado = "";
		
		// mando resultado
		if (puntosJug1 > puntosJug2)
			leyendaResultado = jug1->getNombre() + " gana la partida";
		else
		{
			if (puntosJug1 < puntosJug2)
				leyendaResultado = jug2->getNombre() + " gana la partida";
			else
				leyendaResultado = "Ha empatado la partida";
		}
		
		leyendaResultado = "I" + leyendaResultado; 
		jug1->enviarMensaje(leyendaResultado);
		jug2->enviarMensaje(leyendaResultado);
	}
	

	//devuelve el control al administrador
	jug1->setMensajesRecibidos(mensajesRecibidosAdministrador);
	jug2->setMensajesRecibidos(mensajesRecibidosAdministrador);
	
	jug1->setJugando(false);
	jug2->setJugando(false);
	
	bloquearEstado();
	jugando = false;
	desbloquearEstado();
}


bool HiloBatalla::analizarResultado()
{
	Mensaje* mensaje;
	bool obtuveRespuesta1 = false;
	bool obtuveRespuesta2 = false;
	bool retorno = false;
	
	while ((!obtuveRespuesta1) || (!obtuveRespuesta2))
	{
	
		mMensajesRecibidosAdministrador.lock();

		if (!mensajesRecibidos.empty())
		{
			mensaje = mensajesRecibidos[0];
			mensajesRecibidos.erase(mensajesRecibidos.begin());
		
			switch (mensaje->getTipo())
			{
				case 'D':
					//informar al otro
					obtenerRival(mensaje->getNombre())->enviarMensaje("H");
					//ajustar el resultado
					if ((mensaje->getNombre()).compare(jug1->getNombre()) == 0)
						jugador1conectado = false;
					else
						jugador2conectado = false;
					retorno = true;
					this->terminar();
					break;
					
				case 'P':
					//recibe puntaje
					if ((mensaje->getNombre()).compare(jug1->getNombre()) == 0)
					{
						puntosJug1 = atoi((mensaje->getTexto()).c_str());
						obtuveRespuesta1 = true;
					}
					else
					{
						puntosJug2 = atoi((mensaje->getTexto()).c_str());
						obtuveRespuesta2 = true;
					}
					retorno = true;
					obtuveRespuesta1 = true;
					obtuveRespuesta2 = true;
					break;
					
				case 'N':
					// no termino el mapa
					if ((mensaje->getNombre()).compare(jug1->getNombre()) == 0)
						obtuveRespuesta1 = true;
					else
						obtuveRespuesta2 = true;
					break;
			}
		
			delete mensaje;
		}
	
		mMensajesRecibidosAdministrador.unlock();
		
		sleep(0);
	
	}
	
	return retorno;

}


HiloManejadorCliente* HiloBatalla::obtenerJugador(const std::string& nombre)
{
	if (nombre.compare(jug1->getNombre()) == 0)
		return jug1;
	else
		return jug2;
}

HiloManejadorCliente* HiloBatalla::obtenerRival(const std::string& nombre)
{
	if (nombre.compare(jug1->getNombre()) == 0)
		return jug2;
	else
		return jug1;
}

bool HiloBatalla::jug1Conectado()
{
	return jugador1conectado;
}

bool HiloBatalla::jug2Conectado()
{
	return jugador2conectado;
}

int HiloBatalla::jug1Puntos()
{
	return puntosJug1;
}

int HiloBatalla::jug2Puntos()
{
	return puntosJug2;
}

HiloManejadorCliente* HiloBatalla::obtenerJugador1()
{
	return jug1;
}

HiloManejadorCliente* HiloBatalla::obtenerJugador2()
{
	return jug2;
}



