/* ==============================================================================
	
    A.P.B.
    Versi�n: 1.0.0b

    Jos� Angel Montelongo Reyes
    Javier Honorio Casado Fern�ndez
    Gustavo Adolfo Negr�n Cerde�a

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* t_raton.h */


#ifndef __T_RATON_H__
#define __T_RATON_H__


#include <dinput.h>


class T_RATON
{

public :

	LPDIRECTINPUT8 lp_di; // objeto direct input
	LPDIRECTINPUTDEVICE8 lp_raton; // puntero al dispositivo
	int i_x, // coordenada x del rat�n
		i_y, // coordenada y del rat�n
		i_z; // coordenada z del rat�n
	float f_vel_x, // velocidad de la coordenada x 
		  f_vel_y, // velocidad de la coordenada y
		  f_vel_z; // velocidad de la coordenada x
	RECT r_zona; // zona por la que puede moverse le puntero del rat�n
	BOOL b_boton_der, // bot�n derecho del rat�n
		 b_boton_cen, // bot�n central del rat�n
		 b_boton_izq; // bot�n izquierdo del rat�n

	T_RATON::T_RATON();
	// constructor de la clase

	BOOL B_Inicializar(HINSTANCE hinst, HWND hwnd);
	// inicializa el dispositivo

	BOOL B_App_Activa(BOOL b_activa);
	// toma o devuelve los privilegios sobre el rat�n

	BOOL B_Actualizar();
	// actualiza los datos
	
	void V_Liberar();
	// libera los recursos

};


#endif


/* t_raton.h */

