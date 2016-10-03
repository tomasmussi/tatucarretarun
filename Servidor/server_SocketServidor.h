#ifndef __SOCKET_SERVIDOR_H__
#define __SOCKET_SERVIDOR_H__

#include "common_Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

class SocketServidor: public Socket
{
	public:

		/* Asigna el puerto al que se asocia el socket.
		 * Se utiliza para los servidores y el puerto pasado
		 * es el puerto al que se conectarán los clientes
		 * Devuelve true si fue una operación exitosa. */
		bool bind(const int puerto);

		/* Para un servidor.
		 * Establece la cantidad de conexiones en espera que
		 * puede tener hasta hacer un accept.
		 * Empieza a "escuchar" conexiones entrantes.
		 * Devuelve true si fue una operación exitosa. */
		bool listen() const;

		/* Acepta una nueva conexión que se conecta a nosotros.
		 * Carga en nuevo el Socket que surge de este proceso y a partir
		 * del cual se empezarán a comunicar.
		 * Devuelve true si fue una operación exitosa. */
		bool accept(Socket& nuevo);

};

#endif

