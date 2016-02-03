#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int socket_serveur;
 /* écoute sur toutes les interfaces */

int creer_serveur(int port){

	socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;				/* Socket ipv4 */
	saddr.sin_port = htons (port); 			 /* Port d ’ écoute */
	saddr.sin_addr.s_addr = INADDR_ANY ;	 /* écoute sur toutes les interfaces */

	if (socket_serveur == -1) {
		perror("socket_serveur");
		return -1;
	}

	if(bind(socket_serveur, (struct sockaddr*)& saddr, sizeof(saddr)) == -1){
		perror("bind socket_serveur");
		return -1;
	}

	if(listen(socket_serveur, 10) == -1){
		perror("listen socket_serveur");
		return -1;
	}
	return socket_serveur;
}