// CS 435 Evan Travers
int Write(int fd, const void *buf, size_t count) {
	int e_wri=write(fd, buf, count);
	if (e_wri==-1) {
		perror("write has failed");
		exit(-1);
	}
	else {
		return e_wri;
	}
}