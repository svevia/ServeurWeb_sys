void skip_headers(FILE * file_client);
int parse_http_request(char *request_line, http_request *request);
char *fgets_or_exit(char *buffer, int size, FILE *stream);
void goHTTP(FILE *file_client, char *http);