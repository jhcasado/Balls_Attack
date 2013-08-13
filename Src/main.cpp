/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

/* main.cpp */


#include <stdio.h>
#include <windows.h>
#include <windowsx.h>


#include "apb.h"


static BOOL bActive, b_sin_red = TRUE;


long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_ACTIVATEAPP:
			
		    bActive = wParam;
			App_Activa(bActive);

		break;

		case WM_KEYDOWN:

			switch (wParam)
			{				
				case VK_ESCAPE:			
					Cancelar_Partida();
					//Cerrar_Conexion();
					//PostMessage(hWnd, WM_CLOSE, 0, 0);            
				break;

				case VK_F5:
					b_sin_red = !(b_sin_red);
					Jugar_Red(b_sin_red);
				break;
			}

        break;

        case WM_DESTROY:

			  PostQuitMessage(0);
        
		break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
} 


int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;    
    WNDCLASS wc;
	HWND hwnd;
    lpCmdLine = lpCmdLine;
    hPrevInstance = hPrevInstance;
    char *nombre = " ";
    char *titulo = " ";

// **************

     // Inicializar y registrar la clase de la ventana
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = nombre;
    wc.lpszClassName = nombre;
    RegisterClass(&wc);
    
    // Crear una ventana que ocupe toda la pantalla
    hwnd = CreateWindowEx(WS_EX_TOPMOST,
                          nombre,
                          titulo,
                          WS_POPUP,
                          0, 0,
                          GetSystemMetrics(SM_CXSCREEN),
                          GetSystemMetrics(SM_CYSCREEN),
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    if (!hwnd)
	{
		return FALSE;
	}
    
	ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

// **************
	
	if (!Iniciar_Conexion())
	{
		Error("No se puede establecer la conexión\n");
	}

// **************

	if (!Iniciar_Raton(hInstance, hwnd))
	{
		Error("Imposible inicializar el ratón\n");
		return FALSE;
	}

	if (!Iniciar_Modo_Video(hwnd))
	{
		Error("Imposible inicializar el modo de video\n");
		return FALSE;
	}
	
	if (!Cargar_Ficheros())
	{
		Restaurar_Modo_Video();
		Error("Error al cargar un fichero\n");
		return FALSE;
	}

// **************

    for (;;)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            if (!GetMessage(&msg, NULL, 0, 0))
            {
                return msg.wParam;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else if (bActive)
        {   

// **************

			if (!MAIN())
			{
				PostMessage(hwnd, WM_CLOSE, 0, 0);
			}
				
// **************

        }

        else
        {
            WaitMessage();
        }
 
	}

    return msg.wParam;	
}


/* main.cpp */
