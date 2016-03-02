#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "socket.h"
#include "http.h"
#include "client.h"


void traiterClient(int socket_client){

	FILE * file_client = fdopen(socket_client,"w+");

	char reqHTTP[80];

	fgets_or_exit(reqHTTP, 80, file_client);
	skip_headers(file_client);
	goHTTP(file_client, reqHTTP);
	fclose(file_client);
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

