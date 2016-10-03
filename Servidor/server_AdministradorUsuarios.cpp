#include "server_AdministradorUsuarios.h"
#include "StringEntero.h"
#include <map>
#include <cstdlib>
#include <fstream>
#include <string>
using namespace std;

#define CANT_LIDERES 5


void AdministradorUsuarios::levantarEscenarios(const std::string& rutaEscenarios)
{
	ifstream arch(rutaEscenarios.c_str());
	string linea;
	
	//Cada nombre de escenario es una linea
	while (getline(arch,linea,'\n'))
		escenarios.push_back(linea);
	arch.close();
}

void AdministradorUsuarios::levantarPuntajes()
{
	string nombre;
	string puntos;
	ifstream arch(rutaPuntajes.c_str());
	while (!arch.eof())
	{
		// Lee el nombre seguido del puntaje
		arch >> nombre;
		if (!arch.eof())
		{
			arch >> puntos;
			puntajes.insert(make_pair(nombre,atoi(puntos.c_str())));
		}
	}
	arch.close();
}

AdministradorUsuarios::AdministradorUsuarios(const std::string rutaPuntajes, const std::string rutaEscenarios)
{
	this->rutaPuntajes = rutaPuntajes;
	levantarPuntajes();
	levantarEscenarios(rutaEscenarios);
	cantPartidas = 0;
}

void AdministradorUsuarios::grabarPuntajes()
{
	ofstream arch(rutaPuntajes.c_str());
	map<string,unsigned int>::iterator it = puntajes.begin();
	// Graba un nombre de usuario seguido de su correspondiente puntaje
	while (it != puntajes.end())
	{
		arch << it->first << " " << it->second << " ";
		it++;
	}
	arch.close();
}


AdministradorUsuarios::~AdministradorUsuarios()
{
	grabarPuntajes();
	
	//elimina los hilos batallas
	vector<HiloBatalla*>::iterator itDes = desafios.begin();
	HiloBatalla* actual;
	while (itDes != desafios.end())
	{
		actual = *itDes;
		actual->terminar();
		actual->join();
		delete actual;
		itDes++;
	}
	
	//elimina los hilos manejadores de clientes
	map<string,HiloManejadorCliente*>::iterator it = usuarios.begin();
	while (it != usuarios.end())
	{
		it->second->desconectar();
		it->second->terminar();
		it->second->join();
		delete it->second;
		it++;
	}
	
	//elimina los mensajes remanentes
	Mensaje* mensaje;
	mMensajesRecibidos.lock();
	while (!mensajesRecibidos.empty())
	{
		mensaje = mensajesRecibidos[0];		
		mensajesRecibidos.erase(mensajesRecibidos.begin());
		delete mensaje;
	}
	mMensajesRecibidos.unlock();
}

bool AdministradorUsuarios::pedirEscenario(std::string& nombre, HiloManejadorCliente* cliente)
{
	if (escenarios.size() == 0)
		return false; // no hay escenarios
		
		
	vector<string> disponibles;
	
	//Obtiene los escenarios que todavía no jugó este usuario
	for (unsigned int i = 0 ; i < escenarios.size() ; i++)
		if (!cliente->escenarioJugado(escenarios[i]))
			disponibles.push_back(escenarios[i]);
		
	if (disponibles.size() == 0)
		return false; // ya jugó todos los escenarios
		
	//Elije un nombre random
	nombre = disponibles[(rand() % disponibles.size())];
	cliente->agregarEscenarioJugado(nombre);
	nombre = "esc/" + nombre;
	
	return true;
}


bool AdministradorUsuarios::pedirEscenario(std::string& nombre)
{
	if (escenarios.size() == 0)
		return false; // no hay escenarios
		
	//Elije un nombre random
	nombre = escenarios[(rand() % escenarios.size())];
	nombre = "esc/" + nombre;
	
	return true;
}


bool AdministradorUsuarios::actualizarPuntaje(std::string nombre, unsigned int puntos)
{
	bool retorno;
	bloquearEstado();
	// busca al usuario
	map<string,unsigned int>::iterator it = puntajes.find(nombre);
	if (it != puntajes.end())
	{
		//suma puntos
		it->second += puntos;
		retorno = true;
	}
	else
		retorno = false; //No existe el usuario
	desbloquearEstado();
	return retorno;
}


bool AdministradorUsuarios::usuarioExiste(std::string nombre)
{
	bool retorno;
	bloquearEstado();
	// hace búsqueda del usuario y analiza su existencia
	map<string,unsigned int>::iterator it = puntajes.find(nombre);
	if (it != puntajes.end())
		retorno = true;
	else
		retorno = false; //No existe el usuario
	desbloquearEstado();
	return retorno;
}

bool AdministradorUsuarios::conectarJugador(const std::string nombre, Socket& socket)
{

	bool retorno;
	HiloManejadorCliente* manejador;

	bloquearEstado();

	//Busca si existe en nombre
	map<string,HiloManejadorCliente*>::iterator it = usuarios.find(nombre);

	if (it == usuarios.end())
	{
		// informa al resto que se conectó el usuario
		enviarMensajeATodos("C  " + nombre + " se ha conectado");
		
		// crea el nuevo manejador del cliente
		manejador = new HiloManejadorCliente(nombre,socket,mMensajesRecibidos,mensajesRecibidos);
		usuarios.insert(make_pair(nombre,manejador));

		//Hace correr al hilo
		manejador->start();
		retorno = true;
	}
	else
		retorno = false; //No puede existir el usuario

	desbloquearEstado();
		
		
	return retorno;
}

bool AdministradorUsuarios::agregarUsuario(std::string nombre)
{

	bool retorno;
	bloquearEstado();

	//Busca si existe en nombre
	map<string,unsigned int>::iterator it = puntajes.find(nombre);

	if (it == puntajes.end())
	{
		//agrega el usuario
		puntajes.insert(make_pair(nombre,0));
		retorno = true;
	}
	else
		retorno = false; //No puede existir el usuario previamente

	desbloquearEstado();

	
	return retorno;
}


void AdministradorUsuarios::desconectarJugador(std::string nombre)
{

	bloquearEstado();
	map<string,HiloManejadorCliente*>::iterator it = usuarios.find(nombre); //Busca al usuario
	if (it != usuarios.end())
	{
		// mensaje que desbloquea el socket del cliente
		it->second->enviarMensaje("D");

		// termina y elimina el hilo que maneja al cliente
		it->second->join();
		delete it->second;
		usuarios.erase(it);
	}
	//envía mensaje de desconexión al resto de los usuarios
	enviarMensajeATodos("C  " + nombre + " se ha desconectado");
	desbloquearEstado();
}



void AdministradorUsuarios::iniciarJuegoSolitario(std::string nombre)
{
	string escenario,fondo,posiciones;

	bloquearEstado();
	map<string,HiloManejadorCliente*>::iterator it = usuarios.find(nombre);
	if (it != usuarios.end())
	{
		if (cantPartidas == MAX_PARTIDAS)
			it->second->enviarMensaje("O"); //ya está lleno el cupo de partidas
		else
		{
			// pide escenario que no se haya jugado
			if (pedirEscenario(escenario,it->second))
			{
				// envía los dos archivos correspondientes al escenario
				fondo = escenario + ".jpg";
				posiciones = escenario + ".xml";
				it->second->enviarArchivo(fondo);
				it->second->enviarArchivo(posiciones);
				it->second->setJugando(true);
				cantPartidas++;

			}
			else
			{
				it->second->enviarMensaje("T"); //Termino todos los niveles
				it->second->vaciarEscenariosJugados();
			}
		}
	}
	desbloquearEstado();
}



void AdministradorUsuarios::enviarMensajeChat(Mensaje* mensaje)
{

	bloquearEstado();
	map<string,HiloManejadorCliente*>::iterator it = usuarios.begin();
	while (it != usuarios.end())
	{
		// envía el mensaje a todos los que no están jugando
		if (!(it->second->estaJugando()))
			it->second->enviarMensajeChat(mensaje);
		it++;
	}
	desbloquearEstado();
}


void AdministradorUsuarios::enviarMensajeATodos(const std::string& mensaje)
{
	map<string,HiloManejadorCliente*>::iterator it = usuarios.begin();
	while (it != usuarios.end())
	{
		// envía el mensaje a todos los usuarios que no están jugando
		if (!(it->second->estaJugando()))
			it->second->enviarMensaje(mensaje);
		it++;
	}
}



void AdministradorUsuarios::finPartidoSolitario(Mensaje* mensaje)
{
	bloquearEstado();

	map<string,HiloManejadorCliente*>::iterator it = usuarios.find(mensaje->getNombre());

	// indica que no está jugando
	if (it != usuarios.end())
		it->second->setJugando(false);
		
	cantPartidas--;

	desbloquearEstado();

	actualizarPuntaje(mensaje->getNombre(),atoi((mensaje->getTexto()).c_str()));
}

void AdministradorUsuarios::enviarUsuarios(const std::string& nombre)
{
	string mensaje = "U";
	HiloManejadorCliente* hilo = NULL;

	bloquearEstado();
	
	map<string,HiloManejadorCliente*>::iterator it = usuarios.begin();
	
	// concatena los nombres de usuarios distintos al dado con un \n que separa
	while (it != usuarios.end())
	{
		if (nombre.compare(it->first) != 0)
			mensaje += it->first + "\n";
		else
			hilo = it->second;
		it++;
	}


	// envía el mensaje
	if (hilo != NULL)
		hilo->enviarMensaje(mensaje);
		
		
	desbloquearEstado();
	
}


void AdministradorUsuarios::invitarDesafio(Mensaje* mensaje)
{
	string propuesta = "B";
	propuesta += mensaje->getNombre();

	bloquearEstado();

	map<string,HiloManejadorCliente*>::iterator it = usuarios.find(mensaje->getTexto());

	if ((it != usuarios.end()) && (!it->second->estaJugando()))
		it->second->enviarMensaje(propuesta); // envía propuesta
	else
	{
		it = usuarios.find(mensaje->getNombre());
		propuesta = "R";
		propuesta += mensaje->getTexto();
		if (it == usuarios.end())
			propuesta += "\nNo está conectado el usuario";
		else
			propuesta += "\nEl usuario ya está jugando";
		it->second->enviarMensaje(propuesta);
	}
		
	desbloquearEstado();
}

void AdministradorUsuarios::notificarRechazo(Mensaje* mensaje)
{
	string notificacion = "R";
	notificacion += mensaje->getNombre();
	notificacion += "\nHa rechazado su propuesta.";

	bloquearEstado();
	map<string,HiloManejadorCliente*>::iterator it = usuarios.find(mensaje->getTexto());
	
	if (it != usuarios.end())
		//Envío notificación de rechazo de desafío
		it->second->enviarMensaje(notificacion);
	
	desbloquearEstado();
}

void AdministradorUsuarios::aceptarDesafio(Mensaje* mensaje)
{

	string notificacion;

	//Tengo que analizar si el desafiante no empezó a jugar mientras el desafiado aceptaba
	bloquearEstado();
	
	map<string,HiloManejadorCliente*>::iterator it = usuarios.find(mensaje->getTexto());
	
	//Primero ver si está conectado todavía
	if (it == usuarios.end())
	{
		notificacion = "R";
		notificacion += mensaje->getTexto();
		notificacion += "\nYa no está conectado.";
	}
	//Analiza si no empezó a jugar otra partida
	else if (it->second->estaJugando())
	{
		notificacion = "R";
		notificacion += mensaje->getTexto();
		notificacion += "\nEstá jugando otra partida.";
	}
	else
	{
		HiloManejadorCliente* j1;
		HiloManejadorCliente* j2;
		HiloBatalla* nuevoHilo;
		string escenario;
		
		
		//Arranca todo el proceso de una partida multijugador
		
		//Envío mensaje de desafío.
		j1 = it->second;
		it = usuarios.find(mensaje->getNombre());
		j2 = it->second;
		
		if (cantPartidas == MAX_PARTIDAS)
		{
			// cupo de partidas ocupado
			j1->enviarMensaje("O");
			j2->enviarMensaje("O");
		}
		else
		{
			pedirEscenario(escenario);

			// indica inicio de partida
			j1->enviarMensaje("Z"+mensaje->getNombre());
			j2->enviarMensaje("Z"+mensaje->getTexto());

			//crea hilo batalla
			nuevoHilo = new HiloBatalla(j1,j2,escenario,mMensajesRecibidos,mensajesRecibidos);
			desafios.push_back(nuevoHilo);
			nuevoHilo->iniciarPartida(); //inicializa la batalla
			nuevoHilo->start();
			cantPartidas++;
		}
	}
	
	desbloquearEstado();
	
	
}

void AdministradorUsuarios::analizarMensaje(Mensaje* mensaje)
{
	// actúa en función del tipo de mensaje
	char tipo = mensaje->getTipo();
	switch (tipo)
	{
		case 'D': 
			desconectarJugador(mensaje->getNombre());
			break;
		case 'J':
			iniciarJuegoSolitario(mensaje->getNombre());
			break;
		case 'C':
			enviarMensajeChat(mensaje);
			break;
		case 'P':
			finPartidoSolitario(mensaje);
			break;
		case 'U':
			enviarUsuarios(mensaje->getNombre());
			break;
		case 'B':
			invitarDesafio(mensaje);
			break;
		case 'R':
			notificarRechazo(mensaje);
			break;
		case 'A':
			aceptarDesafio(mensaje);
			break;	
		case 'G':
			enviarRanking(mensaje->getNombre());
			break;	
	}
}


void AdministradorUsuarios::enviarRanking(const std::string& nombre)
{
	string ranking;
	
	//busca la tabla de posiciones
	obtenerRanking(ranking,nombre);
	
	bloquearEstado();
	map<string,HiloManejadorCliente*>::iterator it = usuarios.find(nombre);
	if (it != usuarios.end())
	{
		it->second->enviarMensaje(ranking);
	}
	desbloquearEstado();
	
}

void AdministradorUsuarios::mantenerDesafios()
{
	vector<HiloBatalla*>::iterator it = desafios.begin();
	HiloBatalla* actual;
	while (it != desafios.end())
	{
		actual = *it;
		if (!actual->activo())
		{
			//Analizar resultado
			if (actual->jug1Puntos() > 0)
				actualizarPuntaje(actual->obtenerJugador1()->getNombre(),actual->jug1Puntos());
			if (actual->jug2Puntos() > 0)
				actualizarPuntaje(actual->obtenerJugador2()->getNombre(),actual->jug2Puntos());			
		
		
			//Analiza si se desconecto alguien
			if (!actual->jug1Conectado())
				desconectarJugador(actual->obtenerJugador1()->getNombre());
			if (!actual->jug2Conectado())
				desconectarJugador(actual->obtenerJugador2()->getNombre());
			
			//Limpia lo que quedó del desafío
			actual->join();
			delete actual;
			desafios.erase(it);
			it = desafios.begin();
		}
		else
			it++;
	}
}


void AdministradorUsuarios::obtenerRanking(std::string& ranking, const std::string& jugador)
{
	ranking = "";
	unsigned int posJugador = 1;
	unsigned int puntosJugador;
	string menor;
	unsigned int ptosMenor;

	map<string,unsigned int> lideres;

	bloquearEstado();

	// busca los puntos del jugador que pide la tabla
	map<string,unsigned int>::iterator it = puntajes.find(jugador);
	if (it == puntajes.end())
	{
		desbloquearEstado();
		return;
	}
	puntosJugador = it -> second;
	
	it = puntajes.begin();

	while (it != puntajes.end())
	{
		if (jugador.compare(it -> first) != 0)
			if (it->second > puntosJugador)
				posJugador++; // cae una posición
		if (lideres.size() < CANT_LIDERES)
			lideres.insert(make_pair(it->first,it->second));
		else
		{
			obtenerMenor(lideres,menor,ptosMenor);
			for (unsigned int i = 0 ; i < CANT_LIDERES ; i ++)
			{
				if (ptosMenor < it->second)
				{
					//agrega a los líderes
					lideres.erase(menor);
					lideres.insert(make_pair(it->first,it->second));
				}
			}
		}
		it ++;
	}

	//hasta acá tengo el mapa con los líderes hay que ordenarlo.
	unsigned int i = lideres.size();
	string nro;
	
	//imprimo posicion del que pidio el ranking
	StringEntero::enteroAString(nro,puntosJugador);
	ranking = " " + nro; //puntos
	StringEntero::enteroAString(nro,posJugador);
	ranking = "Tu posición: \n    " + nro + ". " + jugador + ranking;
	ranking = "\n\n" + ranking;
	
	while (!lideres.empty())
	{
		obtenerMenor(lideres,menor,ptosMenor);
		
		StringEntero::enteroAString(nro,ptosMenor);
		ranking = menor + " " + nro + ranking;
		
		StringEntero::enteroAString(nro,i);
		ranking = "\n    " + nro + ". " + ranking;
		
		lideres.erase(menor);
		
		i--;
	}

	ranking = "GTabla de posiciones\n" + ranking;

	desbloquearEstado();
	
	
}

void AdministradorUsuarios::obtenerMenor(map<string,unsigned int>& lideres, std::string& menor, unsigned int& ptosMenor)
{
	map<string,unsigned int>::iterator it = lideres.begin();
	ptosMenor = it-> second;
	menor = it->first;
	it++;
	while (it != lideres.end())
	{
		if (it->second < ptosMenor)
		{
			menor = it->first;
			ptosMenor = it->second;
		}
		it++;
	}
}


void AdministradorUsuarios::run()
{
	Mensaje* mensaje;
	while (estaVivo())
	{
		mMensajesRecibidos.lock();
			//saca un mensaje de la lista
			if (!mensajesRecibidos.empty())
			{
				mensaje = mensajesRecibidos[0];		
				mensajesRecibidos.erase(mensajesRecibidos.begin());
			}
			else
				mensaje = NULL;
		mMensajesRecibidos.unlock();

		if (mensaje != NULL)
		{
			analizarMensaje(mensaje); //Analiza el mensaje y actúa en consecuencia
			delete mensaje;
		}
			
			mantenerDesafios();

		sleep(0);
	}		
	setCorriendo(false);
}



