void skip_headers(FILE * file_client);
int parse_http_request(char *request_line, http_request *request);
char *fgets_or_exit(char *buffer, int size, FILE *stream);
void goHTTP(FILE *file_client, char *http);
void send_status(FILE *file_client, int code, const char *reason_phrase);
void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body);
char * rewrite_url(char *url);
int check_and_open(const char * url, const char *document_root);