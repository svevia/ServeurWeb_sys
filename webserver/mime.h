typedef struct{
	const char *extension;
	const char *mime;
}mime;

const char *get_mime(const char *url);