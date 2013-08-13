/* ==============================================================================
	
    A.P.B.
    Versi�n: 1.0.0b

    Jos� Angel Montelongo Reyes
    Javier Honorio Casado Fern�ndez
    Gustavo Adolfo Negr�n Cerde�a

    Multimedia
    Universidad de las Palmas de Gran Canaria	
	  	  
   ============================================================================== */

/* t_boton.h */


#ifndef __T_BOTON_H__
#define __T_BOTON_H__

#define NORMAL      0
#define ARRIBA      1
#define PULSADO     2


#include "t_imagen.h"
#include "t_texto.h"
#include "t_raton.h"


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

	int i_x, // origen x
		i_y, // origen y
		i_trans; // cantidad de luminosidad
	BOOL b_pulsado; // informa si hemos pulsado el bot�n

	T_BOTON();
	// constructor de la clase

	BOOL B_Arriba(T_ESTILO_BOTON estilo, T_RATON raton, BOOL b_dibujar = TRUE);
	// indica si nos encontramos arriba del boton

	BOOL B_Click(T_ESTILO_BOTON estilo, T_RATON raton, T_IMAGEN & destino, BOOL b_dibujar = TRUE);
	// dibuja el bot�n e informa si se ha pulsado

};


#endif


/* t_boton.h */

