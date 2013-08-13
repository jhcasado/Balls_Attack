/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* intro.h */


#ifndef __INTRO_H__
#define __INTRO_H__


#include "t_imagen.h"


BOOL B_Modo_Actual(int & i_ancho, int & i_alto, BYTE & by_bpp);
// devuelve las caracteristicas del modo actual

BOOL B_Apagar_Pantalla(T_IMAGEN & pantalla);
// realiza el scroll de presentación

void V_Scroll_Inicial(HWND hwnd, T_IMAGEN & pantalla);
// llama a la función que realiza el scroll de presentación


#endif


/* intro.h */
