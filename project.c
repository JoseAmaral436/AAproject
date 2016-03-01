#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct info {
	int numOfOcc;
	char *label;
} Info;

typedef struct node {
	const void *value;
	struct node *left;
	struct node *right;
} Node;

void *root;

static int compar(const void *pa, const void *pb) {
	Info * la = (Info *) ((Node *) pa)->value;
	Info * lb = (Info *) ((Node *) pb)->value;

	return strcmp(la->label, lb->label);
}

void *tsearch(const void *key, void **rootp,
		int (*compar)(const void *, const void *)) {
	if (*rootp == NULL) {
		*rootp = malloc(sizeof(Node));
		((Node *) (*rootp))->value = key;
		((Node *) (*rootp))->left = NULL;
		((Node *) (*rootp))->right = NULL;
		return *rootp;
	} else {
		Node *aux = *rootp;
		while (aux != NULL) {
			int cmp = compar(&(aux->value), &key);
			if (cmp < 0) {
				if (aux->left == NULL) {
					Node *new = malloc(sizeof(Node));
					new->value = key;
					new->left = NULL;
					new->right = NULL;
					aux->left = new;
					return new;
				} else {
					aux = aux->left;
				}
			} else if (cmp > 0) {
				if (aux->right == NULL) {
					Node *new = malloc(sizeof(Node));
					new->value = key;
					new->left = NULL;
					new->right = NULL;
					aux->right = new;
					return new;
				} else {
					aux = aux->right;
				}
			} else {
				return aux;
			}
		}
		return aux;
	}
}

/*Node * tfind(char *s) {
 if (root == NULL) {
 return NULL;
 } else {
 Node *aux = root;
 while (1) {
 if ((strcmp(aux->value, s)) < 0) {
 if (aux->left == NULL) {
 printf("NULL\n");
 return NULL;
 } else {
 aux = aux->left;
 }
 } else if ((strcmp(aux->value, s)) > 0) {
 if (aux->right == NULL) {
 printf("NULL\n");
 return NULL;
 } else {
 aux = aux->right;
 }
 } else {
 printf("%d\n", aux->numOfOcc);
 return aux;
 }
 }
 }
 }

 Node * tdelete(char *s) { //TODO IF REMOVE ROOT
 if (root == NULL) {
 return NULL;
 } else {
 Node *parent = NULL;
 Node *curr = root;
 int parentage = -1; //0 if left, 1 if right
 while (1) {
 if ((strcmp(curr->value, s)) < 0) {
 if (curr->left == NULL) {
 printf("NULL\n");
 return NULL;
 } else {
 parent = curr;
 curr = curr->left;
 parentage = 0;
 }
 } else if ((strcmp(curr->value, s)) > 0) {
 if (curr->right == NULL) {
 printf("NULL\n");
 return NULL;
 } else {
 parent = curr;
 curr = curr->right;
 parentage = 1;
 }
 } else {
 printf("%d\n", curr->numOfOcc);
 if ((curr->left == NULL) && (curr->right == NULL)) { // no CHILD
 free(curr->value);
 free(curr);
 if (parentage == 0) {
 parent->left = NULL;
 } else if (parentage == 1) {
 parent->right = NULL;
 }
 } else if ((!(curr->left == NULL))
 != (!(curr->right == NULL))) { //XOR , only one child
 free(curr->value);
 free(curr);
 if (parentage == 0) {
 if (curr->left == NULL) {
 parent->left = curr->right;
 } else {
 parent->left = curr->left;
 }
 } else if (parentage == 1) {
 if (curr->left == NULL) {
 parent->right = curr->right;
 } else {
 parent->right = curr->left;
 }
 }
 } else { // TODO 2 CHILDREN

 }
 return parent; // RETURN PARENT of deleted node
 }
 }
 }
 }
 */
int main() {
	root = NULL;
	int numOfLines = 0;
	char *number = NULL;
	size_t size;
	if (getline(&number, &size, stdin) == -1) {
		printf("No number\n");
	} else {
		numOfLines = atoi(number);
	}
	printf("%d\n", numOfLines);
	int i;
	char *line = NULL;
	size_t sizeOfLine;
	for (i = 1; i <= numOfLines; i++) {
		getline(&line, &sizeOfLine, stdin);
		line[strlen(line) - 1] = '\0'; //TODO ASK IF ALL STRINGS HAVE SAME LENGTH
		switch (line[0]) {
		case 'A':
			line = line + 2; //TODO TEST MEMORY LEAK
			Info *info = malloc(sizeof(Info));
			info->label = malloc(sizeof(line));
			strcpy(info->label, line);
			Node *result = tsearch(info, &root, compar);
			((Info*)(result->value))->numOfOcc++;
			printf("%d\n", ((Info*)(result->value))->numOfOcc);
			break;
		case 'F':
			line = line + 2; //TODO TEST MEMORY LEAK
			//tfind(line);
			break;
		case 'D':
			line = line + 2; //TODO TEST MEMORY LEAK
			//tdelete(line);
			break;
		}
	}
	return 0;
}
