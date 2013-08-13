/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* t_texto.h */


#ifndef __T_TEXTO_H__
#define __T_TEXTO_H__


#include "t_imagen.h"


class T_TEXTO
{

public :

	int i_ancho, // ancho de la fuente de texto
		i_alto; // alto de la fuente de texto
	T_IMAGEN fuente; // imagen que contiene la fuente

	void V_Liberar();
	// libera los recursos

	BOOL B_Cargar(char *s_nombre, int i_ancho_fuente, int i_alto_fuente);
	// carga una fuente de texto
		
	BOOL B_Escribir(int i_x, int i_y, char *s_texto, T_IMAGEN & destino, int i_trans = 255); 
	// escribe s_texto en las pos (i_x, i_y) de la imagen destino
		
};


#endif


/* t_texto.h */

