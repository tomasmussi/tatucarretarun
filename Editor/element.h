#ifndef ELEMENTO_H
#define ELEMENTO_H
#include <vector>


class Element
{
	public:
		Element(unsigned f, unsigned l);
		unsigned getFirst();
		unsigned getLast();
		std::vector<int> masas;				
	private:
		unsigned first;
		unsigned last;		
			
};
#endif
