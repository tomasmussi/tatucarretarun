#ifndef ZONA_LLEGADA_H
#define ZONA_LLEGADA_H

class ZonaLlegada
{
	public:
		ZonaLlegada(double x, double y , double width, double heigth);
		double getX();
		double getY();
		double getWidth();
		double getHeigth();				
	private:
		double x;
		double y;
		double width;
		double heigth;	
			
};
#endif
