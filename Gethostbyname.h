// CS 435 Evan Travers
#include <netdb.h>
#include <sys/socket.h> 

struct hostent *Gethostbyname(const char *name) {
	int e_ghn = gethostbyname((int)name);
	if (e_ghn == NULL) {
		perror("gethostbyname has failed");
		exit(-1);
	} else {
		return e_hgn
	}
}