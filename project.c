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
		int cmp;
		while (aux != NULL) {
			cmp = compar(&(aux->value), &key);
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

Node * findmin(void **rootp, int (*compar)(const void *, const void *)) {
	if (*rootp == NULL) {
		return NULL;
	} else {
		Node *aux = *rootp;
		while (1) {
			if (aux->left == NULL) {
				return aux;
			} else {
				aux = aux->left;
			}
		}
	}
}

void * tfind(const void *key, void **rootp,
		int (*compar)(const void *, const void *)) {
	if (*rootp == NULL) {
		return NULL;
	} else {
		Node *aux = *rootp;
		int compare;
		while (1) {
			compare = (compar(&(aux->value), &key));
			if (compare < 0) {
				if (aux->left == NULL) {
					return NULL;
				} else {
					aux = aux->left;
				}
			} else if (compare > 0) {
				if (aux->right == NULL) {
					return NULL;
				} else {
					aux = aux->right;
				}
			} else {
				return aux;
			}
		}
	}
}

void * tdelete(const void *key, void **rootp,
		int (*compar)(const void *, const void *)) { //TODO IF REMOVE ROOT
	if (*rootp == NULL) {
		return NULL;
	} else {
		Node *parent = NULL;
		Node *curr = *rootp;
		int parentage = -1; //0 if left, 1 if right
		int compare;
		while (1) {
			compare = (compar(&(curr->value), &key));
			if (compare < 0) {
				if (curr->left == NULL) {
					printf("NULL\n");
					return NULL;
				} else {
					parent = curr;
					curr = curr->left;
					parentage = 0;
				}
			} else if (compare > 0) {
				if (curr->right == NULL) {
					printf("NULL\n");
					return NULL;
				} else {
					parent = curr;
					curr = curr->right;
					parentage = 1;
				}
			} else {
				if (curr == *rootp) {
					free((void *) curr->value);
					free(curr);
					curr = NULL;
					*rootp = NULL;
					return NULL;
				}
				if ((curr->left == NULL) && (curr->right == NULL)) { // no CHILD
					free((void *) curr->value);
					free(curr);
					if (parentage == 0) {
						parent->left = NULL;
					} else if (parentage == 1) {
						parent->right = NULL;
					}
					curr = NULL;
				} else if ((!(curr->left == NULL))
						!= (!(curr->right == NULL))) { //XOR , only one child
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
					free((void *) curr->value);
					free(curr);
					curr = NULL;
				} else { // 2 CHILDREN : find min of sub right tree;substitute value;delete curr TODO NEED TESTING
					Node *min;
					min = findmin(((void *) &(curr->right)), compar); //min has the node with min in subright tree
					strcpy(((Info * )curr->value)->label,
							((Info * )min->value)->label); // copy value from min to found node
					((Info *) curr->value)->numOfOcc =
							((Info *) min->value)->numOfOcc; // copy number of occ
					tdelete(min, ((void **) &(curr->right)), compar); // delete the old min node
				}
				return parent; // RETURN PARENT of deleted node
			}
		}
	}
}

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
		int lineSize = strlen(line);
		if (i != numOfLines) {
			line[lineSize - 1] = '\0'; //all strings have '\n' in the end beside the last
		} else {
			line[lineSize] = '\0'; // last string doesnt have '\n'
		}
		switch (line[0]) {
		case 'A':
			line = line + 2; //TODO TEST MEMORY LEAK
			Info *info = malloc(sizeof(Info));
			info->label = malloc(sizeof(line));
			strcpy(info->label, line);
			Node *result = tsearch(info, &root, compar);
			((Info*) (result->value))->numOfOcc++;
			printf("%d\n", ((Info*) (result->value))->numOfOcc);
			break;
		case 'F':
			memmove(line, line + 1, strlen(line)); //TODO TEST MEMORY LEAK
			memmove(line, line + 1, strlen(line));
			Info * find = malloc(sizeof(Info)); //this makes no
			find->label = malloc(sizeof(line)); //sense ... why should we do new Info* just to find
			strcpy(find->label, line); //How should we do it ???
			result = tfind(find, &root, compar);
			if (result == NULL) {
				printf("NULL\n");
			} else {
				printf("%d\n", ((Info *) ((Node *) result)->value)->numOfOcc);
			}
			free(find->label);
			free(find);
			break;
		case 'D':
			memmove(line, line + 1, strlen(line));
			memmove(line, line + 1, strlen(line));
			Info * delete = malloc(sizeof(Info)); //this makes no
			delete->label = malloc(sizeof(line)); //sense ... why should we do new Info* just to delete
			strcpy(delete->label, line); //How should we do it ???
			result = tfind(delete, &root, compar); // CAN WE USE FIND IN CASE D ?
			if (result == NULL) {
				printf("NULL\n");
			} else {
				printf("%d\n", ((Info *) ((Node *) result)->value)->numOfOcc); // NEED TO PRINT BEFORE DELETING
				result = tdelete(delete, &root, compar);
				free(delete->label);
				free(delete);
			}
			break;
		}
	}
	return 0;
}
