#include "consola.h"

int es_numero(char* check)
{
	int recount = 0;
	for(recount = 0; recount < strlen(check); recount ++)
	{
		if(!isdigit(check[recount]))
		{
			loggear_error("El parámetro ingresado no es numérico.");
			return 0;
		}
	}
	return 1;
}

void mandar_sabotaje(char** args){}
// check_argumentos : Verifica si la cantidad de parametros ingresada es la mínima para ejectuar el programa
int check_argumentos(char** args, int cantArgs)
{
    int i;
    for (i=1; i < cantArgs; i++)
    {
    	if(args[i] == NULL)
    	{
    		puts("No se recibieron suficientes parámetros.");
    		loggear_error("No se recibieron suficientes parámetros.");
    		return 0; //Si alguno de los parametros es null, no me sirve.
    	}
    }
    return 1; //Si todos los parametros existen, procedo al siguiente paso
}

int obtener_cant_tripulantes(char* parametroCantTripulante)
{
	if(es_numero(parametroCantTripulante))
	{
		return atoi(parametroCantTripulante);
	}
	else
	{
		return 0;
	}
}

char* leer_archivo(char** args)
{
	 char* path = string_duplicate(args[2]);
	 FILE *file = fopen(path, "r");
	 fseek(file, 0, SEEK_END);
	 long fileSize = ftell(file);
	 fseek(file, 0, SEEK_SET);
	 char *contenido = malloc(fileSize + 1);
	 fread(contenido, 1, fileSize, file);
	 fclose(file);

	 contenido[fileSize] = 0;
	 loggear_debug("CONTENIDO: %s", contenido);
	 free(path);
	 return contenido;

}

int es_un_archivo(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
void mockear_patota(char** args)
{
	 if(check_argumentos(args, 2)){
		 int error = 0;
		 puts("Se mockea la patota");
		 int cantTrip = atoi(args[1]);

		 int pid = obtener_siguiente_patota();
		 t_patota * patota = mock_patota(pid, cantTrip, 1);
		 error = enviar_patota_ram(patota);
		 if (error == 0) {
			 agregar_nuevos_tripulantes(patota);
			 loggear_trace("La patota se creó exitosamente.");
			 puts("La patota se creó exitosamente.");
		 }
		 else
		 {
			 loggear_error("No se pudo crear la patota");
			 puts("ERROR: no se pudo crear la patota");
			 destruir_patota(patota);
		 }
	 }
}
//////////////////////////////////////////   COMANDOS DE CONSOLA   //////////////////////////////////////////
void ini_patota(char** args){
	 if(check_argumentos(args, 3)){
		uint32_t cantTripulantes = obtener_cant_tripulantes(args[1]);
		char* path = string_duplicate(args[2]);

		 loggear_debug("El path indicado es: %s",path);
		 if (es_un_archivo(path))
		 {
			 loggear_debug("Se encontró el archivo.");
			 if (cantTripulantes != 0)
			 {
				 loggear_trace("Se entra al if con cantTripulantes");
				 uint32_t pid = obtener_siguiente_patota();
				 loggear_trace("Se obtiene la patota");
				 char* tareas = leer_archivo(args);
				 loggear_trace("Se lee el archivo");
				 t_patota * patota = crear_patota(pid, cantTripulantes, tareas);
				 free(tareas);
				 loggear_trace("Se crea la patota");
				 int params = 3; //Inicializado en 3 porque va a ser el 1er parámetro que va a leer como posición
				 int posiciones = 0; //Cantidad de posiciones que se pasaron por parámetro
				 while (args[params] != NULL)
				 {
					params++;
					posiciones++;
				 }
				 loggear_trace("Se detectan las posiciones pasadas");
				 for (int i = 0; i < cantTripulantes; i++)
				 {
					int tid = obtener_siguiente_tripulante();
					t_tripulante * tripu = patota->tripulantes[i];
					tripu->tid = tid;
					if(i < posiciones && strcmp(args[i+3], ""))
					{
						char** posArgs = string_split(args[i+3], "|"); //+3 por lo mismo de antes, 1er parámetro en el string
						tripu->pos_x = atoi(posArgs[0]);
						tripu->pos_y = atoi(posArgs[1]);
						string_iterate_lines(posArgs,(void*)free);
						free(posArgs);
					}
					else
					{
						tripu->pos_x = 0;
						tripu->pos_y = 0;
					}
					loggear_debug("Se leyó esto desde la consola: x[%d]: %d, y[%d]: %d", tripu->tid, tripu->pos_x, tripu->tid, tripu->pos_y);
				 }
				 int error = 0;
				 error = enviar_patota_ram(patota);
				 loggear_trace("Se envió la patota a la RAM");
				if (error == 0) {
					 agregar_nuevos_tripulantes(patota);
					 loggear_trace("La patota se creó exitosamente.");
					 puts("La patota se creó exitosamente.");
					 destruir_patota_sin_tripulantes(patota);
				 }
				 else
				 {
					 loggear_error("No se pudo crear la patota");
					 puts("ERROR: no se pudo crear la patota");
					 destruir_patota(patota);
				 }
			}
		 }
		 else
		 {
			 puts("ERROR: No se pudo encontrar el archivo.");
			 loggear_error("No se pudo encontrar el archivo.");
		 }
		 free(path);

	 }
}

void list_tripulantes(char** args){
	listar_tripulantes();
}

void ini_planificacion(char** args){
	puts("Arranca planificación.");
	loggear_trace("Se manda a iniciar planificación desde la consola.");
	planificador_despausar();
}

void pau_planificacion(char** args){
	puts("Pausa planificación.");
	loggear_trace("Se manda a pausar planificación desde la consola.");
	planificador_pausar();
}

void obt_bitacora(char** args){
	 if(check_argumentos(args, 2)) {
		 if(es_numero(args[1])) {
			 printf("Mostramos la bitácora del tripulante con TID %s.\n", args[1]);
			 loggear_trace("Se procede a mostrar la bitácora del tripulante TID: %s", args[1]);
			 uint32_t tid = atoi(args[1]);
			 puts("Vos juzgás si laburó o no.");
		 }
	 }
}

// Lista de comandos y su función a ejecutar.
COMANDOS comandos[] = {
        { "INICIAR_PATOTA", 		ini_patota },
        { "LISTAR_TRIPULANTES",		list_tripulantes },
        { "INICIAR_PLANIFICACION", 	ini_planificacion },
        { "PAUSAR_PLANIFICACION", 	pau_planificacion },
        { "OBTENER_BITACORA", 		obt_bitacora },
		{ "MOCK_PATOTA", 			mockear_patota },
		{ "MANDAR_SABOTAJE", 		mandar_sabotaje },
        {(char *) NULL, (Funcion *) NULL}
};

// buscar_comando : revisa si está en el array de comandos a ejecutar.
COMANDOS * buscar_comando(char* linea) {
    int i;

    for (i = 0; comandos[i].nombre; i++){
        // Paso a miniscula y comparo
        string_to_lower(linea);
        char* comando = string_duplicate(comandos[i].nombre);
        string_to_lower(comando);

        if (strcmp (linea, comando) == 0){
            free(comando);
            return (&comandos[i]);
        }
        free(comando);

    }
    return ((COMANDOS *) NULL);
}


// mostrar_comandos : muestra los comandos a ejecutar. Puede ser invocado con 'AYUDA'
void mostrar_comandos (){
	puts("--------------------------------------------------------------------");
	puts("                          CONSOLA PLANIFICADOR");
	puts("                           Escriba un comando");
	puts(" - INICIAR_PATOTA  <cantTripulantes> <archivo> <posX|posY>(opcional)");
	puts(" - LISTAR_TRIPULANTES");
	puts(" - INICIAR_PLANIFICACION");
	puts(" - PAUSAR_PLANIFICACION");
	puts(" - OBTENER_BITACORA <idTripulante>");
	puts(" - AYUDA (Muestra los comandos de la consola.)");
	puts(" - EXIT (Sale del programa.)");
	puts("--------------------------------------------------------------------");
}

// ejecutar_linea : Busca ejecutar el comando indicado
int ejecutar_linea (char * linea){
    char * linea_aux = string_duplicate(linea);
    int i = 0;
    char * funcion;

    if(string_contains(linea, " "))
    {
        while (linea_aux[i] != ' ') i++;

        funcion = malloc((sizeof(char) * i) + 1);
        strncpy(funcion, linea_aux, i);
        funcion[i] = '\0';
    }
    else
    {
        funcion = string_duplicate(linea_aux);
    }

    COMANDOS * comando = buscar_comando(funcion);
    if (comando == NULL)
    {
    	free(funcion);
    	free(linea_aux);
    	return 0;
    }
    else
    {
    	free(funcion);
    	char** args;
		args = string_split(linea_aux, " ");
		//EJECUCION DE FUNCION
		(*(comando->funcion)) (args);
		loggear_info("Se reconoció el comando ingresado.");
    	free(linea_aux);
    	string_iterate_lines(args, (void*)free);
    	free(args);
    	return 1;
    }
}

// levantar consola : inicializa la consola y todos sus componentes.
void levantar_consola (){
	mostrar_comandos();
    while(1){
    	char* linea = readline("\nIngrese un comando: ");
    	char* aux = malloc(strlen(linea) + 1);
    	strcpy(aux, linea);
    	string_to_upper(aux);
    	if (!strcmp(aux, "EXIT")){
    		free(linea);
    		free(aux);
    		clear_history();
    		puts("Adiós! Gracias por venir!");
    		sleep(1);
    		loggear_trace("Se cierra la consola.");
    		break;
    	}
    	if (!strcmp(aux, "AYUDA")){
    		add_history(linea);
    		mostrar_comandos();
    	}
    	else if (ejecutar_linea(linea)){
    		add_history(linea);
    	}
    	else
    	{
    		add_history(linea);
    		puts("No se reconoció el comando.");
    	}
    	free(linea);
    	free(aux);
    }
}

