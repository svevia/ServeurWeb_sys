#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>
#include "socket.h"
#include "http.h"
#include "client.h"


void traiterClient(int socket_client, const char *document_root){

	FILE * file_client = fdopen(socket_client,"w+");
	char reqHTTP[8192];

	fgets_or_exit(reqHTTP, 8192, file_client);
	skip_headers(file_client);
	goHTTP(file_client, reqHTTP, document_root);
	fclose(file_client);
	exit(0);
}

int main(int argc, char **argv){

	const char *document = argv[1];
	struct stat st_doc;

	if(argc == 2){
		stat(document, &st_doc);
		if(!S_ISDIR(st_doc.st_mode)){
			fprintf(stderr, "dossier racine invalide ou non existant\n");
			exit(0);
		}
		if(open(document, O_RDONLY) == -1){
			fprintf(stderr, "permission refusée\n");
			exit(0);
		}
	}
	else{
		fprintf(stderr, "nombre d'arguments non valide\n");
		exit(0);
	}
	
	int socket_client;
	int socket_serveur = creer_serveur(8080);

	if(socket_serveur == -1){
		return 1;
	}

	while(1){

		socket_client = accept(socket_serveur, NULL, NULL);
		if(socket_client == -1) {
			perror("accept");
		}

		int pid = fork();
		initialiser_signaux();

		switch(pid){

			case -1:
			perror("fork");
			return -1;
			break;

			case 0:
			traiterClient(socket_client, document);	
			break;

			default:
			close(socket_client);
		}
	}
	return 0;
}