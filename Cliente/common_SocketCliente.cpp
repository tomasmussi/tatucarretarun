#include "common_SocketCliente.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
using namespace std;

bool SocketCliente::connect(const string direccion, const int puerto)
{
	int id = getId();
	sockaddr_in& dir = getDir();

	if (id == SOCKET_INVALIDO)
	{
		if (!this->create())
			return false;
		id = this->getId();
	}

	//Prepara la conexión
	struct hostent *he;
	he = gethostbyname(direccion.c_str());

	if (he == NULL)
		return false;

	dir.sin_family = AF_INET;
	dir.sin_port = htons (puerto);
	dir.sin_addr = *((struct in_addr *)he->h_addr);

	//Intenta conectarse.
	if (::connect(id,(sockaddr*)&dir,sizeof(dir)) == -1)
		return false;

	return true;

}

