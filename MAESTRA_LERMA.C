
/* ******************************************************************************
  MEDICION Y CONTROL BOMBAS      MASTER PARA POZOS LERMA!

  VERSION   1  26/Oct/2018
  RUTA      C:/DESKTOP/SANTA CATARINA/SOFTWARE/GENERAL_1.C
  IDE       DYNAMIC C 10.72B

  PORTC............... HMI									GATE
  PORTE............... SCAIRLINK							ALTER
  PORTF............... 							 			CLON
Ver 1.0		PREPARACION
Ver 31     	revision
Ver 4.0		revisión Daniel
*/
#define PRINTFDEBUG printf
#define No_max_sitios 19
#define TIMEOUT_ESTADOWEB 180
unsigned char conta_error_conse_scairlink;

#define addr_reg_mb_HMI 5

#define dispo_local 20 - 1
#define No_Bbas_max 6
#define Numero_sitio 20
#define dir_scairlink 100 // LA ERA SCAIRLINK
#define No_max_errores_conta_scairlink 3

unsigned char conta_error_conse_scairlink;
unsigned char conta_error_scailink[60];

const unsigned char Nom_Proy[] = "MASTER - 33";
const unsigned char Version[] = "Version 33.0";
const unsigned char Fecha_Ver[] = "15/Dic/2020";

#define ver_sion 33.1

#define addr_reg_mb_version 2030
#define addr_reg_conta_sec_hmi 2026
#define addr_reg_mb_ctrl_hmi 2023
#define addr_reg_mb_ctrl_regio 2024

#define U_segundos 2010 // Tiempo para UTR's
#define U_minutos 2011
#define U_horas 2012
#define U_dia 2013
#define U_mes 2014
#define U_a�o 2015
#define U_set_tiempo 2016

#define W_segundos 2000 // Tiempo desde WEB
#define W_minutos 2001
#define W_horas 2002
#define W_dia 2003
#define W_mes 2004
#define W_a�o 2005
#define W_set_tiempo 2006

#define No_max_cmd_MB 30
// Resets Regionales Pozos
#define reset_slink 97
#define reset_alarma 98
#define reset_rabbit 99
/////////////////////////////////////////////////////////////////////////////////
//       DEFINICION CONFIGURACION HARDWARE
/////////////////////////////////////////////////////////////////////////////////

/*#define        MAX_4X_REGS                2400                                      // Holding Registerss 0-3 control the Analog Outputs 0-3, Holding Registers 4-7 are general purpose memory mapped registers
	unsigned my4XRegs[MAX_4X_REGS]; */
// Holding Registers 8-27 are used to display ascii characters on the last line of the LCD display
/////////////////////////////////////////////////////////////////////////////////
//       DEFINICION CONFIGURACION TABLA        H M I
/////////////////////////////////////////////////////////////////////////////////

unsigned int conta_secuen_hmi;
unsigned int cmd_control_hmi;
unsigned int cmd_control_regional;

typedef struct
{
	unsigned int incio_tab_hmi;
	unsigned int cantidad_hmi;
	unsigned int enlace_hmi;
	unsigned int tiempo_hmi;

	unsigned int incio_tab_scair;
	unsigned int cantidad_scair;
	unsigned int enlace_scair;
} extructura;
extructura tabla[60];
///////////////////////////////////////////////////////////////////////////////
// CONFIGURACION             M O D B U S   G E N E R A L
///////////////////////////////////////////////////////////////////////////////

#define bueno 1
#define malo 0

struct
{
	unsigned char conta_set_ala;
	unsigned char conta_clr_ala;
	unsigned char flag_ala;
	unsigned char flag_ala_insta;
	unsigned long conta_exito;
	unsigned long conta_errores;
	unsigned char tiempo_max_alar;
	unsigned char flag_local;
} status_com[6];

int *apunta_tiempo;
int tabla_modbus[100];

unsigned int flag_ejecutar;
unsigned char ejecuta_mando;
unsigned char No_salida;

#define tiempo_max_falla 8
#define tiempo_max_final 8

unsigned long finaliza_tiempo;

typedef struct
{
	unsigned long com_ok;
	unsigned long com_bad;
	unsigned char enlace;
	unsigned long fecha;
} es_com;

static bbram es_com esta_com[No_max_cmd_MB];

unsigned long tiempo_inicio;
unsigned long tiempo_final;
unsigned long ventana_tiempo;
/*******************************************************************************
   DEFINICION CAPACIDAD PUERTOS SERIALES
*******************************************************************************/

#define CINBUFSIZE 255
#define COUTBUFSIZE 255
#define EINBUFSIZE 255
#define EOUTBUFSIZE 255
#define FINBUFSIZE 255
#define FOUTBUFSIZE 255

typedef struct
{
	unsigned int dir_utr;
	unsigned char funcion;
	unsigned int dir_fuente;
	unsigned int dir_destino;
	unsigned char No_reg;
	unsigned char puerto;
} conf_utr_int;

conf_utr_int confi_utr_int[No_max_cmd_MB];
/**************************************************************************/
//  Set a default of declaring all local variables "auto" (on stack)
#class auto

// include BLxS2xx series library
#use "BLxS2xx.lib"
/*******************************************************************************
   CONFIGURACION DE LA RED ETHERNET
*******************************************************************************/

static bbram char new_ip[25];
static bbram char new_nm[25];
static bbram char new_gw[25];
static bbram char new_ns[25];

static bbram unsigned long Contador_Inte;

#define TCPCONFIG 101
#define PORT 502		 // TCP port to use for MODBUS/TCP processing
#define TCP_BUF_SIZE 512 // TCP buffer size, max MODBUS/TCP packet size is 256 bytes
						 /*******************************************************************************
   DEFINICION SOCKET ETHERNET
*******************************************************************************/

#use "dcrtcp.lib" // Load TCP/IP Library
#use MTCPS.LIB	  // Load MODBUS TCP Library

unsigned char enlace_global_1;
unsigned char enlace_global_2;
unsigned char enlace_global_3;
unsigned char enlace_global_4;
unsigned char enlace_global_5;
/*******************************************************************************
   CONFIGURACION             CARACTERISTICAS I/0 MODBUS TCP/IP
*******************************************************************************/

#define MY_SLAVE_ADDR 1 // MODBUS Slave Address

#define MAX_0X_COILS 16 // Coils 0-15 control digital outputs 0-15, Coils 16-22 control the LEDS DS1-DS7, Coil 23 controls LCD backlight
unsigned my0XCoils[(MAX_0X_COILS / 16) + 1];
#define MAX_1X_DISCRETES 16 // Discretes 0-23 report the state of digital inputs 0-23
unsigned my1XDiscretes[(MAX_1X_DISCRETES / 16) + 1];
#define MAX_3X_INREGS 60 // Input Registers binary
unsigned my3XInRegs[MAX_3X_INREGS];
#define MAX_4X_REGS 3200		// Holding Registerss 0-3 control the Analog Outputs 0-3, Holding Registers 4-7 are general purpose memory mapped registers
unsigned my4XRegs[MAX_4X_REGS]; // Holding Registers 8-27 are used to display ascii characters on the last line of the LCD display

#define REG_AMOUNT MAX_4X_REGS
/*******************************************************************************
   CONFIGURACION DE VARIABLES GENERALES
*******************************************************************************/
union
{ // UNION conversion de tipo datos
	float conver_float;
	unsigned long conver_long;
	unsigned int conver_int[2];
	unsigned char conver_byte[4];
} conver;

const unsigned int binario[16] = {{0x0001}, {0x0002}, {0x0004}, {0x0008}, {0x0010}, {0x0020}, {0x0040}, {0x0080}, {0x0100}, {0x0200}, {0x0400}, {0x0800}, {0x1000}, {0x2000}, {0x4000}, {0x8000}};
const unsigned int bin[8] = {{0x01}, {0x02}, {0x04}, {0x08}, {0x10}, {0x20}, {0x40}, {0x80}};
/*******************************************************************************
   DEFINICION FUNCIONES PROTOTIPO
*******************************************************************************/

/*===================================================================
   WAIT_SERIAL
===================================================================*/
void wait_485(long sd)
{
	unsigned long t1;
	t1 = MS_TIMER;
	while (t1 + sd > MS_TIMER)
	{
	}
}
//============================================================================*/
//  PUERTO    D E F I N I C I O N E S       G E N E R A L E S        D R I V E R       M O D B U S      *****   DATA LINK    *****   PUERTO C
//  PARA UTR'S INALAMBRICAS
//============================================================================*/

#define MM_BRTO_gate 800 // allow long (500 ms) query/response delay 500 val ant
#define MM_ERTO_gate 10	 // set short (1 mS) inter-character delay 200 val ant
#use "MMZ_rab_gate.LIB"
#define BCAST_ADDR_gate 0		  // broadcast address
#define MODBUS_BAUDRATE_gate 9600 // Defines the Baudrate to use
#define MODBUS_PROTOCOL_gate 1	  // Defines the Protocol.  0 = ASCII, 1 = RTU
#define MODBUS_SERIAL_gate 1	  // Defines the Serial port type. 0 = RS485, 1 = RS232                          \
								  //============================================================================ \
								  //         MODBUS FUNCTION          MASTER   MODBUS                            \
								  //============================================================================

nodebug void mmCrx_gate(void)
{
	// Make sure all of the data has been sent by;
	// 1.) checking the write buffer for any bytes left
	// 2.) checking the status of the Write interrupt transmit bit (2).
	// 3.) checking the status of the Write interrupt data bit (3)
	while (serCwrUsed() || BitRdPortI(SCSR, 2) || BitRdPortI(SCSR, 3))
		;
	// turn off the transmitter
	// Since we echo what we send, flush the read buffer, so that you are
	// ready for the next packet.

	wait_485(2); // 10 mseg
	serCrdFlush();
	ser485Rx();
}

nodebug void mmCtx_gate()
{
	ser485Tx();
}

nodebug int mmCopen_gate(unsigned long qBaud)
{
	if (serCopen(qBaud))
	{
		serCdatabits(PARAM_8BIT);
		return 1;
	}
	return 0;
}
/*============================================================================*/
//  PUERTO    D E F I N I C I O N E S       G E N E R A L E S        D R I V E R       M O D B U S      *****   DATA LINK    *****   PUERTO E
//  PARA UTR'S INALAMBRICAS
/*============================================================================*/

#define MM_BRTO_alter 700 // allow long (500 ms) query/response delay 500 val ant
#define MM_ERTO_alter 10  // set short (1 mS) inter-character delay 200 val ant
#use "MMZ_rab_alter.LIB"
#define BCAST_ADDR_alter 0		   // broadcast address
#define MODBUS_BAUDRATE_alter 9600 // Defines the Baudrate to use
#define MODBUS_PROTOCOL_alter 1	   // Defines the Protocol.  0 = ASCII, 1 = RTU
#define MODBUS_SERIAL_alter 1	   // Defines the Serial port type. 0 = RS485, 1 = RS232
								   /*============================================================================*\
         MODBUS FUNCTION          MASTER   MODBUS
\*============================================================================*/

nodebug void mmErx_alter(void)
{
	// Make sure all of the data has been sent by;
	// 1.) checking the write buffer for any bytes left
	// 2.) checking the status of the Write interrupt transmit bit (2).
	// 3.) checking the status of the Write interrupt data bit (3)
	//while (serCwrUsed() || BitRdPortI(SCSR,2) || BitRdPortI(SCSR,3));
	// turn off the transmitter
	// Since we echo what we send, flush the read buffer, so that you are
	// ready for the next packet.

	//wait_485(2);                                                             // 10 mseg
	//serCrdFlush();
	//ser485Rx();
}

nodebug void mmEtx_alter()
{
	//ser485Tx();
}

nodebug int mmEopen(unsigned long qBaud)
{
	if (serEopen(qBaud))
	{
		serEdatabits(PARAM_8BIT);
		return 1;
	}
	return 0;
}
//============================================================================
//  PUERTO    D E F I N I C I O N E S       G E N E R A L E S        D R I V E R       M O D B U S      *****   DATA LINK    *****   PUERTO F
//  PARA UTR'S INALAMBRICAS
//============================================================================

#define MM_BRTO_clon 1000 // allow long (500 ms) query/response delay 500 val ant
#define MM_ERTO_clon 10	  // set short (1 mS) inter-character delay 200 val ant
#use "MMZ_rab_clon.LIB"
#define BCAST_ADDR_clon 0		  // broadcast address
#define MODBUS_BAUDRATE_clon 9600 // Defines the Baudrate to use
#define MODBUS_PROTOCOL_clon 1	  // Defines the Protocol.  0 = ASCII, 1 = RTU
#define MODBUS_SERIAL_clon 1	  // Defines the Serial port type. 0 = RS485, 1 = RS232                          \
								  //============================================================================ \
								  //         MODBUS FUNCTION          MASTER   MODBUS                            \
								  //============================================================================

nodebug void mmFrx_clon(void)
{
	// Make sure all of the data has been sent by;
	// 1.) checking the write buffer for any bytes left
	// 2.) checking the status of the Write interrupt transmit bit (2).
	// 3.) checking the status of the Write interrupt data bit (3)
	//while (serCwrUsed() || BitRdPortI(SCSR,2) || BitRdPortI(SCSR,3));
	// turn off the transmitter
	// Since we echo what we send, flush the read buffer, so that you are
	// ready for the next packet.

	//wait_485(2);                                                             // 10 mseg
	//serCrdFlush();
	//ser485Rx();
}

nodebug void mmFtx_clon()
{
	//ser485Tx();
}

nodebug int mmFopen(unsigned long qBaud)
{
	if (serFopen(qBaud))
	{
		serFdatabits(PARAM_8BIT);
		return 1;
	}
	return 0;
}
//****************************************************
//    Manda pulso digital con un ancho de 1 seg.
//****************************************************
cofunc COF_Genera_Pulso_Digi(unsigned int No_Salida)
{
	digOut(No_Salida, 0);
	waitfor(DelayMs(1000));
	digOut(No_Salida, 1);
}
/*===================================================================
   FUNCION ALMACENAR DATOS DE TIEMPO EN TABLA ( my4XRegs )
===================================================================*/
void Almacena_Datos_Tiempo(int *apunta_t)
{
	struct tm rtc;
	mktm(&rtc, read_rtc());

	*apunta_t = rtc.tm_sec;
	apunta_t++;

	*apunta_t = rtc.tm_min;
	apunta_t++;

	*apunta_t = rtc.tm_hour;
	apunta_t++;

	*apunta_t = rtc.tm_mday;
	apunta_t++;

	*apunta_t = rtc.tm_mon;
	apunta_t++;

	*apunta_t = rtc.tm_year + 1900;
}
/*******************************************************************************
   FUNCION QUE HACE LLEVA LA ESTADISTICA DE COMUNICACIONES DE LOS ARRANCADORES BENSHAW
*******************************************************************************/

void Supervi_Comm(unsigned char puerto, unsigned char good)
{
	if (good == 0x01)
	{ // COMUNICACION EXITOSA
		status_com[puerto].flag_ala_insta = 0x00;
		status_com[puerto].conta_exito++;
		status_com[puerto].conta_set_ala = 0x00;

		if (status_com[puerto].flag_ala == 0x01)
		{
			if (status_com[puerto].conta_clr_ala < status_com[puerto].tiempo_max_alar)
			{
				status_com[puerto].conta_clr_ala++;
				if (status_com[puerto].conta_clr_ala == status_com[puerto].tiempo_max_alar)
				{
					status_com[puerto].flag_ala = 0x00;
					//tipo_reportes=11+puerto;                                                     // Reporte RESTABLECE ALARMA COMUNICACIONES MED 1 y 2
					//Alma_Reportes();
					//dig_Out(alarma_sirena,1);
					//dig_Out(alarma_foco,0);
				}
			}
		}
	}
	else
	{
		status_com[puerto].flag_ala_insta = 0x01;
		status_com[puerto].conta_errores++;
		status_com[puerto].conta_clr_ala = 0x00;

		if (status_com[puerto].flag_ala == 0x00)
		{
			if (status_com[puerto].conta_set_ala < status_com[puerto].tiempo_max_alar)
			{
				status_com[puerto].conta_set_ala++;
				if (status_com[puerto].conta_set_ala == status_com[puerto].tiempo_max_alar)
				{
					status_com[puerto].flag_ala = 0x01;
					//tipo_reportes=14+puerto;
					//Alma_Reportes();
					//dig_Out(alarma_sirena,1);
					//dig_Out(alarma_foco,1);
				}
			}
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////        R E A D     D A T A       S C A I R L I N K       DISPOSITIVO ( No_dispo )
////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int enlaceweb_cont[20];
unsigned char enlaceweb_estado[20];
unsigned long enlaceweb_timerweb[20];
unsigned char flag_inicio;

#define MUTEX_LIBRE 1
#define MUTEX_OCUPADO 0
#define ULTIMA_DIRECCION_HMI 645

unsigned char mutex_mensajes_scarling = MUTEX_LIBRE;
unsigned int contador_costate_hmi_utr = 0;

unsigned int
calcula_estado_enlaceweb(unsigned int num)
{
	if (enlaceweb_cont[num] == my4XRegs[tabla[num].enlace_hmi + 2300])
	{ // no ha camabiado el contador
		if (read_rtc() - enlaceweb_timerweb[num] > TIMEOUT_ESTADOWEB || flag_inicio == 1)
		{ // ya pas� el timeout
			flag_inicio = 0;
			enlaceweb_cont[num] = my4XRegs[tabla[num].enlace_hmi + 2300];
			enlaceweb_timerweb[num] = read_rtc();
			enlaceweb_estado[num] = 0x00; //el estado web se pone en 0 SOLO si no cambio el contador web y ya paso el timeout
			return 0;
		}
		else
		{
			return 0;
		}
	}
	else
	{ // ya cambio el contador
		enlaceweb_cont[num] = my4XRegs[tabla[num].enlace_hmi + 2300];
		enlaceweb_timerweb[num] = read_rtc();
		enlaceweb_estado[num] = 0x01; // el estado web se pone en 1 SOLO si cambio el contador
		return 1;
	}
}

cofunc void COF_captura_datos_scairlink(unsigned int No_dispo)
{
	int error;
	int nada;
	PRINTFDEBUG("\nINFUNC: %u,", No_dispo);
	waitfor(DelayMs(100));
	//               it  = mmRead_alter(  unsigned wAddr,                    unsigned wReg,                unsigned wCount,                                     void *pwRegs)
	while (mutex_mensajes_scarling == MUTEX_OCUPADO0)
	{
		yield;
	}
	mutex_mensajes_scarling = MUTEX_OCUPADO;
	waitfor(error = mmRead_alter(dir_scairlink, tabla[No_dispo].incio_tab_hmi, tabla[No_dispo].cantidad_hmi, (int *)&my4XRegs[tabla[No_dispo].incio_tab_hmi])); // Read registros CHALMITA
	mutex_mensajes_scarling = MUTEX_LIBRE;
	if (error == 0xffff)
	{
		//memcpy((char*)&my4XRegs[tabla[No_dispo].incio_tab_hmi],(char*)&my4XRegs[tabla[No_dispo].incio_tab_scair], tabla[No_dispo].cantidad_hmi*2);      // Copia valores       ( CHALMITA )
		//my4XRegs[tabla[No_dispo].enlace_hmi] = my4XRegs[tabla[No_dispo].enlace_scair];                                // enlace radio CHALMITA
		//nada = 0x00;
		conta_error_scailink[No_dispo] = 0x00;
		if (my4XRegs[tabla[No_dispo].enlace_hmi] == 0x01)
		{ // si hay enlace Rf fecha se obtiente con almacena datos tiempo
			apunta_tiempo = (int *)&my4XRegs[tabla[No_dispo].tiempo_hmi];
			Almacena_Datos_Tiempo(apunta_tiempo);
			calcula_estado_enlaceweb(No_dispo);

			if (enlaceweb_estado[No_dispo] == 0x01)
			{ //rf ok + web ok
				my4XRegs[tabla[No_dispo].enlace_hmi] = 0x03;
			}
			else
			{ // rf ok + web mal
				my4XRegs[tabla[No_dispo].enlace_hmi] = 0x01;
			}
		}
		else
		{ //rf mal
			calcula_estado_enlaceweb(No_dispo);
			PRINTFDEBUG("\n enlaceweb[%d]=%d", No_dispo, enlaceweb_estado[No_dispo]);
			if (enlaceweb_estado[No_dispo] == 0x01)
			{ //rf mal + web ok
				apunta_tiempo = (int *)&my4XRegs[tabla[No_dispo].tiempo_hmi];
				Almacena_Datos_Tiempo(apunta_tiempo);
				memcpy(&my4XRegs[tabla[No_dispo].incio_tab_hmi], &my4XRegs[2300 + tabla[No_dispo].incio_tab_hmi], 2 * tabla[No_dispo].cantidad_hmi + 12);
				my4XRegs[tabla[No_dispo].enlace_hmi] = 0x02;
			}
			else
			{ // rf mal + web mal
				my4XRegs[tabla[No_dispo].enlace_hmi] = 0x00;
			}
		}
	}
	else
	{
		PRINTFDEBUG("\nINFUNC code: %u,", error);
		conta_error_scailink[No_dispo] = conta_error_scailink[No_dispo] + 1;
		if (conta_error_scailink[No_dispo] >= No_max_errores_conta_scairlink)
		{
			conta_error_scailink[No_dispo] = No_max_errores_conta_scairlink;
			my4XRegs[tabla[No_dispo].enlace_hmi] = 0x00;
			PRINTFDEBUG("Error RD Scair Dispo=%u\n", No_dispo);
		}
		calcula_estado_enlaceweb(No_dispo);
		if (enlaceweb_estado[No_dispo] == 0x01)
		{ //rf mal + web ok
			apunta_tiempo = (int *)&my4XRegs[tabla[No_dispo].tiempo_hmi];
			Almacena_Datos_Tiempo(apunta_tiempo);
			memcpy(&my4XRegs[tabla[No_dispo].incio_tab_hmi], &my4XRegs[2300 + tabla[No_dispo].incio_tab_hmi], 2 * tabla[No_dispo].cantidad_hmi + 12);
			my4XRegs[tabla[No_dispo].enlace_hmi] = 0x02;
		}
		else
		{ // rf mal + web mal
			my4XRegs[tabla[No_dispo].enlace_hmi] = 0x00;
		}
	}
}
//****************************************************
//    Lee ESTADO PERILLAS     AUTO, MANUAL  OFF
//****************************************************
unsigned int read_status_Perilla(unsigned int in_dere, unsigned int in_izqui)
{
	int Ent_loc;
	int Ent_remo;

	Ent_loc = digIn(in_dere); // STATUS PERILLA
	Ent_remo = digIn(in_izqui);

	if ((Ent_loc == 1) && (Ent_remo == 1))
	{
		return 0; // OFF
	}
	else
	{
		if (Ent_loc == 0) // MANUAL
		{
			return 1; // REMOTO
		}
		else
		{
			return 2; // AUTO   LOCAL
		}
	}
}
//****************************************************
//    Lee ESTADO PERILLAS     AUTO, MANUAL  OFF
//****************************************************
unsigned int read_status_Bomba(unsigned int in_estado, unsigned int in_falla)
{
	if (digIn(in_falla) == 0)
	{
		return 3; // OFF
	}
	else
	{
		if (digIn(in_estado) == 0) // MANUAL
		{
			return 1; // Arrancada
		}
		else
		{
			return 2; // Parada
		}
	}
}
//------------------------------------------------------------------------------
//
// SECCION CODIGO DE LA LIBRERIA  MTCPS.LIB    LECTURA DE REGISTROS HOLDING  FUNCION 03
//
//------------------------------------------------------------------------------

int msRead(unsigned wReg, unsigned *pwValue)
{
	if ((wReg < 0) || (wReg > i4XRegs - 1))
		return MS_BADADDR;

	*pwValue = *(p4XRegs + wReg);
	return 0;
}
/*===================================================================
=====================================================================
=====================================================================

            PROGRAMA      PRINCIPAL

=====================================================================
=====================================================================
===================================================================*/

main()
{
	struct tm rtc;
	int tempo_int;
	int channel;
	unsigned long new_seg;
	unsigned char conta_seg;
	unsigned char i, j;
	int error, z, ii, zz, tt;
	unsigned int jj;
	unsigned long t1;
	unsigned int new_hora;
	unsigned int alma_tempo_int;

	unsigned int input_local;
	unsigned int input_remoto;
	unsigned int cmd_control;

	unsigned char flag_inicio_version;

	unsigned char flag_inicio_control_A; // flag Control HMI
	unsigned char flag_inicio_control_B; // flag Control Scairlink

	unsigned char nada;
	unsigned int No_sitio;
	unsigned int Bomba_No;

	int error_mb;
	int buffer_plc[20];
	int buffer_plc_aux;

	unsigned long dummy_long;
	unsigned int No_dispo;

	unsigned int conta_loop;
	/*===================================================================
	   Inicializacion del MODULO  BL4S200
	===================================================================*/

	brdInit(); // Required for controllers
	serMode(0);
	/*===================================================================
	   Inicializacion Salidas Digitales
	===================================================================*/

	for (channel = 0; channel < 8; ++channel) // Set output to be general digital output
	{
		setDigOut(channel, 1); // Set output to be off                                                                              //channels[channel] = 1
	}
	/*===================================================================
	   Inicializacion Entradas Digitales
	===================================================================*/

	for (channel = 8; channel < 32; ++channel) // Set configurable I/O 0-15 to be general digital inputs
	{
		setDigIn(channel);
	}
	/*===================================================================
	   Inicializacion de puertos seriales
	===================================================================*/
	mmrCinit_gate(9600);  // Velocidad puerto Master hacia HMI + SCAIRLINK
	mmrEinit_alter(9600); // Velocidad puerto Master hacia HMI + SCAIRLINK
	mmrFinit_clon(9600);  // Velocidad puerto Master hacia HMI + SCAIRLINK
						  /*===================================================================
	   Inicializacion de extructuras, variables etc.
	===================================================================*/
	memset(tabla_modbus, 0x00, sizeof(tabla_modbus));
	memset(my4XRegs, 0x00, sizeof(my4XRegs));
	//------------------------------------------------------------------------------
	//          INICIALIZACION DE VARIABLES
	//------------------------------------------------------------------------------
	conta_seg = 0;
	new_seg = read_rtc();
	// sprintf(new_ip, "192.168.1.49");
	// sprintf(new_nm, "255.255.255.0");
	// sprintf(new_gw, "192.168.1.50");
	// sprintf(new_ns, "192.168.1.81");
	sprintf(new_ip, "10.10.10.6");
	sprintf(new_nm, "255.255.0.0");
	sprintf(new_gw, "10.10.10.1");
	sprintf(new_ns, "10.10.10.72");
	/*******************************************************************************
	   SECCION INICIALIZACION SOCKET TCP/IP
	*******************************************************************************/
	sock_init();
	ifconfig(IF_ETH0, IFS_DOWN, IFS_END); // bring down interface
	tcp_tick(NULL);
	while (ifpending(IF_ETH0) % 2) // waitfor interface to close
		tcp_tick(NULL);

	ifconfig(IF_ETH0,
			 IFS_DOWN,
			 IFS_IPADDR, aton(new_ip),
			 IFS_NETMASK, aton(new_nm),
			 IFS_ROUTER_SET, aton(new_gw),	   // delete all GW and add this one
			 IFS_NAMESERVER_SET, aton(new_ns), // delete all NS and add this one
			 IFS_UP,
			 IFS_END);
	/*******************************************************************************
	   SECCION INICIALIZACION DE ENTRADAS BINARIAS Y REGISTROS MODBUS
	*******************************************************************************/

	//    Initialize local MODBUS Registers and Coils
	//    This is the user defined data memory which is used to pass data back and forth with the MODBUS/TCP library routines
	//    The users program should define Coils, Discretes, Input Registers, and Holding Registers arrays large enough to
	//    support the MODBUS/TCP references required for the application and initialize their contents.

	memset(&my3XInRegs, 0x00, sizeof(my3XInRegs));
	memset(&my4XRegs, 0x00, sizeof(my4XRegs));

	// Intialize MODBUS TCP protocol Engine by passing pointers and array sizes to the library function mstInit()

	mstInit(MY_SLAVE_ADDR, my0XCoils, MAX_0X_COILS,
			my1XDiscretes, MAX_1X_DISCRETES,
			my3XInRegs, MAX_3X_INREGS,
			my4XRegs, MAX_4X_REGS);

	//    Cooperative multitasking example using costates
	//    This cooperative multitasking examples illustrates the use of costates to perform various tasks
	//    Tasks are included to handle keypad I/O, update and scan digital and analog I/O, refresh the LEDS, and refresh the LCD display
	//    The msRun() MODBUS/TCP Library function is included as a seperate costate task, this allows the handling of all
	//    MODBUS/TCP related tasks to be cleanly seperated from the balance of the application functionality
	/*******************************************************************************
	   SECCION INICIALIZACION DATOS Y COMANDOS A ENVIAR PARA LA ADQ MODBUS TCP/IP
	*******************************************************************************/
	wMSAddr = 1;
	//error_MB_C = 0;
	//error_MB_E = 0;
	//error_MB_F = 0;
	/*===================================================================
	   Inicializa Driver micro SD
	===================================================================*/

	//confi_default();                                                           // Alamacena configura default
	/*===================================================================
	   Recupera datos   CONFIGURACION MAESTRA  del micro SD
	===================================================================*/
	/*
	   memset(confi_utr_int  , 0x00    , sizeof(confi_utr_int));

	   read_micro_SD(No_sector_datos_CONF, buf_tempo_microSD, sizeof(buf_tempo_microSD));        // Load NAMES_ANALOGIC
	   memcpy((char*)confi_utr_int,buf_tempo_microSD, sizeof(confi_utr_int));
	 */

	/*
	   for(i=0; i<No_max_cmd_MB; i++)
	   {
	      PRINTFDEBUG("%u %3u %4u %4u %4u %u\n",confi_utr_int[i].dir_utr, confi_utr_int[i].funcion, confi_utr_int[i].dir_fuente,confi_utr_int[i].dir_destino,confi_utr_int[i].No_reg,confi_utr_int[i].puerto);
	   }
	   */
	/***************************************************************************************************************
								Tabla maestro
  ***************************************************************************************************************/
	mktm(&rtc, read_rtc());
	PRINTFDEBUG("Fecha Inicio -> %u/%u/%u %u:%u:%u\n\r", rtc.tm_year + 1900, rtc.tm_mon, rtc.tm_mday, rtc.tm_hour, rtc.tm_min, rtc.tm_sec);

	finaliza_tiempo = read_rtc() + tiempo_max_final;

	conver.conver_float = ver_sion; // Almacena el numero de version
	my4XRegs[addr_reg_mb_version] = conver.conver_int[0];
	my4XRegs[addr_reg_mb_version + 1] = conver.conver_int[1];

	my4XRegs[W_set_tiempo] = 0x00; // Flag de SET TIME MODBUS  = 0 Desabilitado WEB
	my4XRegs[U_set_tiempo] = 0x00; // Flag de SET TIME MODBUS  = 0 Desabilitado UTR

	flag_inicio_version = 1;
	flag_inicio_control_A = 1;
	flag_inicio_control_B = 1;

	cmd_control_hmi = 0x00;
	cmd_control = 0x00;
	cmd_control_regional = 0x00;

	//    LERMA NORTE 1
	tabla[0].incio_tab_hmi = 100;
	tabla[0].cantidad_hmi = 23;
	tabla[0].enlace_hmi = 122;
	tabla[0].tiempo_hmi = 123;

	//    LERMA NORTE 1B
	tabla[1].incio_tab_hmi = 129;
	tabla[1].cantidad_hmi = 23;
	tabla[1].enlace_hmi = 151;
	tabla[1].tiempo_hmi = 152;

	//    LERMA NORTE 2
	tabla[2].incio_tab_hmi = 158;
	tabla[2].cantidad_hmi = 23;
	tabla[2].enlace_hmi = 180;
	tabla[2].tiempo_hmi = 181;

	//    LERMA NORTE 3
	tabla[3].incio_tab_hmi = 187;
	tabla[3].cantidad_hmi = 23;
	tabla[3].enlace_hmi = 209;
	tabla[3].tiempo_hmi = 210;

	//    LERMA NORTE 4
	tabla[4].incio_tab_hmi = 216;
	tabla[4].cantidad_hmi = 23;
	tabla[4].enlace_hmi = 238;
	tabla[4].tiempo_hmi = 239;

	//    LERMA NORTE 5
	tabla[5].incio_tab_hmi = 245;
	tabla[5].cantidad_hmi = 23;
	tabla[5].enlace_hmi = 267;
	tabla[5].tiempo_hmi = 268;

	//    LERMA NORTE 6
	tabla[6].incio_tab_hmi = 274;
	tabla[6].cantidad_hmi = 23;
	tabla[6].enlace_hmi = 296;
	tabla[6].tiempo_hmi = 297;

	//    LERMA NORTE 7
	tabla[7].incio_tab_hmi = 303;
	tabla[7].cantidad_hmi = 23;
	tabla[7].enlace_hmi = 325;
	tabla[7].tiempo_hmi = 326;

	//    LERMA NORTE 8B antes 25
	tabla[8].incio_tab_hmi = 332;
	tabla[8].cantidad_hmi = 23;
	tabla[8].enlace_hmi = 354;
	tabla[8].tiempo_hmi = 356;

	//    LERMA NORTE 9
	tabla[9].incio_tab_hmi = 361;
	tabla[9].cantidad_hmi = 23;
	tabla[9].enlace_hmi = 383;
	tabla[9].tiempo_hmi = 384;

	//    LERMA NORTE 9B
	tabla[10].incio_tab_hmi = 390;
	tabla[10].cantidad_hmi = 23;
	tabla[10].enlace_hmi = 412;
	tabla[10].tiempo_hmi = 413;

	//    LERMA NORTE 10
	tabla[11].incio_tab_hmi = 419;
	tabla[11].cantidad_hmi = 23;
	tabla[11].enlace_hmi = 441;
	tabla[11].tiempo_hmi = 442;

	//    LERMA NORTE 10B
	tabla[12].incio_tab_hmi = 448;
	tabla[12].cantidad_hmi = 23;
	tabla[12].enlace_hmi = 470;
	tabla[12].tiempo_hmi = 471;

	//    LERMA NORTE 14
	tabla[13].incio_tab_hmi = 477;
	tabla[13].cantidad_hmi = 23;
	tabla[13].enlace_hmi = 499;
	tabla[13].tiempo_hmi = 500;

	//    LERMA NORTE 32
	tabla[14].incio_tab_hmi = 506;
	tabla[14].cantidad_hmi = 23;
	tabla[14].enlace_hmi = 528;
	tabla[14].tiempo_hmi = 529;

	//    LERMA NORTE 34
	tabla[15].incio_tab_hmi = 535;
	tabla[15].cantidad_hmi = 23;
	tabla[15].enlace_hmi = 557;
	tabla[15].tiempo_hmi = 558;

	//    LERMA NORTE 35
	tabla[16].incio_tab_hmi = 564;
	tabla[16].cantidad_hmi = 23;
	tabla[16].enlace_hmi = 586;
	tabla[16].tiempo_hmi = 587;

	//    LERMA NORTE 36
	tabla[17].incio_tab_hmi = 593;
	tabla[17].cantidad_hmi = 23;
	tabla[17].enlace_hmi = 615;
	tabla[17].tiempo_hmi = 616;

	//    LERMA NORTE 64A repetidor
	tabla[18].incio_tab_hmi = 622;
	tabla[18].cantidad_hmi = 6;
	tabla[18].enlace_hmi = 627;
	tabla[18].tiempo_hmi = 628;

	//    LOCAL regional
	tabla[19].incio_tab_hmi = 634;
	tabla[19].cantidad_hmi = 6;
	tabla[19].enlace_hmi = 639;
	tabla[19].tiempo_hmi = 640;

	//  Pone el tiempo en Tabla Modbus  ( my4XRegs ) del HMI
	for (i = 0; i < No_max_sitios; i++)
	{
		if (my4XRegs[tabla[i].enlace_hmi] != 0x02)
		{
			apunta_tiempo = (int *)&my4XRegs[tabla[i].tiempo_hmi]; // A todos los sitios de Padierna
			Almacena_Datos_Tiempo(apunta_tiempo);
		}
	}

	flag_inicio_control_A = 0x01;
	flag_inicio_control_B = 0x01;

	conta_error_conse_scairlink = 0;
	memset(conta_error_scailink, 0x00, sizeof(conta_error_scailink));

	conta_loop = 0;

	enlaceweb_timerweb[0] = read_rtc();
	for (i = 1; i < 20; i++)
	{
		enlaceweb_timerweb[i] = enlaceweb_timerweb[0];
	}
	memset(&enlaceweb_estado[0], 0x00, 20);
	for (i = 0; i < 20; i++)
	{
		my4XRegs[tabla[i].enlace_hmi + 2300] = 0;
		enlaceweb_cont[i] = 0;
	}
	flag_inicio = 1;

	while (1)
	{

		//-------------------------------------------------------------------------------------------------------------------------------------
		//----------  R U T I N A    M O D B U S    T C P / I P    ----------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------------------------------------
		costate
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////
			////       R E A D       D A T A       U T R ' S
			///////////////////////////////////////////////////////////////////////////////////////////////////

			PRINTFDEBUG("RD slaves\n");
			for (contador_costate_hmi_utr = 0; contador_costate_hmi_utr < No_max_sitios; contador_costate_hmi_utr++)
			{
				if (contador_costate_hmi_utr != dispo_local)
				{
					PRINTFDEBUG("RD slave [%d]\n", contador_costate_hmi_utr);
					wfd COF_captura_datos_scairlink(contador_costate_hmi_utr);
				}
				yield;
			}
			PRINTFDEBUG("ENd rd Slaves\n");
			///////////////////////////////////////////////////////////////////////////////////////////////////
			////       W R I T E        D A T A        H M I
			///////////////////////////////////////////////////////////////////////////////////////////////////
			for (contador_costate_hmi_utr = 100; contador_costate_hmi_utr < ULTIMA_DIRECCION_HMI; contador_costate_hmi_utr += 100)
			{
				waitfor(DelayMs(100));
				if (contador_costate_hmi_utr - ULTIMA_DIRECCION_HMI >= 100)
					waitfor(error = mmPresetRegs_gate(addr_reg_mb_HMI, contador_costate_hmi_utr, 100, (int *)&my4XRegs[contador_costate_hmi_utr]));
				else
					waitfor(error = mmPresetRegs_gate(addr_reg_mb_HMI, contador_costate_hmi_utr, ULTIMA_DIRECCION_HMI % 100, (int *)&my4XRegs[contador_costate_hmi_utr]));
				if (error == 0xffff)
				{
					nada = 0x00;
				}
				else
				{
					PRINTFDEBUG("Error H3\n");
					nada = 0x00;
				}
				yield;
			}
		}
		costate
		{ // Process MODBUS TCP activity
			msRun();
		}

		costate
		{
			if (new_seg != read_rtc())
			{ // New Second ?
				new_seg = read_rtc();
				////////////////////////////////////////////////////////////////////////////////////////////////////////////
				conta_seg++;
				mktm(&rtc, read_rtc());

				if (my4XRegs[W_set_tiempo] == 0x01)
				{									   // Si flag de set_tiempo == 0x01 actualiza tiempo reloj
					rtc.tm_sec = my4XRegs[W_segundos]; // change the time
					rtc.tm_min = my4XRegs[W_minutos];
					rtc.tm_hour = my4XRegs[W_horas];
					//tm_setMDY( &rtc, 12, 31, 1999);
					tm_setMDY(&rtc, my4XRegs[W_mes], my4XRegs[W_dia], my4XRegs[W_a�o]);
					tm_wr(&rtc); // set clock
					t1 = mktime(&rtc);
					PRINTFDEBUG("Setting date/time to ");
					PRINTFDEBUG("Fecha -> %u/%u/%u %u:%u:%u\n\r", rtc.tm_year + 1900, rtc.tm_mon, rtc.tm_mday, rtc.tm_hour, rtc.tm_min, rtc.tm_sec);
					my4XRegs[W_set_tiempo] = 0x00;
				}

				if (new_hora != rtc.tm_hour)
				{
					my4XRegs[U_segundos] = rtc.tm_sec;
					my4XRegs[U_minutos] = rtc.tm_min;
					my4XRegs[U_horas] = rtc.tm_hour;

					my4XRegs[U_dia] = rtc.tm_mday;
					my4XRegs[U_mes] = rtc.tm_mon;
					my4XRegs[U_a�o] = rtc.tm_year + 1900;

					my4XRegs[U_set_tiempo] = 0x01;

					PRINTFDEBUG("Fecha -> %u/%u/%u %u:%u:%u\n\r", rtc.tm_year + 1900, rtc.tm_mon, rtc.tm_mday, rtc.tm_hour, rtc.tm_min, rtc.tm_sec);

					waitfor(DelayMs(100));
					//                      mmPresetRegs_alter(  unsigned wAddr,   unsigned wReg,   unsigned wCount,          void *pwRegs)
					waitfor(error = mmPresetRegs_alter(101, U_segundos, 7, (int *)&my4XRegs[U_segundos])); // Set Time TRIFURCACION
					if (error != 0xffff)
					{
						PRINTFDEBUG("Error Sinc Time Pozo 0\n");
						nada = 0x00;
					}
					else
					{
						PRINTFDEBUG("Sinc Time Pozo 1\n");
						nada = 0x00;
					}

					waitfor(DelayMs(100));
					waitfor(error = mmPresetRegs_alter(101, U_segundos, 7, (int *)&my4XRegs[U_segundos])); // Set Time CTM

					waitfor(DelayMs(100));
					waitfor(error = mmPresetRegs_alter(104, U_segundos, 7, (int *)&my4XRegs[U_segundos])); // Set Time GM14

					waitfor(DelayMs(100));
					waitfor(error = mmPresetRegs_alter(105, U_segundos, 7, (int *)&my4XRegs[U_segundos])); // Set Time GM14 A

					waitfor(DelayMs(100));
					waitfor(error = mmPresetRegs_alter(107, U_segundos, 7, (int *)&my4XRegs[U_segundos])); // Set Time ZACATENCO 2

					if (error != 0xffff)
					{
						PRINTFDEBUG("Error Sinc Time ZACATENCO 2\n");
						nada = 0x00;
					}
					else
					{
						PRINTFDEBUG("Sinc Time ZACATENCO 2\n");
						nada = 0x00;
					}
					new_hora = rtc.tm_hour;
					PRINTFDEBUG("Fecha UTR -> %u/%u/%u %u:%u:%u\n\r", rtc.tm_year + 1900, rtc.tm_mon, rtc.tm_mday, rtc.tm_hour, rtc.tm_min, rtc.tm_sec);
				}

				if (conta_seg >= 2)
				{
					conta_seg = 0;
					conta_loop++;
					PRINTFDEBUG("Conta_loop=%u\n", conta_loop);
					tiempo_inicio = read_rtc();
					///////////////////////////////////////////////////////////////////////////////////////////////////
					////       WR No de Version en HMI
					///////////////////////////////////////////////////////////////////////////////////////////////////

					if (flag_inicio_version == 1)
					{ // Write No Version Software RABBIT
						waitfor(DelayMs(100));
						//                     mmPresetRegs_gate(  unsigned wAddr,          unsigned wReg,   unsigned wCount,                          void *pwRegs)
						waitfor(error = mmPresetRegs_gate(addr_reg_mb_HMI, addr_reg_mb_version, 2, (int *)&my4XRegs[addr_reg_mb_version]));
						if (error == 0xffff)
						{
							PRINTFDEBUG("WR OK No_Version\n");
							flag_inicio_version = 0;
						}
						else
						{
							PRINTFDEBUG("WR Error Version\n");
							nada = 0x00;
						}
					}

					if (flag_inicio_control_A == 1)
					{
						my4XRegs[addr_reg_mb_ctrl_regio] = 0x00; // Reset registro control Bombas Scairlink
						waitfor(DelayMs(100));
						// mmPresetReg_gate(  unsigned wAddr,         unsigned wReg, unsigned wVal)
						waitfor(error = mmPresetReg_gate(addr_reg_mb_HMI, addr_reg_mb_ctrl_hmi, 0));
						if (error == 0xffff)
						{
							if (my4XRegs[addr_reg_mb_ctrl_hmi] == 0)
							{
								PRINTFDEBUG("WR OK reset_cmd_control_A_HMI\n");
								flag_inicio_control_A = 0;
							}
						}
						else
						{
							PRINTFDEBUG("WR Error CTRL A\n");
							nada = 0x00;
						}
					}

					if (flag_inicio_control_B == 1)
					{
						waitfor(DelayMs(100));
						// mmPresetReg_alter(unsigned wAddr,          unsigned wReg, unsigned wVal)
						waitfor(error = mmPresetReg_alter(dir_scairlink, addr_reg_mb_ctrl_hmi, 0));
						if (error == 0xffff)
						{
							PRINTFDEBUG("WR OK reset__B_Scairlink\n");
							flag_inicio_control_B = 0;
						}
						else
						{
							PRINTFDEBUG("WR Error CTRL B\n");
							nada = 0x00;
						}
					}

					/*
            wfd error = COF_RD_INSTA_UTR();                                // RD valores Instantaneos del
            enlace_global_1  = esta_com[0].enlace;
            enlace_global_2  = esta_com[1].enlace;
            enlace_global_3  = esta_com[2].enlace;
            if( enlace_global_1==1 && enlace_global_2==1 && enlace_global_3==1 ){
             	conta_error_conse_scairlink = 0x00;
              for(tt=0; tt<No_max_sitios; tt++){
                No_dispo = tt;
                //memcpy((char*)&my4XRegs[tabla[No_dispo].incio_tab_hmi],(char*)&my4XRegs[tabla[No_dispo].incio_tab_hmi], tabla[No_dispo].cantidad_hmi*2);      // Copia valores       ( CHALMITA )
                //my4XRegs[tabla[No_dispo].enlace_hmi] = my4XRegs[tabla[No_dispo].enlace_scair];                                // enlace radio CHALMITA
                yield;
              }
              for(z=310; z<=320; z++){
                  PRINTFDEBUG("%u,",my4XRegs[z]);
              }
              PRINTFDEBUG("\n");
            }else{
              conta_error_conse_scairlink++;
							if( conta_error_conse_scairlink >= No_max_errores_conta_scairlink){
              	conta_error_conse_scairlink = No_max_errores_conta_scairlink;
                for(tt=0; tt<No_max_sitios; tt++){
                  No_dispo = tt;
                  my4XRegs[tabla[No_dispo].enlace_hmi] = 0x00;
                }
              }
              PRINTFDEBUG("Err_RD_Scair\n");
            }
            for(tt=0; tt<No_max_sitios; tt++){
             	No_dispo = tt;
              if( my4XRegs[tabla[No_dispo].enlace_hmi] == 0x01 ){
               apunta_tiempo = (int *)&my4XRegs[tabla[No_dispo].tiempo_hmi];
               Almacena_Datos_Tiempo( apunta_tiempo );
              }
				    }
          */
					/////////////////////////////////////////////////////////////////////////////////////////////////////
					//
					//     READ DATOS LOCAL SCAIRLINK   ( Falla Energia, Mante, Volt Bateria)  y  WRITE DATOS ( Nivel, Presion, Flujo, Posi Valvula )   Rabbit <---> Scairlnk
					//
					/////////////////////////////////////////////////////////////////////////////////////////////////////
					waitfor(DelayMs(100));
				}

				//               it  = mmRead_alter(   unsigned wAddr,                         unsigned wReg,                    unsigned wCount,                                  void *pwRegs)
				while (mutex_mensajes_scarling == MUTEX_OCUPADO)
				{
					yield;
				}
				mutex_mensajes_scarling = MUTEX_OCUPADO;
				waitfor(error = mmRead_alter(dir_scairlink, tabla[dispo_local].incio_tab_hmi, tabla[dispo_local].cantidad_hmi, (int *)&my4XRegs[tabla[dispo_local].incio_tab_hmi])); // Read registros Scairlink
				mutex_mensajes_scarling = MUTEX_LIBRE;
				if (error == 0xffff)
				{
					//memcpy((char*)&my4XRegs[tabla[dispo_local].incio_tab_hmi],(char*)&my4XRegs[tabla[dispo_local].incio_tab_scair], tabla[dispo_local].cantidad_hmi*2);      // Copia valores       ( CHALMITA )
					/*
              if( my4XRegs[tabla[dispo_local].enlace_hmi] >= 2){
                 my4XRegs[tabla[dispo_local].enlace_hmi] = 0x01;
              }else{
              	my4XRegs[tabla[dispo_local].enlace_hmi] = 0x00;
              }
            */
					my4XRegs[322] = 0x01;							// Mantenimiento
					my4XRegs[tabla[dispo_local].enlace_hmi] = 0x01; // Enlace
					apunta_tiempo = (int *)&my4XRegs[tabla[dispo_local].tiempo_hmi];
					Almacena_Datos_Tiempo(apunta_tiempo);
				}
				else
				{
					PRINTFDEBUG("Error RD UTR LOCAL\n");
					my4XRegs[tabla[dispo_local].enlace_hmi] = 0x00;
				}
				///////////////////////////////////////////////////////////////////////////////////////////////////
				////       C O N T R O L      B O M B A S       H M I
				///////////////////////////////////////////////////////////////////////////////////////////////////
				if (flag_inicio_control_A == 0)
				{
					waitfor(DelayMs(100));
					//               it  = mmRead_gate(      unsigned wAddr,         unsigned wReg,   unsigned wCount,             void *pwRegs)
					waitfor(error = mmRead_gate(addr_reg_mb_HMI, addr_reg_mb_ctrl_hmi, 1, (int *)&cmd_control_hmi)); // Read Registros
					if (error == 0xffff)
					{
						if (cmd_control_hmi != 0)
						{
							waitfor(DelayMs(100));
							// mmPresetReg_gate(   unsigned wAddr,          unsigned wReg, unsigned wVal)
							waitfor(error = mmPresetReg_gate(addr_reg_mb_HMI, addr_reg_mb_ctrl_hmi, 0));
							if (error == 0xffff)
							{
								PRINTFDEBUG("--%x\n", cmd_control_hmi);
								nada = 0x00;
							}
							else
							{
								cmd_control_hmi = 0x00;
							}
						}
					}
					else
					{
						cmd_control_hmi = 0x00;
					}
				}

				if (flag_inicio_control_B == 0)
				{
					waitfor(DelayMs(100));
					//               it  = mmRead_alter(    unsigned wAddr,           unsigned wReg,   unsigned wCount,                  void *pwRegs)
					while (mutex_mensajes_scarling == MUTEX_OCUPADO)
					{
						yield;
					}
					mutex_mensajes_scarling = MUTEX_OCUPADO;
					waitfor(error = mmRead_alter(dir_scairlink, addr_reg_mb_ctrl_regio, 1, (int *)&cmd_control_regional)); // Read Registros
					mutex_mensajes_scarling = MUTEX_LIBRE;
					if (error == 0xffff)
					{
						if (cmd_control_regional != 0)
						{
							waitfor(DelayMs(100));
							// mmPresetReg_alter(   unsigned wAddr,            unsigned wReg, unsigned wVal)
							waitfor(error = mmPresetReg_alter(dir_scairlink, addr_reg_mb_ctrl_regio, 0));
							if (error == 0xffff)
							{
								PRINTFDEBUG("--%x\n", cmd_control_regional);
								nada = 0x00;
							}
							else
							{
								cmd_control_regional = 0x00;
							}
						}
					}
					else
					{
						cmd_control_regional = 0x00;
					}
				}

				if (cmd_control_hmi != 0)
				{
					cmd_control = cmd_control_hmi;
				}
				else
				{
					if (cmd_control_regional != 0)
					{
						cmd_control = cmd_control_regional;
						cmd_control_regional = 0x00;
					}
				}

				//////////////////////////////////////////////////////////////////////////
				//     PROCESA CONTROL REGIONAL
				//////////////////////////////////////////////////////////////////////////

				//if( (cmd_control != 0) && (my4XRegs[dir_modo_control] == 2) )   // Hay un comando control y perilla en automatico
				if ((cmd_control != 0))
				{ // Hay un comando control y perilla en automatico
					PRINTFDEBUG("Reg cmd_control_regional = %x\n", cmd_control);
					No_sitio = cmd_control & 0xFF00;
					No_sitio = No_sitio >> 8;

					if (No_sitio != Numero_sitio)
					{
						if (No_sitio > 0 && No_sitio < 55)
						{
							waitfor(DelayMs(100));
							// mmPresetReg_alter(  unsigned wAddr,            unsigned wReg,    nsigned wVal)
							waitfor(error = mmPresetReg_alter(No_sitio + 100, addr_reg_mb_ctrl_regio, cmd_control));
							PRINTFDEBUG("START/STOP %d,%d\n", No_sitio, cmd_control);
							if (error != 0xffff)
							{
								PRINTFDEBUG("Falla Control por radio, escribiendo en 2390 %d\n", No_sitio);
								nada = 0x00;
								my4XRegs[2390] = cmd_control;
								my4XRegs[2391] = 101;
							}
						}
						else
						{
							PRINTFDEBUG("Error sitio Equivocado\n");
							nada = 0x00;
						}
					}
					else
					{
						PRINTFDEBUG("Error sitio Equivocado\n");
						nada = 0x00;
					}
					cmd_control = 0x00;
					cmd_control_hmi = 0x00;
					my4XRegs[addr_reg_mb_ctrl_regio] = 0x00;
				}
				///////////////////////////////////////////////////////////////////////////////////////////////////
				////       W R I T E        C O U N T E R      S E C U E N C I A L        H M I
				///////////////////////////////////////////////////////////////////////////////////////////////////

				conta_secuen_hmi++;
				waitfor(DelayMs(100));
				// mmPresetReg_gate(   unsigned wAddr,            unsigned wReg,      unsigned wVal)
				waitfor(error = mmPresetReg_gate(addr_reg_mb_HMI, addr_reg_conta_sec_hmi, conta_secuen_hmi)); // WR conta secuencial
				if (error == 0xffff)
				{
					nada = 0x00;
				}
				else
				{
					PRINTFDEBUG("Error Conta Sec HMI\n");
					nada = 0x00;
				}

				if (my4XRegs[reset_slink] == 0xaa55)
				{ // Reset hardware Medidor de Nivel
					digOut(0, 0);
					waitfor(DelayMs(2000));
					digOut(0, 1);
					my4XRegs[reset_slink] = 0x00;
				}

				if (my4XRegs[reset_alarma] == 0xaa55)
				{
					digOut(1, 0);
					waitfor(DelayMs(2000));
					digOut(1, 1);
					my4XRegs[reset_alarma] = 0x00;
				}

				if (my4XRegs[reset_rabbit] == 0xaa55)
				{
					digOut(2, 0);
					waitfor(DelayMs(2000));
					digOut(2, 1);
					my4XRegs[reset_rabbit] = 0x00;
				}

				tiempo_final = read_rtc();
				ventana_tiempo = tiempo_final - tiempo_inicio;
				PRINTFDEBUG("Tiempo proceso=%lu\n", ventana_tiempo);
			}
		}
	}

	/*===================================================================
	      MODULO QUE A TRAVES MODBUS MANDA EJECUTAR MANDO
	  ===================================================================*/
	costate
	{
		if (ejecuta_mando == 0x01)
		{
			wfd COF_Genera_Pulso_Digi(No_salida);
			ejecuta_mando = 0x00;
		}
	}
}
}
