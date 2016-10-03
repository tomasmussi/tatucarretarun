#include "server_HiloManejadorCliente.h"
#include <fstream>
using namespace std;


HiloManejadorCliente::HiloManejadorCliente(const string& nombre, const Socket& socket,Mutex& m, vector<Mensaje*>& mensajes): mMensajesRecibidos(m) , mensajesRecibidos(&mensajes)
{
	this->socket = socket;
	this->nombre = nombre;
	this->jugando = false;
}

HiloManejadorCliente::~HiloManejadorCliente()
{
	socket.close();
}

void HiloManejadorCliente::desconectar()
{
	socket.shutdown(0);
}

void HiloManejadorCliente::run()
{
	string cadMensaje;
	Mensaje* mensaje;
	bool pideFin = false;

	while (!pideFin)
	{

		if (!socket.recv(cadMensaje))
		{
			//Desapareció el Cliente
			cadMensaje = "D";
			pideFin = true;
		}
		else if (cadMensaje[0] == 'D')
			pideFin = true;

		mensaje = new Mensaje(cadMensaje,nombre);

		//Coloca en la lista el mensaje junto a su nombre.
		mMensajesRecibidos.lock();
		mensajesRecibidos->push_back(mensaje);
		mMensajesRecibidos.unlock();
		sleep(0);
	}
	setCorriendo(false);
}

bool HiloManejadorCliente::estaJugando()
{
	return jugando;
}

void HiloManejadorCliente::setJugando(bool valor)
{
	this->jugando = valor;
}

void HiloManejadorCliente::enviarMensajeChat(Mensaje* mensaje)
{
	socket.send("C" + mensaje->getNombre() + " dice: " + mensaje->getTexto());
}

void HiloManejadorCliente::enviarMensaje(string mensaje)
{
	socket.send(mensaje);
}

void HiloManejadorCliente::setMensajesRecibidos(std::vector<Mensaje*>& mensajes)
{
	mMensajesRecibidos.lock();
	mensajesRecibidos = &mensajes;
	mMensajesRecibidos.unlock();
}
		
std::vector<Mensaje*>& HiloManejadorCliente::getMensajesRecibidos()
{
	return *mensajesRecibidos;
}

void HiloManejadorCliente::enviarArchivo(string ruta)
{
	ifstream arch(ruta.c_str(),ios::binary);
	string mensaje = "A";
	char buf;	

	while (!arch.eof())
	{
		arch.get(buf);
		if (!arch.eof())
			convertir(mensaje,buf);
	}

	socket.send(mensaje);

	arch.close();
}

char HiloManejadorCliente::ntoh(char n)
{
	switch (n)
	{
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
		case 10: return 'a';
		case 11: return 'b';
		case 12: return 'c';
		case 13: return 'd';
		case 14: return 'e';
		case 15: return 'f';
	}
	return -1;
}

void HiloManejadorCliente::convertir(string& mensaje, char buf)
{
	// codifica el archivo
	
	char nibble;
	char aux[3];
	aux[2] = '\0';

	nibble = buf >> 4;
	nibble = nibble & 15;

	aux[0] = ntoh(nibble);

	nibble = buf & 15;
	aux[1] = ntoh(nibble);
	
	mensaje += aux;
}

string& HiloManejadorCliente::getNombre()
{
	return nombre;
}

bool HiloManejadorCliente::escenarioJugado(const std::string& nombre)
{
	bool retorno = false;
	bloquearEstado();
	
	// recorre los escenarios jugados a ver si encuentra el dado
	for (unsigned int i = 0 ; i < escenariosJugados.size() ; i++)
		if (nombre.compare(escenariosJugados[i]) == 0)
		{
			retorno = true;
			break;
		}
		
	desbloquearEstado();
	return retorno;
}

void HiloManejadorCliente::agregarEscenarioJugado(const std::string& nombre)
{
	bloquearEstado();
	escenariosJugados.push_back(nombre);
	desbloquearEstado();
}
		
void HiloManejadorCliente::vaciarEscenariosJugados()
{
	bloquearEstado();
	escenariosJugados.clear();
	desbloquearEstado();
}


