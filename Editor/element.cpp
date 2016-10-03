#include "element.h"
		
Element::Element(unsigned f, unsigned l){
	first = f;
	last  = l;
}

unsigned Element::getFirst(){
	return masas.at(0);
}

unsigned Element::getLast(){
	return masas.at(masas.size()-1);
}


