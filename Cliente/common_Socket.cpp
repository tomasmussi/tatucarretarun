#include "common_Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
using namespace std;

Socket::Socket()
{
	//Aún no está creado
	id = SOCKET_INVALIDO;

	//Inicializa dirección y buffer de remanentes
	memset(&(dir.sin_zero),'\0', 8);
	memset(historial, 0, TAM_BUFFER + 1 );
	validosHistorial = 0;
}

Socket::~Socket()
{
	if (id != SOCKET_INVALIDO)
		::close(id); //Cierra Socket
}


bool Socket::create()
{
	//Crea Socket
	id = socket(AF_INET,SOCK_STREAM,0);

	//Analiza resultado
	if (id == SOCKET_INVALIDO)
		return false;
	return true;
}


bool Socket::send(const string& dato) const
{
	unsigned int enviados = 0;
	int retorno;
	string aux = dato;

	/* Envía hasta que la cantidad de enviados sea el tamaño del dato más el \0
	 *que indica fin de mensaje. */
	while (enviados != (dato.size()+1))
	{
		retorno = ::send (id,aux.c_str(),dato.size() + 1 - enviados, MSG_NOSIGNAL);
		if (retorno == -1)
			return false;
		enviados += retorno; //Suma los enviados.
		if (enviados != (dato.size()+1))
			aux = dato.substr(enviados); //Carga en aux los datos que faltan enviar
	}
	return true;

}


bool Socket::recv(string& dato)
{

	char buffer [TAM_BUFFER +1];
	bool listo = false;
	dato = "";
	int retorno;
	unsigned int recibidos;

	//Analiza si hay elementos remanentes que son parte del mensaje actual.
	if (validosHistorial != 0)
	{
		dato = historial; //Toma hasta el primer \0
		if (dato.size() != validosHistorial) //Si el dato no tomó todos los remanentes es porque encontró un \0
		{
			listo = true;
			validosHistorial -= (dato.size()+1); //Resto el tamaño del mensaje mas el \0
		}
		else
			validosHistorial = 0;

		//Coloca en historial los nuevos remanentes.
		memcpy(buffer,historial+dato.size()+1,validosHistorial);
		memset(historial, 0, TAM_BUFFER + 1 );
		memcpy(historial,buffer,validosHistorial);

	}

	//Inicializa el buffer
	memset (buffer,0,TAM_BUFFER + 1);

	//Recibe hasta encontrar el \0
	while (!listo)
	{
		retorno = ::recv(id,buffer,TAM_BUFFER, 0 );
		if ((retorno == -1) || (retorno == 0))
			return false;
		recibidos = retorno;
		if (strlen(buffer) < recibidos)
		{
			listo = true;
			if (strlen(buffer) < recibidos - 1)
				//Tengo que copiar a los remanentes
				validosHistorial = recibidos-(strlen(buffer)+1);
				memcpy(historial,buffer+strlen(buffer)+1,validosHistorial);
		}
		dato += buffer; //Concatena lo leído al dato.
		memset(buffer, 0, TAM_BUFFER + 1 );
	}
	return true;
}


void Socket::setId(const int id)
{
	this->id = id;
}

int Socket::getId() const
{
	return id;
}

sockaddr_in& Socket::getDir()
{
	return dir;
}


void Socket::close()
{
	if (id == SOCKET_INVALIDO)
		return;
	::close(id);
	id = SOCKET_INVALIDO;
}

void Socket::shutdown(int modo)
{
	if (id == SOCKET_INVALIDO)
		return;
	::shutdown(id,modo);
	id = SOCKET_INVALIDO;
}



