//http://opendatastructures.org/ods-java/7_2_Treap_Randomized_Binary.html
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	struct node *parent;
} Node;

void *root;

int compareKeys(const void *pa, const void *pb) {
	Info * la = (Info *) ((Node *) pa)->value;
	Info * lb = (Info *) ((Node *) pb)->value;

	return strcmp(la->key, lb->key);
}

int comparePriorities(const void *pa, const void *pb) {
 Info * la = (Info *) ((Node *) pa)->value;
 Info * lb = (Info *) ((Node *) pb)->value;

 return la->priority < lb->priority;
}

void *tsearch(const void *key, void **rootp,
		int (*compar)(const void *, const void *)) {
	if (*rootp == NULL) {
		*rootp = malloc(sizeof(Node));
		((Node *) (*rootp))->value = key;
		((Node *) (*rootp))->left = NULL;
		((Node *) (*rootp))->right = NULL;
		((Node *) (*rootp))->parent = NULL;
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
					new->parent = aux;
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
					new->parent = aux;
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
					if (curr == *rootp) { // if root
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
							curr->right->parent = parent;
							parent->left = curr->right;
						} else {
							curr->left->parent = parent;
							parent->left = curr->left;
						}
					} else if (parentage == 1) {
						if (curr->left == NULL) {
							curr->right->parent = parent;
							parent->right = curr->right;
						} else {
							curr->left->parent = parent;
							parent->right = curr->left;
						}
					} else { // if root
						if (curr->left == NULL) {
							*rootp = curr->right;
						} else {
							*rootp = curr->left;
						}
						((Node *) (*rootp))->parent = NULL;
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
					if (curr == *rootp) {
						*rootp = min;
						((Node *) (*rootp))->parent = NULL;
						if (curr->right == min) {
							curr->left->parent = min;
							min->left = curr->left;
						} else {
							curr->right->parent = min;
							curr->left->parent = min;
							min->right = curr->right;
							min->left = curr->left;
							minParent->left = NULL;
						}
// 						printf("min  :%s\n", ((Info *)(((Node *)min)->value))->key);
// 						printf("curr  :%s\n", ((Info *)(((Node *)curr)->value))->key);
// 						printf("minParent  :%s\n", ((Info *)(((Node *)minParent)->value))->key);
// 						break;
					} else if (curr->right == min) {
						curr->left->parent = min;
						min->left = curr->left;
						if (parentage == 0) {
							parent->left = min;
						} else if (parentage == 1) {
							parent->right = min;
						}
						min->parent = parent;
					} else {
						curr->right->parent = min;
						curr->left->parent = min;
						min->right = curr->right;
						min->left = curr->left;
						minParent->left = NULL;
						if (parentage == 0) {
							parent->left = min;
						} else if (parentage == 1) {
							parent->right = min;
						}
						min->parent = parent;
					}
				}
				return parent; // RETURN PARENT of deleted node
			}
		}
	}
}
char* checkNull(Node* n) {
	if (n == NULL)
		return "NULL";
	return ((Info*) n->value)->key;
}
/*void printTree(Node * node) {
	if (node != NULL) {
		printf("key: %s    priority: %d   parent: %s left: %s right: %s\n",
				((Info*) (node->value))->key, ((Info*) (node->value))->priority,
				checkNull(node->parent), checkNull(node->left),
				checkNull(node->right));
//		printf("GOING LEFT\n");
		printTree(node->left);
//		printf("GOING RIGHT\n");
		printTree(node->right);
	}
}*/

int R_ROTATIONS = 0;
int L_ROTATIONS = 0;
void rotateLeft(Node *node) {
//	printf("ROTATE LEFT\n");
    L_ROTATIONS++;
	Node* aux = node->right;
	aux->parent = node->parent;
	if (aux->parent != NULL) {
		if (aux->parent->left == node) {
			aux->parent->left = aux;
		} else {
			aux->parent->right = aux;
		}
	}
	node->right = aux->left;
	if (node->right != NULL) {
		node->right->parent = node;
	}
	node->parent = aux;
	aux->left = node;
	if (node == root) {
		root = aux;
		((Node*) root)->parent = NULL;
	}

//	if (node->parent != NULL){ // node is not root
//		node->parent->right = node->left;
//		node->left = node->parent;
//		if (node->parent->parent == NULL){ // parent of node is root
//			root = node;
//		}
//		node->parent = node->parent->parent;
//		node->left->parent = node;
//		if (node->left->right != NULL){
//			node->left->right->parent = node->left->right;
//		}
//// 		node->parent->left = node->right;
//// 		node->right->parent = node->parent;
//// 		node->right = node->right->left;
//// 		if (node->right != NULL){
//// 			node->right->parent = node;
//// 		}
//// 		node->parent->left->left = node;
//// 		node->parent = node->parent->left;
//	}
//	else{ // node is root
//		Node *aux = node->right->left;
//		node->right->left = node;
//		node->parent = node->right;
//		node->right->parent = NULL;
//		node->right = aux;
//		if (aux != NULL){
//			aux->parent = node;
//		}
//		root = node->parent;
//	}

//	printTree(root);
//	printf("---------------\n");
}

void rotateRight(Node *node) {
//	printf("ROTATE RIGHT\n");
    R_ROTATIONS++;
    
	Node* aux = node->left;
	aux->parent = node->parent;
	if (aux->parent != NULL) {
		if (aux->parent->left == node) {
			aux->parent->left = aux;
		} else {
			aux->parent->right = aux;
		}
	}
	node->left = aux->right;
	if (node->left != NULL) {
		node->left->parent = node;
	}
	node->parent = aux;
	aux->right = node;
	if (node == root) {
		root = aux;
		((Node*) root)->parent = NULL;
	}

//	if (node->parent != NULL){ // node is not root
//
//	}
//	else{ // node is root
//		Node *aux = node->left->right;
//		node->left->right = node;
//		node->parent = node->left;
//		node->left->parent = NULL;
//		node->left = aux;
//		if (aux != NULL){
//			aux->parent = node;
//		}
//		root = node->left;
//	}
//	printTree(root);
//	printf("---------------\n");

}

void clean(Node * node) {
	if (node != NULL) {
		free(((Info *) (node->value))->key);
		free((void *) node->value);
		clean(node->left);
		clean(node->right);
		free(node);
	}
}

int main(){
   
    int seed;
    FILE *f = fopen("/dev/urandom", "r");
    fread(&seed, sizeof(seed), 1, f);
    fclose(f);
    srand(seed);
    
    int MAX = 10000;
    int MASK = 0xffffff;
    const char* STR_FORMAT = "%06x";
    char* keys[MAX];
    int priorities[MAX];
    int i;
    for (i = 0; i < MAX; i++){
        keys[i] = malloc(sizeof(MASK));
        sprintf(keys[i], STR_FORMAT, rand() & MASK);
        priorities[i] = rand();
    }    
    const char operations[3] = {'A', 'D', 'F'};
    
    Info* find = malloc(sizeof(Info));
    for (i = 0; i < MAX; i++){
        Node* result = NULL;
        //char op = operations[0];
        char op = operations[rand() % 3];
        switch (op) {
            case 'A':{
                //printf("");
                Info* info = malloc(sizeof(Info));
                info->key = keys[i];
                info->numOfOcc = 0;
                info->priority = priorities[i];
                result = tsearch(info, &root, compareKeys);
                if (((Info*) (result->value))->numOfOcc != 0) { // this key already exists
                    free(info->key);
                    free(info);
                } else { // new key (need to test heap property)
                    while (result->parent != NULL && comparePriorities(result->parent, result)) {
                        if (result->parent->right == result) {
                            rotateLeft(result->parent);
                        } else {
                            rotateRight(result->parent);
                        }
                    }
                    if (result->parent == NULL) {
                        root = result;
                    }
                }
                ((Info*) (result->value))->numOfOcc++;
                printf("%d\n", ((Info*) (result->value))->numOfOcc);
                break;
            }
            case 'F':
                find->key = keys[i];
                result = tfind(find, &root, compareKeys);
                if (result == NULL) {
                    printf("NULL\n");
                } else {
                    printf("%d\n", ((Info *) ((Node *) result)->value)->numOfOcc);
                }
                free(find->key);
                break;
            case 'D':
                find->key = keys[i];
                Node *toDelete = tfind(find, &root, compareKeys);
                if (toDelete == NULL) {
                    printf("NULL\n");
                    free(find->key);
                } else {
                    printf("%d\n", ((Info *) ((Node *) toDelete)->value)->numOfOcc); // NEED TO PRINT BEFORE DELETING
                    while(toDelete->left != NULL || toDelete->right != NULL){
                        if (toDelete->left == NULL){
                            rotateLeft(toDelete);
                        }
                        else if (toDelete->right == NULL){
                            rotateRight(toDelete);
                        }
                        else if (comparePriorities(toDelete->right, toDelete->left)){
                            rotateRight(toDelete);
                        }
                        else{
                            rotateLeft(toDelete);
                        }
                        if (toDelete == root){
                            root = toDelete->parent;
                        }
                    }
                    result = tdelete(find, &root, compareKeys);
                    free(((Info *) ((Node *) toDelete)->value)->key);
                    free(((void *) ((Node *) toDelete)->value));
                    free(toDelete);
                    free(find->key);
                }
                break;
        }

    }
    printf("R_ROTATIONS= %d  L_ROTATIONS= %d\n",  R_ROTATIONS,  L_ROTATIONS);
    clean(root);
    free(find);
    return 0;
}
/*int main() {

	int seed;
// 	uint16_t seed;
	FILE *f;
	f = fopen("/dev/urandom", "r");
	fread(&seed, sizeof(seed), 1, f);
	fclose(f);
//   printf("%u\n", seed);
	srand(seed);

	root = NULL;
	int numOfLines = 0;
	char *number = NULL;
	size_t size;
	if (getline(&number, &size, stdin) == -1) {
		printf("No number\n");
	} else {
		numOfLines = atoi(number);
	}
// 	int priorities[numOfLines];
// 	if (getline(&number, &size, stdin) == -1) {
// 		printf("No priorities\n");
// 	} else {
// 		printf("Priorities\n");
// 	}
	int i;
// 	priorities[0] = atoi(strtok(number, " "));
// 	for (i = 1; i < numOfLines; i++) {
// 		priorities[i] = atoi(strtok(NULL, " "));
// 	}
	free(number);
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
			info->priority = rand();
// 			info->priority = priorities[i - 1];
			strcpy(info->key, line);
			result = tsearch(info, &root, compareKeys);
			if (((Info*) (result->value))->numOfOcc != 0) { // this key already exists
				free(info->key);
				free(info);
			} else { // new key (need to test heap property)
				while (result->parent != NULL
						&& comparePriorities(result->parent, result)) {
					if (result->parent->right == result) {
						rotateLeft(result->parent);
					} else {
						rotateRight(result->parent);
					}
				}
				if (result->parent == NULL) {
					root = result;
				}
			}
			((Info*) (result->value))->numOfOcc++;
			printf("%d\n", ((Info*) (result->value))->numOfOcc);
			break;
		case 'F':
			memmove(line, line + 1, strlen(line)); //TODO TEST MEMORY LEAK
			memmove(line, line + 1, strlen(line));
			find->key = malloc(sizeof(line));
			strcpy(find->key, line);
			result = tfind(find, &root, compareKeys);
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
			Node *toDelete = tfind(find, &root, compareKeys); // CAN WE USE FIND IN CASE D ?
			if (toDelete == NULL) {
				printf("NULL\n");
				free(find->key);
			} else {
				printf("%d\n", ((Info *) ((Node *) toDelete)->value)->numOfOcc); // NEED TO PRINT BEFORE DELETING
				while(toDelete->left != NULL || toDelete->right != NULL){
					if (toDelete->left == NULL){
						rotateLeft(toDelete);
					}
					else if (toDelete->right == NULL){
						rotateRight(toDelete);
					}
					else if (comparePriorities(toDelete->right, toDelete->left)){
						rotateRight(toDelete);
					}
					else{
						rotateLeft(toDelete);
					}
					if (toDelete == root){
						root = toDelete->parent;
					}
				}
				result = tdelete(find, &root, compareKeys);
				free(((Info *) ((Node *) toDelete)->value)->key);
				free(((void *) ((Node *) toDelete)->value));
				free(toDelete);
				free(find->key);
			}
			break;
		}
	}
	printTree(root);
	clean(root);
	free(line);
	free(find);
	return 0;
}
*/