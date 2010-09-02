// string comparison

#include <string.h>
#include <stdio.h>

char string[255];
// char string[255] = {"hello");

scanf("%s", string);
if (strcmp(string, "hello")) {
	printf("string does not match hello\n");
}
else {
	printf("you entered hello\n");
}

===============================================================

