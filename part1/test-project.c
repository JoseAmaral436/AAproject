#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

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
	Info * la = (Info *) pa;
	Info * lb = (Info *) pb;

	return strcmp(la->key, lb->key);
}

int comparePriorities(const void *pa, const void *pb) {
	Info * la = (Info *) pa;
	Info * lb = (Info *) pb;

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

void rotateLeft(Node *node) {
//	printf("ROTATE LEFT\n");
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
float ROTATIONS_ADD = 0;
void add(char* key, int priority) {
	Info* info = malloc(sizeof(Info));
	info->key = key;
	info->numOfOcc = 0;
	info->priority = priority;
	Node* result = tsearch(info, &root, compareKeys);
	if (((Info*) (result->value))->numOfOcc != 0) {
		free(info->key);
		free(info);
	} else {
		while (result->parent != NULL
				&& comparePriorities(result->parent, result)) {
			if (result->parent->right == result) {
				rotateLeft(result->parent);
			} else {
				rotateRight(result->parent);
			}
			ROTATIONS_ADD++;
		}
		if (result->parent == NULL) {
			root = result;
		}
	}
	((Info*) (result->value))->numOfOcc++;
}

float ROTATIONS_DELETE = 0;
void delete(char* key) {
	Info * find = malloc(sizeof(Info));
	find->key = key;

	Node *toDelete = tfind(find, &root, compareKeys);
	if (toDelete != NULL) {
		while (toDelete->left != NULL || toDelete->right != NULL) {
			if (toDelete->left == NULL) {
				rotateLeft(toDelete);
			} else if (toDelete->right == NULL) {
				rotateRight(toDelete);
			} else if (comparePriorities(toDelete->right, toDelete->left)) {
				rotateRight(toDelete);
			} else {
				rotateLeft(toDelete);
			}
			ROTATIONS_DELETE++;
			if (toDelete == root) {
				root = toDelete->parent;
			}
		}
		tdelete(find, &root, compareKeys);
		free(((Info *) ((Node *) toDelete)->value)->key);
		free(((void *) ((Node *) toDelete)->value));
		free(toDelete);
	}
	free(find);
}

void findTest(char* key) {
	Info * find = malloc(sizeof(Info));
	find->key = key;

	tfind(find, &root, compareKeys);
	free(find);
}
int main(int argc, char **argv) {
	float TOTAL_ROTATIONS_ADD = 0;
	float TOTAL_ROTATIONS_DELETE = 0;
	float TOTAL_TIME_ADD = 0;
	float TOTAL_TIME_DELETE = 0;
	float TOTAL_TIME_FIND = 0;
	int arg = atoi(argv[1]);
	int MAX[12] = { 500, 1000, 5000, 10000, 15000, 20000, 50000, 100000, 200000, 500000, 750000, 1000000 };
//  				0	 1		2	  3		 4		5	   6	   7	  8		   9	  10	   11
	int ADDS = MAX[arg];
	int FINDS = ADDS;
	int DELETES = ADDS / 2;
	int ITERATIONS = 500;
	int iter;
	for (iter = 0; iter < ITERATIONS; iter++) {
		root = NULL;

		int seed;
		FILE *f = fopen("/dev/urandom", "r");
		fread(&seed, sizeof(seed), 1, f);
		fclose(f);
		srand(seed);

		int MASK = 0xffffff;
		const char* STR_FORMAT = "%06x";
		char* keys[MAX[arg]];
		int* priorities = malloc(sizeof(int) * MAX[arg]);
		int i;
		for (i = 0; i < MAX[arg]; i++) {
			keys[i] = malloc(sizeof(MASK));
			sprintf(keys[i], STR_FORMAT, rand() & MASK);
			priorities[i] = rand();
		}

		//ADD
		clock_t start = clock();
		for (i = 0; i < ADDS; i++) {
			add(keys[i], priorities[i]);
		}
		clock_t end = clock();
		double time = (double) (end - start) / CLOCKS_PER_SEC * 1000.0; //[ms]
		TOTAL_TIME_ADD += (time / ADDS);
		TOTAL_ROTATIONS_ADD += (ROTATIONS_ADD / ADDS);
		ROTATIONS_ADD = 0.0;

		//DELETE
		start = clock();
		for (i = 0; i < DELETES; i++) {
			delete(keys[i]);
		}
		end = clock();
		time = (double) (end - start) / CLOCKS_PER_SEC * 1000.0; //[ms]
		TOTAL_TIME_DELETE += (time / DELETES);
		TOTAL_ROTATIONS_DELETE += (ROTATIONS_DELETE / DELETES);
		ROTATIONS_DELETE = 0.0;

		//FIND
		start = clock();
		for (i = 0; i < FINDS; i++) {
			findTest(keys[i]);
		}
		end = clock();
		time = (double) (end - start) / CLOCKS_PER_SEC * 1000.0; //[ms]
		TOTAL_TIME_FIND += (time / FINDS);
	}
	free(root);
	printf("%d ADDS %d DELETES %d FINDS\n", ADDS, DELETES, FINDS);
	printf("TOTAL_TIME_ADD %f\n", TOTAL_TIME_ADD / ITERATIONS);
	printf("TOTAL_TIME_FIND %f\n", TOTAL_TIME_FIND / ITERATIONS);
	printf("TOTAL_TIME_DELETE %f\n", TOTAL_TIME_DELETE / ITERATIONS);
	printf("TOTAL_ROTATIONS_ADD %f\n", TOTAL_ROTATIONS_ADD / ITERATIONS);
	printf("TOTAL_ROTATIONS_DELETE %f\n", TOTAL_ROTATIONS_DELETE / ITERATIONS);
	return 0;
}
