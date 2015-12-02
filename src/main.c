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
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CMD	20

int main(int argc, char *argv[])
{
	/*	Variables de conexión	*/
	char	ip[]	=	"127.0.0.1";
	int		port	=	15001;
	int		sockfd	=	-1;
	struct sockaddr_in server_info;

	/*	Variables de control	*/
	int		ctrl = 0;
	char test[6];

	/*	Comienzo del programa	*/

	if (argc != 2)
	{
		printf("uso: cliente \"comando [opciones] [argumentos]\" \n Comillas necesarias.\n");
		return -1;
	}
	printf("Envando comando: \"%s\" a servidor remoto %s:%d\n",argv[1],ip,port);

	/*Genero lista de comandos a partir del argumento principal	*/

	/*	Socket Set-up	*/

	sockfd = socket(PF_INET,SOCK_STREAM,0);
		if(sockfd == -1)
		{
			printf(">>ERROR: No se pudo abrir el socket.\n\n");
			return -1;
		}

	inet_pton(AF_INET, ip, &(server_info.sin_addr));
	server_info.sin_family		=	AF_INET;
	server_info.sin_port		=	htons(port);

	/*	Configuración para el select	*/
	fd_set readfds;
	struct timeval tv;
	tv.tv_sec	=	0;
	tv.tv_usec	= 	500000;
	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);

	printf("Intentando conectar con servidor... ");

	ctrl = connect (sockfd, (struct sockaddr *)&server_info, sizeof (struct sockaddr_in));
	if(ctrl == -1)
		{
			printf(">>ERROR: No se pudo establecer la conexión.\n\n");
			close(sockfd);
			return -1;
		}

	printf("Conexión establecida.\nPasando comandos y argumentos... ");

	write(sockfd,argv[1],strlen(argv[1])+1);

	for(ctrl = 0; ctrl < sizeof(test);ctrl++)
	{
		test[ctrl] = '\0';
	}

	read(sockfd,test,6);
	if(strcmp(test,"Listo") == 0)
	{
		printf("Éxito\n----------------------------------------\n\n");
	}
	else
	{
		printf("ERROR.\n");
		close(sockfd);
		return -1;
	}

	/*	Comienzo de ejecución remota	*/

	char buff[1000];

	while(1)
	{
		select(sockfd+1,&readfds,NULL,NULL,&tv);

		if (FD_ISSET(sockfd, &readfds))
		{	/*	Se pueden recibir datos	*/
			ctrl = read(sockfd,buff,sizeof buff);

			if(ctrl == 0)
				{
				printf("\n----------------------------------------\n\n");
				close(sockfd);
				break;
				}
			else
			{
				printf("%s",buff);
			}
		}
		else
		{	/*	No se envió nada, el comando espera input	*/
			fgets(buff, sizeof(buff), stdin);
			send(sockfd,buff,strlen(buff),0);
		}
	for(ctrl=0;ctrl<sizeof buff;ctrl++)
	{
		buff[ctrl] = '\0';
	}

	}

	return 0;

	while( strncmp(buff,"CMD_DONE",8) != 0 )
	{
		fgets(buff, sizeof(buff), stdin);
		send(sockfd,buff,strlen(buff),0);

	}
	printf("----------------------------------------\n\n");

	/*	Fin de Programa	*/
	close(sockfd);
	return 0;
}
