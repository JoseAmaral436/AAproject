#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/random.h>
#include <stdint.h>

typedef struct info {
	int numOfOcc;
	char *key;
	int priority;
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
	
	return strcmp(la->key, lb->key);
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
			cmp = compar(&key, &(aux->value));
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

/*Node * findmin(void **rootp, int (*compar)(const void *, const void *)) {
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
}*/

void * tfind(const void *key, void **rootp,
		int (*compar)(const void *, const void *)) {
	if (*rootp == NULL) {
		return NULL;
	} else {
		Node *aux = *rootp;
		int compare;
		while (1) {
		  
			compare = (compar(&key, &(aux->value)));
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
			compare = (compar(&key, &(curr->value)));
			if (compare < 0) {
				if (curr->left == NULL) {
					return NULL;
				} else {
					parent = curr;
					curr = curr->left;
					parentage = 0;
				}
			} else if (compare > 0) {
				if (curr->right == NULL) {
					return NULL;
				} else {
					parent = curr;
					curr = curr->right;
					parentage = 1;
				}
			} else {
				if ((curr->left == NULL) && (curr->right == NULL)) { // no CHILD
					if (curr == *rootp){ // if root
					  curr = NULL;
					  *rootp = NULL;
					  return NULL;
					}
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
					else{ // if root
					  if (curr->left == NULL) {
						  *rootp = curr->right;
					  } else {
						  *rootp = curr->left;
					  }
					}
					curr = NULL;
				} else { // 2 CHILDREN 
					Node *minParent = curr;
// 					printf("!!!!%s\n", ((Info *)(((Node *)minParent)->value))->key);
					Node *min = curr->right;
					while (min->left != NULL) {
					  minParent = min;
					  min = min->left;
					}
					//printf("min  :%s\n", ((Info *)(((Node *)min)->value))->key);
					if (curr == *rootp){
						*rootp = min;
						if (curr->right == min){
							min->left = curr->left;
						}
					  else{
							min->right = curr->right;
							min->left = curr->left;
							minParent->left = NULL;
						}
// 						printf("min  :%s\n", ((Info *)(((Node *)min)->value))->key);
// 						printf("curr  :%s\n", ((Info *)(((Node *)curr)->value))->key);
// 						printf("minParent  :%s\n", ((Info *)(((Node *)minParent)->value))->key);
// 						break;
					}
					else if (curr->right == min){
						min->left = curr->left;
					  if (parentage == 0) {
					    parent->left = min;
					  } else if (parentage == 1) {
					    parent->right = min;
					  }
					}
					else{
						min->right = curr->right;
						min->left = curr->left;
						minParent->left = NULL;
						if (parentage == 0) {
							parent->left = min;
					  } else if (parentage == 1) {
							parent->right = min;
					  }
					}
				}
				return parent; // RETURN PARENT of deleted node
			}
		}
	}
}

void clean(Node * node){
  if (node != NULL){
    free(((Info *)(node->value))->key);
    free((void *)node->value);
    clean(node->left);
    clean(node->right);
    free(node);
  }
}

void printTree(Node * node){
  if (node != NULL){
    printf("%s\n", ((Info*) (node->value))->key);
		printTree(node->left);
		printTree(node->right);
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
	free(number);
	int i;
	char *line = NULL;
	size_t sizeOfLine;
	Info * find = malloc(sizeof(Info));
	for (i = 1; i <= numOfLines; i++) {
		Node *result = NULL;
		getline(&line, &sizeOfLine, stdin);
		int lineSize = strlen(line);
		if (i != numOfLines) {
			line[lineSize - 1] = '\0'; //all strings have '\n' in the end beside the last
		} else {
			line[lineSize] = '\0'; // last string doesnt have '\n'
		}
		switch (line[0]) {
		case 'A':
			memmove(line, line + 1, strlen(line)); //TODO TEST MEMORY LEAK
			memmove(line, line + 1, strlen(line));
			Info * info = malloc(sizeof(Info));
			info->key = malloc(sizeof(line));
			info->numOfOcc = 0;
			strcpy(info->key, line);
			result = tsearch(info, &root, compar);
			if (((Info*) (result->value))->numOfOcc != 0){
			  free(info->key);
			  free(info);
			}
			((Info*) (result->value))->numOfOcc++;
			printf("%d\n", ((Info*) (result->value))->numOfOcc);
			break;
		case 'F':
			memmove(line, line + 1, strlen(line)); //TODO TEST MEMORY LEAK
			memmove(line, line + 1, strlen(line));
			find->key = malloc(sizeof(line)); 
			strcpy(find->key, line); 
			result = tfind(find, &root, compar);
			if (result == NULL) {
				printf("NULL\n");
			} else {
				printf("%d\n", ((Info *) ((Node *) result)->value)->numOfOcc);
			}
			free(find->key);
			break;
		case 'D':
			memmove(line, line + 1, strlen(line));
			memmove(line, line + 1, strlen(line));
			find->key = malloc(sizeof(line)); 
			strcpy(find->key, line);
			Node *toDelete = tfind(find, &root, compar); // CAN WE USE FIND IN CASE D ?
			if (toDelete == NULL) {
				printf("NULL\n");
				free(find->key);
			} else {
				printf("%d\n", ((Info *) ((Node *) toDelete)->value)->numOfOcc); // NEED TO PRINT BEFORE DELETING
				result = tdelete(find, &root, compar);
				free(((Info *) ((Node *) toDelete)->value)->key);
				free(((void *)((Node *) toDelete)->value));
				free(toDelete);
				free(find->key);
			}
			break;
		}
	}
	uint16_t randval;
  FILE *f;
  f = fopen("/dev/random", "r");
  fread(&randval, sizeof(randval), 1, f);
  fclose(f);
  printf("%u\n", randval);
// 	printTree(root);
	clean(root);
	free(line);
	free(find);
	return 0;
}
