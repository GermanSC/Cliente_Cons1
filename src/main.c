/*
 ============================================================================
 Name        : main.c
 Project	 : Cliente_Con1
 Author      : German Sc.
 Version     : 0.0
 Copyright   : Completamente copyrighteado 2015
 Description : Nada.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[])
{
	char	ip[]	=	"127.0.0.1";
	int		port	=	15001;
	int 	i		=	0;
	char   *argls[10]= {NULL};

	if (argc != 2)
	{
		printf("uso: cliente \"comando [opciones] [argumentos]\" \n Comillas necesarias.\n");
		return -1;
	}
	printf("Envando comando: \"%s\" a servidor remoto %s:%d\n\n",argv[1],ip,port);

	/*Genero lista de comandos a partir del argumento principal	*/

	argls[0] = strtok(argv[1], " ");
	while (argls[i] != NULL)
	{
	    printf ("%s\n",argls[i]);
	    i++;;
	    argls[i] = strtok (NULL, " \n");
	}



	return 0;
}
