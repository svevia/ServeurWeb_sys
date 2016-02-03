#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>


int socket_serveur;
 /* écoute sur toutes les interfaces */

void traitement_signal(){
	//int status;
	printf("signal ok \n");
	wait(NULL);
	//waitpid(0, &status, 0);
}

void initialiser_signaux(void){
	struct sigaction sa ;
	sa.sa_handler = traitement_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR){
		perror("signal");
	}

	if(sigaction(SIGCHLD, &sa, NULL) == -1){
		perror("sigaction(SIGCHLD)");
	}
}

int creer_serveur(int port){

	socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;				/* Socket ipv4 */
	saddr.sin_port = htons (port); 			 /* Port d ’ écoute */
	saddr.sin_addr.s_addr = INADDR_ANY ;	 /* écoute sur toutes les interfaces */

	if (socket_serveur == -1){
		perror("socket_serveur");
		return -1;
	}

	int optval = 1;
	if(setsockopt(socket_serveur, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1){
		perror("Can not set SO_REUSEADDR option");
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

