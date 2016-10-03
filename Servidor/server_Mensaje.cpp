#include "server_Mensaje.h"

#define MENSAJE_INVALIDO 0

Mensaje::Mensaje(std::string mensaje, std::string nombre)
{
	this->nombre = nombre;
	if (mensaje.length() == 0)
	{
		tipo = MENSAJE_INVALIDO; //Invalido
		texto = "";
	}
	else
	{
		tipo = mensaje[0];
		texto = mensaje.substr(1,mensaje.length()-1);
	}
}

bool Mensaje::esValido()
{
	return (tipo != MENSAJE_INVALIDO);
}


std::string& Mensaje::getNombre()
{
	return nombre;
}


char Mensaje::getTipo()
{
	return tipo;
}


std::string& Mensaje::getTexto()
{
	return texto;
}


