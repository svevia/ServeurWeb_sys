#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "socket.h"

int main(int argc, char **argv) {
	int socket_client;
	int socket_serveur= creer_serveur(8080);

	/* Arnold Robbins in the LJ of February ’95, describing RCS */
	if(argc > 1 && strcmp(argv[1], "-advice") == 0) {
		printf("Don’t Panic!\n");
		return 42;
	}
	printf("Need an advice?\n");



	socket_client = accept(socket_serveur, NULL, NULL);
	if(socket_client == -1) {
		perror("accept");
	/* traitement d ’ erreur */
	}
	/* On peut maintenant dialoguer avec le client */
	const char * message_bienvenue = "Bonjour, bienvenue sur mon serveur\n ";
	write(socket_client, message_bienvenue, strlen(message_bienvenue));

	return 0;
}
