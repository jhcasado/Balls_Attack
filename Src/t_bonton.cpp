

/* t_boton.h */


#ifndef __T_BOTON_H__
#define __T_BOTON_H__

#define NORMAL      0
#define ARRIBA      1
#define ACTIVO      2


#include "t_imagen.h"


class T_ESTILO_BOTON
{

public : 

	T_IMAGEN imagen; // imagen con los datos del bot�n
	int pi_region [3][4]; // vector con los rect�ngulos que forman los botones

	void V_Liberar();
	// libera la memoria

	BOOL B_Cargar(char *s_imagen); 
	// carga un bot�n desde un fichero 

};


class T_BOTON
{

public :

	char *s_texto; // texto del bot�n
	int i_x, // origen x
		i_y, // origen y
		i_trans; // cantidad de transparencia

	B_Arriba(T_ESTILO_BOTON, T_RATON);
	// informa si estamos arriba del bot�n

	B_Click(T_ESTILO_BOTON, T_RATON);
	// informa si hemos pulsado el bot�n y lo dibuja seg�n su estado

}

#endif


/* t_boton.h */

