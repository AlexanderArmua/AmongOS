/**
 * Todos los codigos de error de la aplicacion general van aca
 */
#ifndef CODIGO_ERROR_H_
#define CODIGO_ERROR_H_

#define STATUS_OK 0

// Lectura de archivo de configuracion [-1 a -10]
#define CONFIG_ARGUMENTOS_INVALIDOS -1
#define CONFIG_ERROR_EN_ARCHIVO -2
#define RECURSO_YA_DECLARADO -3
#define RECURSO_NO_DECLARADO -4

// Sockets [-11 a -20]
#define ERROR_AL_ABRIR_SOCKET -11
#define ERROR_AL_CONECTAR_AL_SERVIDOR -12
#define ERROR_AL_BINDEAR_EL_SERVIDOR -13
#define ERROR_AL_ESCUCHAR_EL_SOCKET -14

// Protocolos [-21 a -30]
#define FALLO_AL_ENVIAR_A_TRAVES_DEL_SOCKET -21
#define ERROR_DESCONEXION_RAM -22
#define ERROR_DESCONEXION_FILESYSTEM -23
#define ERROR_DESCONEXION_PLANIFICADOR -24
#define ERROR_AL_RECIBIR_MENSAJE -25
#define ERROR_MENSAJE_DESCONOCIDO -26

// Hilos [-31 a -40]
#define ERROR_AL_ABRIR_EL_SOCKET -31
#define ERROR_SOCKET_NO_ABIERTO -32

#endif
