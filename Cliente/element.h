#ifndef ELEMENTO_H
#define ELEMENTO_H
#include <vector>


class Element
{
	public:
		Element(unsigned f, unsigned l);
		/*Devuelve la primera masa del elemento */
		unsigned getFirst();
		/*Devuelve la ultima masa que constituye
		el elemento */
		unsigned getLast();
		/*Vector que contiene los numeros de
		las masas que componen el elemento*/
		std::vector<int> masas;				
	private:
		unsigned first;
		unsigned last;		
			
};
#endif
