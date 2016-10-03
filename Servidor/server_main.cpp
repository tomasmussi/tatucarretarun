#include "server_Server.h"
#include "common_SocketCliente.h"
#include <iostream>
#include <string>
using namespace std;

const char* MENSAJE_CERRAR = "salir";
const char* MENSAJE_CONEXION_PUERTO = "Se conecta al puerto ";
int PUERTO_DEFAULT = 3000;

bool modoDebug;

int main(int argc, char* argv[])
{
	Server server;
	string buffer;
	bool cerrar = false;
	modoDebug = false;
	int puerto;

	//Busca el puerto indicado por línea de comando o utiliza el puerto por default
	if (argc < 2)
		puerto = 3000;
	else
		puerto = atoi(argv[1]);

	if (argc > 2)
		if (strcmp("paquetes",argv[2]) == 0)
			modoDebug = true;

	if (server.inicializar(puerto))
		cout << MENSAJE_CONEXION_PUERTO << puerto << endl;
	else
		cerrar = true;

	//Inicia el Servidor
	server.start();

	//Espera el mensaje de cerrar el servidor
	while (!cerrar)
	{
		getline(cin,buffer,'\n');

		if (buffer.compare(MENSAJE_CERRAR) == 0)
			cerrar = true;
		sleep(0);
	}

	//Hace terminar al Servidor enviando una conexión nueva que destrabe el accept
	server.terminar();
	if (server.estaCorriendo())
	{
		SocketCliente mensajeFin;
		mensajeFin.create();
		mensajeFin.connect("127.0.0.1",puerto);
	}
	server.join();

}

