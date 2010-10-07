// CS 435 Evan Travers
int Read(int fd, void *buf, size_t count) {
	int msglength = read(fd, buf, count);
	if (msglength==-1) {
		perror("the read has failed");
		exit(1);
	} else {
		return msglength;
	}
}
