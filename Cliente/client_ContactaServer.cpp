#include "client_ContactaServer.h"
using namespace std;

/* Mensajes */
#define MENSAJE_CAE_SERVER "Se cayó el server"
#define MENSAJE_SE_DESCONECTO "Se ha desconectado"
#define MENSAJE_DESCONECCION "/desconectar"
#define ERROR_USUARIO_EXISTENTE "Nombre de usuario existente. \n Conéctese nuevamente con otro nombre de usuario."


ContactaServer::ContactaServer()
{
	conectado = false;
	socket.create(); // crea socket
}


ContactaServer::~ContactaServer()
{
	if (socketConectado())
		desconectarSocket();
}


void ContactaServer::run()
{
	string mensaje;
	while (estaVivo())
	{
		if (socketConectado())
		{
			if (!socket.recv(mensaje))
			{
				//Se deconectó del Server
				mensaje = "DSe desconectó el server";
				setConectado(false);
				socket.close();
			}
			if (mensaje.compare("SUsuario existente") == 0)
			{
				//Ya existe el nombre de usuario del cliente en el servidor
				mensaje = "D"+mensaje;
				socket.close();
				setConectado(false);
			}

			if (mensaje.compare("D") == 0)
			{
				//Debe desconectarse
				socket.close();
				setConectado(false);
			}

			//Carga el mensaje en la lista de Mensajes Recibidos
			bloquearEstado();
			mensajesRecibidos.push_back(mensaje);
			desbloquearEstado();

		}
			
	}
}


bool ContactaServer::conectar(const string& host, const int puerto)
{
	if (socketConectado())
		desconectarSocket();
	if (socket.connect(host,puerto))
	{
		setConectado(true);
		return true;
	}

	return false;
}


bool ContactaServer::socketConectado()
{
	bool retorno;
	bloquearEstado();
	retorno = conectado;
	desbloquearEstado();
	return retorno;
}


void ContactaServer::desconectarSocket()
{
	if (!socketConectado())
		return;

	//Envía un mensaje de desconección al server
	enviarMensaje("D");
	sleep(0);

	//Espera que vuelva el mensaje de desconección desde el server y se destrabe el recv
	while(socketConectado())
		sleep(0);

}


void ContactaServer::setConectado(bool valor)
{
	bloquearEstado();
	conectado = valor;
	desbloquearEstado();
}


bool ContactaServer::obtenerMensajeRecibido(string& mensaje)
{
	bool retorno;
	bloquearEstado();
	if (!mensajesRecibidos.empty())
	{
		retorno = true;

		//Toma el primero de la lista y lo saca de la lista
		mensaje = mensajesRecibidos[0];
		mensajesRecibidos.erase(mensajesRecibidos.begin());
	}
	else
		retorno = false;
	desbloquearEstado();
	return retorno;
}


bool ContactaServer::enviarMensaje(const string& mensaje)
{
	if (socketConectado())
		return socket.send(mensaje);
	return false;
}


void ContactaServer::agregarMensaje(const string& mensaje)
{
	bloquearEstado();
	mensajesRecibidos.push_back(mensaje);
	desbloquearEstado();
}

void ContactaServer::vaciarMensajesRecibidos()
{
	bloquearEstado();
	mensajesRecibidos.clear();
	desbloquearEstado();
}


