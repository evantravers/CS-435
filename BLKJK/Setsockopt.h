// CS 435 Evan Travers
int Setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len) {
	int e_sockopt = setsockopt(socket, level, option_name, option_value, option_len);
	if (e_sockopt==-1) {
		perror("the setsockopt has failed");
		exit(1);
	}
	else {
		return e_sockopt;
	}
}
