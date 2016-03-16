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
#include "mime.h"


const char *get_mime(const char *url){
	mime types[] = {
		{"png","image/png"},
		{"html","text/html"},
		{"css","text/css"},
		{"js","application/javascript"},
		{NULL,NULL}
	};

	char *extension;
	extension = strrchr(url,'.');
	int i;
	while(types[i].extension != NULL){
		if(strcmp(types[i].extension, extension+1) == 0){
			return types[i].mime;
		}
		i++;
	}
	return "text/plain";
}