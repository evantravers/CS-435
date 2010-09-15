// CS 435 Evan Travers
int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout) {
	int e_sel = select(nfds, readfds, writefds, errorfds, timeout);
	if (e_sel==-1) {
		perror("the select has failed");
		exit(-1);
	} else {
		return e_sel;
	}
}