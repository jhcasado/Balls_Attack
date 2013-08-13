/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* t_piezas.h */


#ifndef __T_PIEZAS_H__
#define __T_PIEZAS_H__

#define PRINCIPAL    1
#define SECUNDARIA   2

#define BLANCA   1
#define NEGRA	 2


// declaración de la clase T_TABLERO_MOV


class T_TABLERO_MOV
{

public :

	int i_ex, i_ey; // coordenadas de la pieza que movemos al movernos a esa pos
	BOOL b_movimiento; // TRUE si nos podemos mover a esa pos, si no FALSE

	T_TABLERO_MOV(); 
	// constructor de la clase

};


// declaración de la clase T_PIEZA


class T_PIEZA
{

public :

	int i_tipo, // PRINCIPA o SECUNDARIA
		i_color, // BLANCO o NEGRO
		i_contador, // número de movimientos 
		i_x1, i_y1, i_x2, i_y2, // pos dentro de una imagen
		i_cox, i_coy, i_cfx, i_xfy, // pos para dibujarla
		i_cont_dibujo; // orden de dibujo

	T_TABLERO_MOV p_movimientos [9][12]; // array con los movimientos válidos

	T_PIEZA(int i_tipo_p, int i_color_p, 
		    int i_x1_p, int i_y1_p, int i_x2_p, int i_y2_p);
	// contructor de la clase

	virtual void V_Generar_Movimientos(int i_sx, int i_sy, T_PIEZA *p_tablero [9][12]) = 0;
	// nos devulve una tabla con los movimientos válidos
		
};


// declaración de la clase T_SECUNDARIA


class T_SECUNDARIA : public T_PIEZA
{

public :
	
	T_SECUNDARIA(int i_color_p, 
				 int i_x1_p, int i_y1_p, int i_x2_p, int i_y2_p) : 
	T_PIEZA(SECUNDARIA, i_color_p, i_x1_p, i_y1_p, i_x2_p, i_y2_p) { }
	// constructor de la clase

	void V_Generar_Movimientos(int i_sx, int i_sy, T_PIEZA *p_tablero [9][12]);
	// nos devuelve una tabla con los movimientos válidos

};


// declaración de la clase T_PRINCIPAL


class T_PRINCIPAL : public T_PIEZA
{

public :
	
	T_PRINCIPAL(int i_color_p,
			    int i_x1_p, int i_y1_p, int i_x2_p, int i_y2_p) : 
	T_PIEZA(PRINCIPAL, i_color_p, i_x1_p, i_y1_p, i_x2_p, i_y2_p) { }
	// constructor de la clase

	void V_Generar_Movimientos(int i_sx, int i_sy, T_PIEZA *p_tablero [9][12]);
	// nos devuelve una tabla con los movimientos válidos

};


#endif


/* t_piezas.h */

