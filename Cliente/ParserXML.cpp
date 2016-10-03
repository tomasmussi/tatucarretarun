#include "ParserXML.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <iostream>
#include <list>
#include <fstream>
using namespace std;

Mapa* ParserXML::obtenerMapaArchivo (const std::string& rutaEntrada) const
{
	xmlTextReaderPtr reader;

	reader = xmlReaderForFile(rutaEntrada.c_str(), NULL, 0);
	
	return obtenerMapa(reader);
}

Mapa* ParserXML::obtenerMapa (xmlTextReaderPtr reader) const
{
	Mapa* mapa = new Mapa();
	
	int ret;
	if (reader != NULL) 
	{
		ret = xmlTextReaderRead(reader);
		while (ret == 1) 
		{
			analizarNodo(mapa,reader);
			ret = xmlTextReaderRead(reader);
		}
	}
	mapa->actualizarEnlaces();
	mapa->setNroElementoParaCrear(NINGUN_ELEMENTO);
	return mapa;
}


int ParserXML::nroElemento(const std::string& tipo) const
{
	string nom = tipo.substr(tipo.find_last_of(":")+1,tipo.length()-tipo.find_last_of(":")-1);

	if (nom.compare("Masa") == 0)
		return MASA;
		
	if (nom.compare("Rueda") == 0)
		return RUEDA;
	
	if (nom.compare("PuntoFijo") == 0)
		return PUNTO_FIJO;
	
	if (nom.compare("Barra") == 0)
		return BARRA;
	
	if (nom.compare("Plataforma") == 0)
		return PLATAFORMA;	

	if (nom.compare("Soga") == 0)
		return SOGA;
	
	if (nom.compare("Cinta") == 0)
		return CINTA;
	
	if (nom.compare("Cohete") == 0)
		return COHETE;
		
	if (nom.compare("ZonaLlegada") == 0)
		return ZONA_LLEGADA;
		
	return NINGUN_ELEMENTO;
	
}

void ParserXML::procesarElemento(Mapa* mapa, xmlTextReaderPtr reader) const
{
	const xmlChar *value;
    	string tipo;
    	int nroTipo;
	char* tipoAux;
	long angulo;
	ulong magnitud;
	Coordenada coor;
	
	xmlTextReaderRead(reader);

	xmlTextReaderRead(reader);    
	xmlTextReaderRead(reader); //leo el tipo
	value = xmlTextReaderConstValue(reader);
		tipoAux = (char*) value;
		tipo = tipoAux;
		nroTipo = nroElemento(tipo);
	xmlTextReaderRead(reader);    
	xmlTextReaderRead(reader);
	
	mapa->setNroElementoParaCrear(nroTipo);

	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader); //leo la magnitud
	value = xmlTextReaderConstValue(reader);
		magnitud = atoi ((const char*) value);
	xmlTextReaderRead(reader);    
	xmlTextReaderRead(reader);

	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader); //leo el angulo
	value = xmlTextReaderConstValue(reader);
		angulo = atoi ((const char*) value);
	xmlTextReaderRead(reader);    
	xmlTextReaderRead(reader);

	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader); //leo la posicion

	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader); //leo x
	value = xmlTextReaderConstValue(reader);
		coor.x = atoi ((const char*) value);
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);

	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader); //leo y
	value = xmlTextReaderConstValue(reader);
		coor.y = atoi ((const char*) value);
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);

	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	mapa->agregarElemento(coor);

    
    	if ((nroTipo >= BARRA) && (nroTipo <= COHETE))
    	{
    		Coordenada extremoOpuesto;
    		
		xmlTextReaderRead(reader);
		xmlTextReaderRead(reader);
		
		xmlTextReaderRead(reader);
		xmlTextReaderRead(reader);
		//leo x
		value = xmlTextReaderConstValue(reader);
		extremoOpuesto.x = atoi((const char*) value);
		xmlTextReaderRead(reader);
		xmlTextReaderRead(reader);
		
		xmlTextReaderRead(reader);
		xmlTextReaderRead(reader);
		//leo y
		value = xmlTextReaderConstValue(reader);
		extremoOpuesto.y = atoi((const char*) value);
		xmlTextReaderRead(reader);
		xmlTextReaderRead(reader);
		
		xmlTextReaderRead(reader);
		xmlTextReaderRead(reader);
		
		mapa->setExtremoOpuesto(extremoOpuesto);
    	
    	}
    	
	mapa->agrandarElementoA(magnitud);
	mapa->rotarElementoA(angulo);
    
    
//	xmlTextReaderRead(reader);
//	xmlTextReaderRead(reader);
    
	xmlTextReaderRead(reader);


}


void ParserXML::procesarHabilitados(Mapa* mapa, xmlTextReaderPtr reader) const
{

	const xmlChar *value;
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo masa
	if (strcmp((const char*)value,"NO") == 0)
		mapa->deshabilitarElemento(MASA);
	else
		mapa->habilitarElemento(MASA);
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo punto fijo
	if (strcmp((const char*)value,"NO") == 0)
		mapa->deshabilitarElemento(PUNTO_FIJO);
	else
		mapa->habilitarElemento(PUNTO_FIJO);
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo rueda
	if (strcmp((const char*)value,"NO") == 0)
		mapa->deshabilitarElemento(RUEDA);
	else
		mapa->habilitarElemento(RUEDA);
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo barra
	if (strcmp((const char*)value,"NO") == 0)
		mapa->deshabilitarElemento(BARRA);
	else
		mapa->habilitarElemento(BARRA);
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo plataforma
	if (strcmp((const char*)value,"NO") == 0)
		mapa->deshabilitarElemento(PLATAFORMA);
	else
		mapa->habilitarElemento(PLATAFORMA);
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo soga
	if (strcmp((const char*)value,"NO") == 0)
		mapa->deshabilitarElemento(SOGA);
	else
		mapa->habilitarElemento(SOGA);
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo cinta
	if (strcmp((const char*)value,"NO") == 0)
		mapa->deshabilitarElemento(CINTA);
	else
		mapa->habilitarElemento(CINTA);
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo cohete
	if (strcmp((const char*)value,"NO") == 0)
		mapa->deshabilitarElemento(COHETE);
	else
		mapa->habilitarElemento(COHETE);
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	
}


void ParserXML::procesarCostos(Mapa* mapa, xmlTextReaderPtr reader) const
{

	const xmlChar *value;
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo masa
	mapa->setPrecio(MASA,atoi((const char*)value));
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo punto fijo
	mapa->setPrecio(PUNTO_FIJO,atoi((const char*)value));

	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo rueda

	mapa->setPrecio(RUEDA,atoi((const char*)value));
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo barra

	mapa->setPrecio(BARRA,atoi((const char*)value));
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo plataforma

	mapa->setPrecio(PLATAFORMA,atoi((const char*)value));
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo soga

	mapa->setPrecio(SOGA,atoi((const char*)value));
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo cinta

	mapa->setPrecio(CINTA,atoi((const char*)value));
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	value = xmlTextReaderConstValue(reader); //leo cohete

	mapa->setPrecio(COHETE,atoi((const char*)value));
	xmlTextReaderRead(reader);
	xmlTextReaderRead(reader);
	
	xmlTextReaderRead(reader);
	
}



void ParserXML::analizarNodo(Mapa* mapa, xmlTextReaderPtr reader) const
{

	const xmlChar *name, *value;
	char* valueAux;

	name = xmlTextReaderConstName(reader);

	if (strcmp((const char*)name,"nombre") == 0)
	{
		xmlTextReaderRead(reader);
		value = xmlTextReaderConstValue(reader);
		valueAux = (char*) value;
		mapa->setNombre(valueAux);//asignar el nombre al mapa
		xmlTextReaderRead(reader);
		xmlTextReaderRead(reader);
	}
	else if (strcmp((const char*)name,"plata") == 0)
	{
		xmlTextReaderRead(reader);
		value = xmlTextReaderConstValue(reader);
		valueAux = (char*) value;
		mapa->setPlata(atoi(valueAux));
		xmlTextReaderRead(reader);
	}
	else if (strcmp((const char*)name,"elemento") == 0)
		procesarElemento(mapa,reader);
		
	else if (strcmp((const char*)name,"habilitados") == 0)
		procesarHabilitados(mapa,reader);
	
	else if (strcmp((const char*)name,"costes") == 0)
		procesarCostos(mapa,reader);
	
	else if (strcmp((const char*)name,"TiempoCohete") == 0)
	{
		xmlTextReaderRead(reader);
		value = xmlTextReaderConstValue(reader);
		valueAux = (char*) value;
		mapa->setTiempoCohete(atoi(valueAux));
		xmlTextReaderRead(reader);
	}

}



Mapa* ParserXML::obtenerMapaMemoria (const std::string& xml) const
{
	xmlTextReaderPtr reader;

	reader = xmlReaderForMemory(xml.c_str(), xml.length(), NULL, NULL, 0);
	
	return obtenerMapa(reader);
}

void ParserXML::obtenerXMLArchivo (const std::string& rutaSalida, const Mapa* mapa) const
{
	string xml;
	obtenerXMLMemoria(xml,mapa);
	ofstream arch(rutaSalida.c_str());
	arch.write(xml.c_str(),xml.length());
	arch.close();
}

void ParserXML::enteroAString(std::string& cadena, ulong entero) const
{
	if (entero == 0)
		cadena = "0";
	else
		cadena = "";
	while (entero != 0)
	{
		switch (entero %10)
		{
			case 0: cadena="0"+cadena; break;
			case 1: cadena="1"+cadena; break;
			case 2: cadena="2"+cadena; break;
			case 3: cadena="3"+cadena; break;
			case 4: cadena="4"+cadena; break;
			case 5: cadena="5"+cadena; break;
			case 6: cadena="6"+cadena; break;
			case 7: cadena="7"+cadena; break;
			case 8: cadena="8"+cadena; break;
			case 9: cadena="9"+cadena; break;
		}
		entero /= 10;
	}
}
	

void ParserXML::imprimirElementos(std::string& xml, const Mapa* mapa) const
{
	list<Elemento*> lista = mapa->getListaElementos();
	Elemento* actual;
	string nro;
	Coordenada coorActual;
	list<Elemento*>::iterator it = lista.begin();
	while (it != lista.end())
	{
		actual = *it;
		xml += "		";
		xml += "<elemento>\n";

		xml += "			<tipo>";
		xml += actual->getNombre();
		xml += "</tipo>\n";

		xml += "			<magnitud>";
		enteroAString(nro,actual->getMagnitud());
		xml += nro;
		xml += "</magnitud>\n";

		xml += "			<angulo>";
		if ((actual->getAngulo()) < 0)
			enteroAString(nro,actual->getAngulo()+360);
		else
			enteroAString(nro,actual->getAngulo());
		xml += nro;
		xml += "</angulo>\n";

		coorActual = actual->getPosicion();

		xml += "			<posicion>\n";

		xml += "				<x>";
		enteroAString(nro,coorActual.x);
		xml += nro;
		xml += "</x>\n";

		xml += "				<y>";
		enteroAString(nro,coorActual.y);
		xml += nro;
		xml += "</y>\n";


		xml += "			</posicion>\n";
		
		
		if (actual->es(LONGITUDINAL))
		{
			coorActual = actual->getExtremoOpuesto();

			xml += "			<extremo_opuesto>\n";

			xml += "				<x>";
			enteroAString(nro,coorActual.x);
			xml += nro;
			xml += "</x>\n";

			xml += "				<y>";
			enteroAString(nro,coorActual.y);
			xml += nro;
			xml += "</y>\n";


			xml += "			</extremo_opuesto>\n";
		}
		
		
		xml += "		";
		xml += "</elemento>\n";
		

		
		it++;
	}
}
	

void ParserXML::obtenerXMLMemoria (std::string& xml, const Mapa* mapa) const
{
	string plata;
	enteroAString(plata,mapa->getPlata());
	
	xml.clear();
	
/*	xml += "<?xml version=";
	xml += '"';
	xml += "1.0";
	xml += '"';
	xml += " encoding=";
	xml += '"';
	xml += "UTF-8";
	xml += '"';
	xml += "?>\n";	*/
	
	xml += "<mapa>\n";
	xml += "	";
	xml += "<nombre>";
	xml += mapa->getNombre();
	xml += "</nombre>\n";
	xml += "	";
	xml += "<plata>";
	xml += plata;
	xml += "</plata>\n";
	xml += "	";
	xml += "<elementos>\n";
	imprimirElementos(xml,mapa);
	xml += "</elementos>\n";
	imprimirHabilitados(xml,mapa);
	imprimirCostes(xml,mapa);
	imprimirTiempo(xml,mapa);
	xml += "	";
	xml += "</mapa>";
}

void ParserXML::imprimirHabilitados(std::string& xml, const Mapa* mapa) const
{
	xml += "	<habilitados>\n";

	xml += "		<Masa>";
	if (mapa->elementoHabilitado(MASA))
		xml += "SI";
	else
		xml += "NO";
	xml += "</Masa>\n";

	xml += "		<Punto_Fijo>";
	if (mapa->elementoHabilitado(PUNTO_FIJO))
		xml += "SI";
	else
		xml += "NO";
	xml += "</Punto_Fijo>\n";
	
	xml += "		<Rueda>";
	if (mapa->elementoHabilitado(RUEDA))
		xml += "SI";
	else
		xml += "NO";
	xml += "</Rueda>\n";
	
	xml += "		<Barra>";
	if (mapa->elementoHabilitado(BARRA))
		xml += "SI";
	else
		xml += "NO";
	xml += "</Barra>\n";
	
	xml += "		<Plataforma>";
	if (mapa->elementoHabilitado(PLATAFORMA))
		xml += "SI";
	else
		xml += "NO";
	xml += "</Plataforma>\n";
	
	xml += "		<Soga>";
	if (mapa->elementoHabilitado(SOGA))
		xml += "SI";
	else
		xml += "NO";
	xml += "</Soga>\n";
	
	xml += "		<Cinta>";
	if (mapa->elementoHabilitado(CINTA))
		xml += "SI";
	else
		xml += "NO";
	xml += "</Cinta>\n";
	
	xml += "		<Cohete>";
	if (mapa->elementoHabilitado(COHETE))
		xml += "SI";
	else
		xml += "NO";
	xml += "</Cohete>\n";
	
	xml += "	</habilitados>\n";

}

void ParserXML::imprimirCostes(std::string& xml, const Mapa* mapa) const
{
	string nro;

	xml += "	<costes>\n";

	xml += "		<Masa>";
	enteroAString(nro,mapa->getPrecio(MASA));
	xml += nro;
	xml += "</Masa>\n";

	xml += "		<Punto_Fijo>";
	enteroAString(nro,mapa->getPrecio(PUNTO_FIJO));
	xml += nro;
	xml += "</Punto_Fijo>\n";
	
	xml += "		<Rueda>";
	enteroAString(nro,mapa->getPrecio(RUEDA));
	xml += nro;
	xml += "</Rueda>\n";
	
	xml += "		<Barra>";
	enteroAString(nro,mapa->getPrecio(BARRA));
	xml += nro;
	xml += "</Barra>\n";
	
	xml += "		<Plataforma>";
	enteroAString(nro,mapa->getPrecio(PLATAFORMA));
	xml += nro;
	xml += "</Plataforma>\n";
	
	xml += "		<Soga>";
	enteroAString(nro,mapa->getPrecio(SOGA));
	xml += nro;
	xml += "</Soga>\n";
	
	xml += "		<Cinta>";
	enteroAString(nro,mapa->getPrecio(CINTA));
	xml += nro;
	xml += "</Cinta>\n";
	
	xml += "		<Cohete>";
	enteroAString(nro,mapa->getPrecio(COHETE));
	xml += nro;
	xml += "</Cohete>\n";
	
	xml += "	</costes>\n";
}

void ParserXML::imprimirTiempo(std::string& xml, const Mapa* mapa) const
{
	string nro;
	enteroAString(nro,mapa->getTiempoCohete());
	xml += "	<TiempoCohete>";
	xml += nro;
	xml += "</TiempoCohete>\n";
}



