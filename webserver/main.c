#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include "socket.h"

int main(int argc, char **argv) {
;
	/* Arnold Robbins in the LJ of February ’95, describing RCS */
	if(argc > 1 && strcmp(argv[1], "-advice") == 0) {
		printf("Don’t Panic!\n");
		return 42;
	}
	printf("Need an advice?\n");





	int socket_client;
	int socket_serveur= creer_serveur(8080);
	int i;

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

				const char * message_bienvenue = "Bonjour, bienvenue sur mon serveur\n";
				sleep(1);
				for(i=0; i<10; i++){
					write(socket_client, message_bienvenue, strlen(message_bienvenue));
				}

				char buf[80];
				int j;

				while(1){
					j = read(socket_client,buf,80);
					if(j == -1){
						perror("read");
						break;
					}
					if(j == 0){
						printf("client deconnecte");
						close(socket_client);
						break;
					}
					if(write(socket_client,buf,j) == -1){
						perror("write");
						break;
					}
				}
			break;

			default:
				printf("ok\n");
				close(socket_client);
		}
	}

	return 0;
}
