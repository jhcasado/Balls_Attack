/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* t_imagen.h */


#ifndef __T_IMAGEN_H__
#define __T_IMAGEN_H__

#define MAX_TABLA_LUM       65536

#define X1		0
#define Y1		1
#define X2		2
#define Y2		3


#include <ddraw.h>


class T_IMAGEN 
{

public :

    int i_ancho, // ancho de la imagen
		i_alto, // alto de la imagen
		i_buffer_posterior; // número de buffers posteriores	
	BOOL b_pantalla; // var que indica si la superficie apunta a la pantalla o no
    LPDIRECTDRAWSURFACE7 lp_datos, // puntero a los datos de la supeficie
		                 lp_pantalla; // puntero a los datos de la pantalla
	DDSURFACEDESC2 ddsd; // datos con información acerca de la superficie
	
	T_IMAGEN(); 
	// constructor de la clase
	
	void V_Liberar(); 
	// libera los recursos

	BYTE By_Bpp();
	// devuelve el nº de bits por pixel

	BOOL B_Recortar(int i_xd, int i_yd, T_IMAGEN & destino, 
				    int i_xo1, int i_yo1, int i_xo2, int i_yo2, RECT & ro, RECT & r_d);
	// recorta la imagen
	
	BOOL B_Recortar_Origen(int i_xo1, int i_yo1, int i_xo2, int i_yo2, RECT & r_o); 
	// recorta el cuadro de origen

    BOOL B_Crear(int i_x, int i_y); 
	// crea una imagen : ancho, alto

	BOOL B_Cargar(char *s_nombre); 
	// carga una imagen desde un fichero
    
	BOOL B_Borrar(DWORD dw_color, 
		          int i_xo1 = -1, int i_yo1 = -1, int i_xo2 = -1, int i_yo2 = -1); 
	// borra la imagen con el color especificado																				 	
    
	BOOL B_Pegar(int i_xd, int i_yd, T_IMAGEN & destino, BOOL b_sprite = FALSE,
	             int i_xo1 = -1, int i_yo1 = -1, int i_xo2 = -1, int i_yo2 = -1); 
	// pega la imagen en destino

	BOOL B_Luminosidad(int i_lum, int xo1 = -1, int yo1 = -1, int xo2 = -1, int yo2 = -1); 
	// varia el brillo de la región indicada

	BOOL B_Pegar_Luminosidad(int i_lum, int i_xd, int i_yd, T_IMAGEN & destino,
				           int i_xo1 = -1, int i_yo1 = -1, int i_xo2 = -1, int i_yo2 = -1); 
	// pegar con brillo

    BOOL B_Transparencia(int i_trans, DWORD dw_color_t, 
		                 int i_xo1 = -1, int i_yo1 = -1, int i_xo2 = -1, int i_yo2 = -1); 
	// genera un cuadro con un grado de transparencia trans y un color color_t

	BOOL B_Pegar_Transparencia(int i_trans, int i_xd, int i_yd, T_IMAGEN & destino, 
		                       BOOL b_sprite = FALSE, 
							   int i_xo1 = -1, int i_yo1 = -1, int i_xo2 = -1, int i_yo2 = -1); 
	// pega con transparencia					         

	BOOL B_Dibujar_Rectangulo(int i_trans, DWORD dw_color, 
		    		          int i_xo1, int i_yo1, int i_xo2, int i_yo2);
	// dibuaja un rectángulo en pantalla

	BOOL B_Pantalla();
	// prepara la imagen actual para poder apuntar a la pantalla

	BOOL B_Poner_Mondo_Video(HWND hwnd, int i_x, int i_y, BYTE by_bpp);
	// cambia el modo de vide actual

	BOOL B_Flip();
	// realiza el intercambio entre al buffer principal y el secundario

	void V_Pos_RGB(int &i_r, int &i_g, int &i_b);
	// devuelve la pos de cada una de las componentes de color

	void V_Mascara_RGB(DWORD &i_r, DWORD &i_g, DWORD &i_b);
	// devuelva la mascara de cada componente

	DWORD DW_RGB(DWORD dw_r, DWORD dw_g, DWORD dw_b);
	// transforma los componentes en un valor r, g, b

	DWORD DW_Color(int i_px, int i_py);
	// devuelve el color de la pos pasada por parámetros

	BOOL Restaurar_Modo_Video();
	// restaura el modo de video 

};


#endif


/* t_imagen.h */


