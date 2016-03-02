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

void send_status(FILE *file_client, int code, const char *reason_phrase){
	fprintf(file_client,"HTTP/1.1 %i %s\r\n",code, reason_phrase);
}

void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body){
	send_status(client, code, reason_phrase);
	fprintf(client,"%s\r\n",message_body);
}
int parse_http_request(char *request_line, http_request *request){
	char* parse = strtok(request_line, " ");
	if(parse == NULL){
		return 1;
	}
	else if(strcmp(parse,"GET") == 0){
		request->method = HTTP_GET;
	}
	else {
		request->method = HTTP_UNSUPPORTED;
	}

	parse = strtok(NULL, " ");
	if(parse == NULL){
		return 1;
	}
	else{
		request->url = strdup(parse); // chemin
	}

	parse = strtok(NULL, " ");
	if(parse == NULL){
		return 1;
	}
	else if(strcmp(parse,"HTTP/1.1") == 0){
		request->major_version = 1;
		request->minor_version = 1;
	}
	else if(strcmp(parse,"HTTP/1.0") == 0){
		request->major_version = 1;
		request->minor_version = 0;
	}
	else{
		return 1;
	}

	parse = strtok(NULL, " ");
	if(parse != NULL){
		return 1;
	}
	return 0;
}

void goHTTP(FILE *file_client, char *http){
	const char * msg_bienvenue = "Bonjour, bienvenue sur notre serveur.\r\n";

	http_request request;
	int bad_request = parse_http_request(http, &request);


	if (bad_request)
	send_response ( file_client , 400 , " Bad Request " , " Bad request \r \n " );
	else if (request.method != HTTP_GET )
	send_response (file_client , 405 , " Method Not Allowed " , " Method Not Allowed \r \n " );
	else if ( strcmp (request.url , "/" ) == 0)
	send_response (file_client, 200, "OK", msg_bienvenue);
	else
	send_response ( file_client , 404 , " Not Found " , " Not Found \r \n " );
}



char *fgets_or_exit(char *buffer, int size, FILE *stream){
		fgets(buffer, size, stream); 
		if(buffer == NULL){
			printf("Déconnexion Client\n");
			fclose(stream);
			exit(0);
		}

		char* p = strchr(buffer,'\n');
		if(p){
			if(*(p - 1) == '\r'){
				p--;
			}
			*p = '\0';
		}
		return buffer;
}

void skip_headers(FILE * file_client){
	char read[8192];
	while(1){

		fgets_or_exit(read, 8192, file_client);
		if(read[0] == '\0'){
			return;
		}
	}
}
	char * rewrite_url(char *url);
