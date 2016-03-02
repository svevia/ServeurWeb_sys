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

void goHTTP(FILE *file_client, char *http){

	printf(http);
	const char * msg_bienvenue = "Bonjour, bienvenue sur notre serveur.\r\n";

	char * req = strtok(http, " ");
	printf(req);
	if(strcmp(req,"GET") == 0){
		req = strtok(NULL, " ");

		if(strcmp(req,"/") == 0){
			req = strtok(NULL, " ");
			int cpt = strlen(req)-1;
			while(req[cpt] == '\r' || req[cpt] == '\n'){
				req[cpt] = '\0';
				cpt--;
			}
			if(strcmp(req,"HTTP/1.1\0") == 0 || strcmp(req,"HTTP/1.0\0") == 0){
				send_response(file_client,200,"OK",msg_bienvenue);
			}
			else{
				send_response(file_client,400,"Bad Request","Bad Request\r\n");
				}
			}

		else{
			send_response(file_client,404,"Not found","Not found\r\n");
		}

	}
	else{
		send_response(file_client,405,"Method Not Allowed","Method Not Allowed\r\n");
	}
}



char *fgets_or_exit(char *buffer, int size, FILE *stream){
		fgets(buffer, size, stream);
		if(buffer == NULL){
			printf("Déconnexion Client\n");
			fclose(stream);
			exit(0);
		}
		return buffer;
}

int parse_http_request(char *request_line, http_request *request){
	char * parse = strtok(request_line, " ");
	if(strcmp(parse,"GET") == 0){
		request->method = HTTP_GET;
	}
	else{
		printf("Bad method request\n");
		return 0;
	}
	parse = strtok(parse, " ");
	if(strcmp(parse,"/") == 0){
		request->url = parse;
	}
	else{
		printf("Bad url request\n");
		return 0;
	}
	parse = strtok(parse, " ");
	if(strcmp(parse,"HTTP/1.1") == 0){
		request->major_version = 1;
	}
	else if(strcmp(parse,"HTTP/1.0") == 0){
		request->minor_version = 0;
	}
	else{
		request->method = HTTP_UNSUPPORTED;
		return 0;
	}
	return 1;
}

void skip_headers(FILE * file_client){
	char read[80];
	while(1){

		fgets_or_exit(read, 80, file_client);

		if(strcmp(read,"\n") == 0 || strcmp(read,"\r\n") == 0){
			break;
		}
	}
}