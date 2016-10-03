#include "server_Server.h"
#include "server_AdministradorUsuarios.h"

#define RUTA_PUNTAJES "puntajes.txt"
#define RUTA_ESCENARIOS "escenarios.txt"

void Server::run()
{

	Socket usuarioNuevo;
	string nombre;
	AdministradorUsuarios admin(RUTA_PUNTAJES,RUTA_ESCENARIOS);

	//Inicia al Administrador de Usuarios
	admin.start();

	while (estaVivo())
	{

		//Espera una nueva conexión
		if (!recepcionista.accept(usuarioNuevo)) cout << "Falla accept" << endl;

		if (!estaVivo())
			break;

		//Espera nombre
		if (usuarioNuevo.recv(nombre))
		{

			if (nombre[0] == 'E')
			{
			//Usuario existente
				
				//Analiza si efectivamente existe
				if (!admin.usuarioExiste(nombre.substr(1,nombre.length()-1)))
				{
					usuarioNuevo.send("SNo existe el usuario");
					usuarioNuevo.close();
				}
				
				else
				{
			
					if (admin.conectarJugador(nombre.substr(1,nombre.length()-1),usuarioNuevo))
						usuarioNuevo.send("SSe ha conectado exitosamente");

					else
					{
						usuarioNuevo.send("SYa está jugando ese usuario");
						usuarioNuevo.close();
					}
				}
			}
			if (nombre[0] == 'N')
			//Usuario nuevo
			
				//Analiza si lo puede agregar
				if (!admin.agregarUsuario(nombre.substr(1,nombre.length()-1)))
				{
					usuarioNuevo.send("SUsuario existente");
					usuarioNuevo.close();
				}

				else
				{
					admin.conectarJugador(nombre.substr(1,nombre.length()-1),usuarioNuevo);
					usuarioNuevo.send("SSe ha conectado exitosamente");
				}
		}
	}

	//Termina al administrador
	admin.terminar();
	admin.join();
	this->setCorriendo(false);


}


bool Server::inicializar(const int puerto)
{
	if (!recepcionista.create())
	{
		cout << "Falla create" << endl;
		return false;
	}

	if (!recepcionista.bind(puerto))
	{
		cout << "Falla bind" << endl;
		return false;
	}

	if (!recepcionista.listen())
	{
		cout << "Falla listen" << endl;
		return false;
	}
	return true;
}



