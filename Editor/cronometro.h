#include <time.h>
#include <ctime>
class Cronometro{
   public:
      void iniciar();
      void detener();
      long getMilisegundos();
      int estaParado();
      
   private:
      int parado;
      clock_t final, comienzo;
};

