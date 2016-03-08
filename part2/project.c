#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
  int Ti; 				/**< The value of i in Ti */
  int head;				/**< The path-label start at &(Ti[head]) */
  int sdep; 				/**< String-Depth */
  struct node *child; 			/**< Child */
  struct node *brother; 		/**< brother */
  struct node *slink; 			/**< Suffix link */
  struct node **hook; 			/**< What keeps this linked? */
} Node;





typedef struct point{
  Node *a; 			/**< node above */
  Node *b; 			/**< node bellow */
  int s; 			/**< String-Depth */
} Point;

void *root;

int main(){
	int numOfLines = 0;
	char *number = NULL;
	size_t size;
	if (getline(&number, &size, stdin) == -1) {
		printf("No number\n");
	} else {
		numOfLines = atoi(number);
	}
	free(number);
	int i;
	char *line = NULL;
	size_t sizeOfLine;
	for (i = 1; i <= numOfLines; i++) {
		getline(&line, &sizeOfLine, stdin);
		int lineSize = strlen(line);
		if (i != numOfLines) {
			line[lineSize - 1] = '\0'; //all strings have '\n' in the end beside the last
		} else {
			line[lineSize] = '\0'; // last string doesnt have '\n'
		}
		printf("%s\n", line);
	}
	return 0;
}