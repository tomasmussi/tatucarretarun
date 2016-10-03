#ifndef __PARSER_XML_H__
#define __PARSER_XML_H__

#include "Mapa.h"
#include "Elemento.h"
#include <string>
#include <libxml/xmlreader.h>


class ParserXML
{
	public:
		/* Devuelve un mapa en base a un XML levantado de un archivo */
		Mapa* obtenerMapaArchivo (const std::string& rutaEntrada) const;
		
		/* Devuelve un mapa en base a un XML que está en memoria y se pasa
		 * por parámetro */
		Mapa* obtenerMapaMemoria (const std::string& xml) const;

		/* Se genera un archivo XML en base al mapa dado */
		void obtenerXMLArchivo (const std::string& rutaSalida, const Mapa* mapa) const;
		
		/* Se genera un XML en Memoria en base a un mapa dado */
		void obtenerXMLMemoria (std::string& xml, const Mapa* mapa) const;


	private:
		/* Analiza el contenido de un nodo y en base al nombre del tag invoca al método
		 * correspondiente */
		void analizarNodo(Mapa* mapa, xmlTextReaderPtr reader) const;
		
		/* Convierte un entero a un string */
		void enteroAString(std::string& cadena, ulong entero) const;

		/* Analiza un elemento en su totalidad y lo agrega al mapa */
		void procesarElemento(Mapa* mapa, xmlTextReaderPtr reader) const;
		
		/* Lee los costos de cada elemento */
		void procesarCostos(Mapa* mapa, xmlTextReaderPtr reader) const;
		
		/* Analiza qué elementos debe habilitar y cuáles no */
		void procesarHabilitados(Mapa* mapa, xmlTextReaderPtr reader) const;

		/* Imprime qué botones están habilitados y cuales no en el mapa */
		void imprimirHabilitados(std::string& xml, const Mapa* mapa) const;
		
		/* Imprime todos los elementos del mapa */
		void imprimirElementos(std::string& xml, const Mapa* mapa) const;
		
		/* Obtiene un mapa a partir del lector dado */
		Mapa* obtenerMapa (xmlTextReaderPtr reader) const;
		
		/* Imprime el tiempo del cohete */
		void imprimirTiempo(std::string& xml, const Mapa* mapa) const;
		
		/* Imprime los costos de cada elemento */
		void imprimirCostes(std::string& xml, const Mapa* mapa) const;
		
		/* Determina el número de elemento según el tipo dado */
		int nroElemento(const std::string& tipo) const;

};


#endif

