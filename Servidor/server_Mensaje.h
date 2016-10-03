#ifndef __MENSAJE_H__
#define __MENSAJE_H__

#include <string>

class Mensaje
{
	private:
		std::string nombre; //Cliente que envió el mensaje
		char tipo; //Tipo de mensaje
		std::string texto; //Cuerpo del mensaje

	public:
		/* Constructor, recibe el mensaje completo y el nombre de quien lo envió */
		Mensaje(std::string mensaje, std::string nombre);

		/* Analiza si el mensaje es válido o no */
		bool esValido();

		/* Devuelve el nombre de quien lo envía */
		std::string& getNombre();

		/* Devuelve el tipo de mensaje */
		char getTipo();

		/* Devuelve el texto del mensaje */
		std::string& getTexto();

};


#endif

