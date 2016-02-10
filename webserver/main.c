#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "socket.h"

int main(int argc, char **argv) {

	/* Arnold Robbins in the LJ of February ’95, describing RCS */
	if(argc > 1 && strcmp(argv[1], "-advice") == 0) {
		printf("Don’t Panic!\n");
		return 42;
	}
	printf("Need an advice?\n");

	int socket_client;
	int socket_serveur= creer_serveur(8080);
	int i;
	FILE * file_client;

	if(socket_serveur == -1){
		return 1;
	}
	while(1){
		socket_client = accept(socket_serveur, NULL, NULL);



		int pid= fork();
		initialiser_signaux();
		switch(pid){
			case -1:
				perror("fork");
				return -1;
			break;

			case 0:
				if(socket_client == -1) {
					perror("accept");
				}

				file_client = fdopen(socket_client,"w+");

				const char * message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";
				sleep(1);
				for(i=0; i<10; i++){
					fprintf(file_client, message_bienvenue);
					fflush(file_client);
					//write(socket_client, message_bienvenue, strlen(message_bienvenue));
				}

				char buf[80];
				char *rd;
				char* balise = "<serve>\n";
		//		char* balise2 = "</serve>";

				while(1){

					rd = fgets(buf, 80, file_client);

					if(rd == NULL){
						printf("deconnexion client\n");
						close(socket_client);
						break;
					}
					int wr = fprintf(file_client,"%s%s", balise, buf);
					fflush(file_client);

					if(wr < 0){
						perror("fprintf");
						break;
					}

				}
			exit(0);
			break;

			default:
				close(socket_client);
				printf("ok\n");
		}
	}

	return 0;
}
