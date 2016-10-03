#ifndef __SOCKET_CLIENTE_H__
#define __SOCKET_CLIENTE_H__


#include "common_Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

class SocketCliente: public Socket
{
	public:
		/* Trata de conectarse a un Socket que se encuentra escuchando
		 * en la dirección y puerto indicados.
		 * Devuelve true si fue una conexión exitosa. */
		bool connect(const std::string direccion, const int puerto);
};

#endif

