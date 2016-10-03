#include "StringEntero.h"

void StringEntero::enteroAString(std::string& cadena, ulong entero)
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

