#ifndef ZONA_LLEGADA_H
#define ZONA_LLEGADA_H

class ZonaLlegada
{
	public:
		ZonaLlegada(double x, double y , double width, double heigth);
		/*Devuelven las propiedades de la zona de llegada*/
		/*posicion*/
		double getX();
		double getY();
		/*dimensiones*/
		double getWidth();
		double getHeigth();				
	private:
		double x;
		double y;
		double width;
		double heigth;	
			
};
#endif
