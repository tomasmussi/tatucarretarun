#ifndef __SERVER_H__
#define __SERVER_H__

#include "common_Thread.h"
#include "server_SocketServidor.h"
#include "common_Mutex.h"
#include <vector>
#include <iostream>
using namespace std;


class Server: public Thread
{
	private:
		int puerto; //Puerto del Servidor
		SocketServidor recepcionista; //Recibe nuevas conexiones

	public:
		/* Lógica principal del Server. Espera a que lleguen nuevos usuarios y se los agrega a la lista */
		void run();

		/* Inicializa el Servidor. Lo crea, hace el bind y listen */
		bool inicializar(const int puerto);
};

#endif

