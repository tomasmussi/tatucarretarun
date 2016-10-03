#ifndef __CHAT_DOBLE_H__
#define __CHAT_DOBLE_H__

#include <string>
#include <vector>
#include <map>
#include <SDL.h>
#include "SDL_image.h"
#include <SDL_opengl.h>
#include "common_Mutex.h"

#define MAX_LINEAS 13
#define MAX_CARACTERES 20

class ChatDoble
{

	private:
		std::vector<std::string> lineas; //Guarda las lineas del chat
		std::string buffer; //Guarda lo que está escribiendo el usuario
		std::vector<std::string> entradas; //Guarda las entradas ya ingresadas por el usuario
		Mutex m;
		std::map<char,GLuint> texturas; //texturas disponibles
		
		bool visible; //se ve el chat
		bool salir;
		
		bool simular; //debe simular
		bool bloquear; //se debe bloquear al usuario
		
		bool nuevoMovimiento; //hay nuevo movimiento disponible
		std::string movimiento; //mapa rival

		/* Carga textura */
		void loadTexture(const char * path, char letra);
		
		/* Imprime el fondo del chat */
		void imprimirFondo();
		
		/* Imprime un caracter en la posición indicada */
		void imprimirCaracter(char letra, float x, float y);
		
		
		
	public:
		/* Constructor */
		ChatDoble();
	
		/* Agrega una letra.
		 * Devuelve false si el buffer está lleno */
		bool agregarLetra(char letra);
		
		// backspace
		bool quitarLetra();
		
		// enter del chat
		bool enter();
		
		/* Agrega línea al chat, pisa una si hace falta */
		void agregarLinea(const std::string& linea);
		
		/* Imprime todo el chat */
		void imprimirChat();
		
		/* Obtener entrada del usuario. False si no hay más */
		bool obtenerEntrada(std::string& entrada);
		
		/* Devuelve una letra si la mapea. -1 si no lo hace */
		char esLetra(SDL_Event evento);
		
		/* Carga las texturas del chat */
		void cargarTexturas();
		
		/* Modifica el valor de la visibilidad del chat */
		void cambiarVisible();
		
		/* Pregunta si la ventana debe salir */
		bool debeSalir();
		
		/* Modifica el valor de salir */
		void setSalir(bool valor);
		
		/* Carga el mapa del rival en xml.
		 * Devuelve true si hay mapa disponible, false si no */
		bool obtenerMovimiento(std::string& xml);
		
		/* Modifica el movimiento que se está almacenando */
		void cambiarMovimiento(const std::string& xml);
		
		/* Imprime el fondo del mapa rival */
		void imprimirFondoRival();
		
		/* Indica si el Servidor mandó a simular */
		bool debeSimular();
		
		/* Modifica el valor de simular */
		void setSimular(bool valor);
		
		/* Indica si debe bloquear los botones */
		bool debeBloquear();
		
		/* Modifica el valor de bloquar */
		void setBloquear(bool valor);
		
		/* Agrega un mensaje a las entradas */
		void agregarMensaje(const std::string& mensaje);
		
};

#endif

