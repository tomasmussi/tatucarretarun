#include <time.h>
#include <ctime>
class Cronometro{
   public:
      /*Resetea el cronometro y comienza a contar*/
      void iniciar();
      /*Detiene el cronometro y almacena el tiempo transcurrido
      desde que se inicio*/
      void detener();
      /*Devuelve el tiempo desde el inicio en ms.*/
      long getMilisegundos();
      /*Devuelve:
      	0 si esta encendido
      	1 si está parado*/
      int estaParado();
      
   private:
      int parado;
      clock_t final, comienzo;
};

