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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	/*	Variables de conexión	*/
	char	ip[INET_ADDRSTRLEN]	=	"127.0.0.1";
	int		port	=	15001;
	int		sockfd	=	-1;
	struct sockaddr_in server_info;

	/*	Variables de control	*/
	int		ctrl = 0;
	char	test[6];
	char buff[1000];

	/*	Comienzo del programa	*/

	if (argc != 2)
	{
		printf("uso: cliente \"comando [opciones] [argumentos]\" \n Comillas necesarias.\n");
		return -1;
	}
	printf("Envando comando: \"%s\" a servidor remoto %s:%d\n",argv[1],ip,port);

	/* Set de descriptores para lectura	*/

	fd_set readfds;

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

	read(sockfd,test,6);

	if(strncmp(test,"Listo",6) == 0)
	{

		printf("Éxito\n----------------------------------------\n\n");

	}else{

		printf("ERROR.\n");
		close(sockfd);
		return -1;
	}

	/*	Comienzo de ejecución remota	*/

	while(1)
	{
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);
		FD_SET(STDIN_FILENO,&readfds);

		do{

			ctrl = select(sockfd+1,&readfds,NULL,NULL,NULL);

		}while( ctrl == -1 && errno == EINTR );
		if( ctrl == -1 )
		{
			printf("Error de Select.\n\n");
			close(sockfd);
			return -1;
		}

		if ( FD_ISSET(sockfd, &readfds) != 0 )
		{
			/*	Hay datos en el socket	*/

			ctrl = read(sockfd,buff,sizeof(buff));

			if(ctrl == 0)	/*	se cerró la conexión	*/
			{
				printf("\n----------------------------------------\n\n");
				break;

			} else {

				printf("%.*s",ctrl,buff);

			}
		}

		if ( FD_ISSET(STDIN_FILENO, &readfds) != 0 )
		{
			/*	input de STDIN	*/

			ctrl = read(STDIN_FILENO,buff, sizeof buff);
			send(sockfd,buff,ctrl,0);
		}
	}

	/*	Fin de Programa	*/
	close(sockfd);
	return 0;
}
