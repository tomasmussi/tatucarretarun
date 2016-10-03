#include "server_SocketServidor.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
using namespace std;


bool SocketServidor::bind(const int puerto)
{
	int id = getId();
	sockaddr_in& dir = getDir();

	if (id == SOCKET_INVALIDO)
		return false;

	//Prepara la dirección
	dir.sin_family = AF_INET;
	dir.sin_addr.s_addr = INADDR_ANY;
	dir.sin_port = htons (puerto);

	//Realiza el bind y analiza el resultado
	if (::bind(id,(struct sockaddr*) &dir, sizeof(dir)) == -1)
		return false;
	return true;

}


bool SocketServidor::listen() const
{
	int id = getId();

	if (id == SOCKET_INVALIDO)
		return false;

	if (::listen(id,BACKLOG) == -1)
		return false;
	return true;
}
	

bool SocketServidor::accept(Socket& nuevo)
{

	int id = getId();
	sockaddr_in& dir = getDir();

	if (id == SOCKET_INVALIDO)
		return false;

	int idNuevo,longDir;
	longDir = sizeof (dir);

	//Obtiene un nuevo id.
	idNuevo = ::accept(id,(struct sockaddr*) &dir, (socklen_t*)&longDir);

	if (idNuevo == -1)
		return false;
	nuevo.setId(idNuevo);
	return true;
}

