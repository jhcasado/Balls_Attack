/* ==============================================================================
	
    A.P.B.
    Versión: 1.0.0b

    José Angel Montelongo Reyes
    Javier Honorio Casado Fernández
    Gustavo Adolfo Negrín Cerdeña

    Multimedia
    Universidad de las Palmas de Gran Canaria	

	  	  
   ============================================================================== */

#include <dplay.h>
#include <dplobby.h>

/********* TIPOS DE MENSAJES RECIVIDOS ****************/
// Se ha producido un error al intentar recibir:
#define MSG_ERROR				0x00000000

// Se ha encontrado un mensaje que es del sistema
// y no está soportado por mi wrapper:
#define MSG_DESCONOCIDO         0x00000001

// No hay mensajes en la cola:
#define MSG_NO_HAY				0x00000002

// Ha entrado un jugador nuevo
// el puntero pasado apuntará al nombre del jugador
// y el dword pasado será el id del jugador nuevo 
// los id de los demás es importante que los almacenes
// si vas a mandar mensajes sólo a uno jugando más de
// dos personas:
#define MSG_CREA_JUGADOR		0x00000010

// Ha salido un jugador
// el puntero pasado apuntará al nombre del jugador
// y el dword pasado será el id del jugador (extraerlo de tu lista):
#define MSG_DESTRUYE_JUGADOR	0x00000020

// El mensaje se ha enviado satisfactoriamente
// el puntero = NULL
// el dword = tiempo en milisegundos de lo que tardó en llegar
// NOTA: se supone que todos los mensajes se envían
// de manera segura, o sea, este mensaje si quieres
// lo tienes en cuenta y si quieres no
#define MSG_MENSAJE_ENVIADO		0x00000040

// El dueño de la sesión la ha cerrado
// Automáticamente se destruye tu jugador y se sale de la sesión
#define MSG_SESION_PERDIDA		0x00000080

// Ha llegado uno de tus mensajes
// el puntero = inicio de tu mensaje
// el dword = tamaño de tu mensaje:
#define MSG_PERSONAL			0x00000100
/*******************************************************/


/***** PARA ENVIAR MENSAJES A TODOS LOS JUGADORES ******/
//Envío del mensaje a todos los jugadores menos a ti mismo
//Ideal para un juego de 2 personas :)
#define ID_TODOS DPID_ALLPLAYERS
/*******************************************************/


/**********************  DE USO INTERNO **********************/
//Nombre máximo para las sesiones
#define MAX_NOMBRE     255

struct T_LISTA
//Estructura de la lista usada en las enumeraciones
//Todas las funciones/variables relacionadas con la lista: L_/l_
{
	DPID id;							//Sólo en la enumeración de jugadores
	GUID guid;							//Sólo en la enumeración de sesiones
	char nombre[MAX_NOMBRE];			//Nombre del jugador o sesion
	T_LISTA *sig;
};
/*************************************************************/



class RED
/*************************************************************/
/*                             RED                           */
/*************************************************************/
{
private:
	//Objeto direct play
	LPDIRECTPLAY4A lp_direct_play;

	//Variables de control
	BOOL b_conexion_abierta;
	BOOL b_sesion_abierta;

	//Id del jugador actual
	DPID id_jugador;

	//Control interno de la lista encadenada
	T_LISTA l_primero;
	DWORD l_n;
	
	T_LISTA *L_N (DWORD n);
	BOOL L_iSesion (LPCDPSESSIONDESC2 lpThisSD);
	BOOL L_iJugador (DPID id, LPCDPNAME lpName);
	BOOL L_Borra_Lista ();

	//Inicializa Direct Play
	BOOL B_Inicializar_Direct_Play ();

	//Funciones de enumeración
	friend BOOL FAR PASCAL EnumSessionsCallback2 
		(LPCDPSESSIONDESC2 lpThisSD, LPDWORD lpdwTimeOut, 
		DWORD dwFlags, LPVOID lpContext);
	friend BOOL FAR PASCAL EnumPlayersCallback2
		(DPID dpId, DWORD dwPlayerType, LPCDPNAME lpName, DWORD dwFlags,
		LPVOID lpContext);

public:
/************    Constructor y Destructor       ************/
	RED ();
	~RED();


/*********** Control sobre las conexiones disponibles ************/
	BOOL B_TCPIP_Disponible ();
	BOOL B_IPX_Disponible ();


/************   Inicio y cierre de las onexiones    ************/
	//Para que esto tire bien en el mismo ordenador
	//en dirección ip pon: "localhost"
	//Por lo visto esto tiene que ver al enumerar las
	//sesiones disponibles, prueba a no ponerlo a ver
	//si encuentra alguna sesion creada en el mismo ordenador
	//El puerto puedes pasar de él
	BOOL B_Iniciar_Conexion_TCPIP (char* c_direccion_ip = "", WORD w_puerto = 0);
    BOOL B_Iniciar_Conexion_IPX ();
	BOOL B_Cerrar_Conexion ();


/************    Control de las Sesiones     ************/
	//El guid de tu aplicación lo obtienes mediante el programita
	//aquel de quidgen creo que se llama que viene con el VisualStudio
	// o usa este:
	// {CB19C7C0-3294-11d4-8245-0040339F96BE}
	//static const GUID GUID_PRUEBA = 
	//{ 0xcb19c7c0, 0x3294, 0x11d4, { 0x82, 0x45, 0x0, 0x40, 0x33, 0x9f, 0x96, 0xbe } };
	//despues viene el nombre de la sesión y el número máximo de jugadores
	// que casualmente ya está puesto a 2
	BOOL B_Crear_Sesion (GUID guid_app, char* c_nombre_Sesion, DWORD d_max_jugadores = 2);

	//Enumera las sesiones
	//Esta función realiza su tarea en un segundo plano,
	//en modo asíncrono lo llaman, dicha tarea mantiene una
	//lista en una cache de las sesiones actuales
	//Cada vez que la llames se borrarán los datos de la lista interna
	//y se rellenarán de nuevo con los datos almacenados en la cache
	//de la tarea
	BOOL B_Enumerar_Sesiones (GUID guid_app);

	//Esta función es por si vas a hacer cosas raras ya que
	//DirectPlay para la tarea automáticamente cuando
	//creas o te unes a una sesión, o cuando cierras la 
	//conexión
	BOOL B_Parar_Enumeracion (GUID guid_app);

	//Pos eso cierra la sesión, y si tienes a un jugador
	//lo destruye
	//Si eres el jugador que creó la sesión enviará a los
	//demás jugadores el mensaje MSG_DESTRUYE_JUGADOR
	BOOL B_Cerrar_Sesion ();

	//Abre la sesión almacenada en la lista interna
	//desde la sesión 0 hasta la L_Tamano()-1
	BOOL B_Abrir_Sesion (DWORD n);


/************    Control externo de la lista     ************/
	//Devuelve el nombre de la sesion/jugador n
	char *L_Nombre (DWORD n);

	//Devuelve el Id del jugador n
	//Sólo usar cuando se enumeren jugadores
	DPID L_Id_Jugador (DWORD n);

	//Devuelve el tamaño de la lista
	DWORD L_Tamano ();


/************     Control de los jugadores     ************/
	//Crea un jugador nuevo en una sesión ya abierta,
	//o sea, abre la sesión y crea seguidamente el jugador
	//Enviará a los demás jugadores el mensaje MSG_CREA_JUGADOR
	BOOL B_Crear_Jugador (char* c_nombre_jugador);

	//Esta enumeración no es en modo asícrono
	//La llamas y guarda en la lista interna los nombres
	//de los jugadores y sus Id
	BOOL B_Enumerar_Jugadores ();

	//Destruye un jugador pero no cierra la sesión pudiendo
	//crear otro jugador nuevo
	//Envía el mensaje MSG_DESTRUYE_JUGADOR a los demás jugadores
	BOOL B_Destruir_Jugador ();


/************         Control de mensajes           ************/
	//Envía a id_to lo que haya en el puntero lp_data que tenga el
	//tamaño de dw_data_size
	//id_to puede ser igual a ID_TODOS con lo que se le enviará el
	//mensaje a todo el mundo
	//Le tengo puesto que todos los mensajes se manden de forma
	//GUARRANTEED con lo que supongo que siempre llegarán bien los
	//mensajes aunque tarden un poquito más
	//Cuando el mensaje ha sido confirmado de que ha sido entregado
	//se recibe el mensaje de MSG_MENSAJE_ENVIADO
	//Esta función también está en modo asíncrono
	BOOL B_Enviar (DPID id_to, LPVOID lp_data, DWORD dw_data_size);

	//El dword que devuelve esta función corresponde con los
	//diferentes tipos de mensajes arriba expuestos que se pueden
	//recibir
	//el lp_data puede ser un puntero a NULL y lpdw_data_size lo
	//que quieras y, después, estos valores serán cambiados según
	//el tipo de mensaje recibido
	DWORD DW_Recivir (LPVOID &lp_data, LPDWORD lpdw_data_size);
};