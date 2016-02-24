#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "socket.h"

void traiterClient(int socket_client){

	char * msg_bienvenue = "Bonjour, bienvenue sur notre serveur.\r\n";
	char * bad_req = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 17\r\n400 Bad request\r\n";
	char * good_req = "HTTP/1.1 200 OK\r\nContent-Length: ";
	FILE * file_client = fdopen(socket_client,"w+");
	char buf[80];
	char *rd;

	while(1){

		rd = fgets(buf, 80, file_client);
		printf("%s\n", buf);
		char * req = strtok(buf, " ");

		if(strcmp(req,"GET") == 0){
			req = strtok(NULL, " ");
			req = strtok(NULL, " ");
			if(strcmp(req,"HTTP/1.1\n") == 0 || strcmp(req,"HTTP/1.0\n") == 0){
				printf("requete acceptee\n");
				fprintf(file_client,"%s%zuhttp://\r\n\r\n%s",good_req, strlen(msg_bienvenue), msg_bienvenue);
			}
			else{
				printf("HTTP");
				fprintf(file_client,"%s",bad_req);
			}
		}
		else{
			printf("GET");
			fprintf(file_client,"%s",bad_req);
		}

		if(rd == NULL){

			printf("Deconnexion Client\n");
			close(socket_client);
			break;
		}
	}
	exit(0);
}

int main(int argc, char **argv) {

	/* Arnold Robbins in the LJ of February ’95, describing RCS */
	if(argc > 1 && strcmp(argv[1], "-advice") == 0) {
		printf("Don’t Panic!\n");
		return 42;
	}
	printf("Need an advice?\n");

	int socket_client;
	int socket_serveur= creer_serveur(8080);

	if(socket_serveur == -1){
		return 1;
	}

	while(1){

		socket_client = accept(socket_serveur, NULL, NULL);
		if(socket_client == -1) {
			perror("accept");
		}

		int pid= fork();
		initialiser_signaux();

		switch(pid){

			case -1:
				perror("fork");
				return -1;
			break;

			case 0:
			traiterClient(socket_client);	
			break;

			default:
				close(socket_client);
		}
	}

	return 0;
}

