// CS 435 Evan Travers
#include <netdb.h>
#include <sys/socket.h> 

struct hostent *Gethostbyname(const char *name) {
	struct hostent e_ghn = gethostbyname(name);
	if (e_ghn == NULL) {
		perror("gethostbyname has failed");
		exit(1);
	} else {
		return e_ghn;
	}
}