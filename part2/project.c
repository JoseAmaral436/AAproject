#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct suffix {
	int id;
	struct suffix * next;
} Suffix;

typedef struct node {
	int id;
	int Ti; /**< The value of i in Ti */
	int head; /**< The path-label start at &(Ti[head]) */
	int sdep; /**< String-Depth */
	struct node *child; /**< Child */
	struct node *brother; /**< brother */
	struct node *slink; /**< Suffix link */
	struct node **hook; /**< What keeps this linked? */
	char* name;
	Suffix* suffixes;
	int suffixesSize;
} Node;

typedef struct point {
	Node *a; /**< node above */
	Node *b; /**< node bellow */
	int s; /**< String-Depth */
} Point;

int *ni;
char **Ti;
int id = 0;
Node *root = NULL;
Node *previouslyNode = NULL;
Node *previouslySplit = NULL;
int j;

Node* initNode() {
	Node* node = malloc(sizeof(Node));
	node->id = id;
	id++;
	node->Ti = 0;
	node->sdep = 0;
	node->head = 0;
	node->brother = NULL;
	node->child = NULL;
	node->hook = NULL;
	node->slink = NULL;
	node->suffixes = NULL;
	node->suffixesSize = 0;
	return node;
}

void printString(Node *n) {
	int i;
	int index = n->head + n->sdep;
//	printf("node %d [%d, %d] ", n->id, n->head, index - 1);
//	if (n->slink != NULL) {
//		printf("slink %d -> ", n->slink->id);
//	}
// 	if (n->child != NULL){
// 		printf("child %d ->  ", n->child->id);
// 	}
// 	if (n->brother != NULL){
// 		printf("brother %d ->  ", n->brother->id);
// 	}
// 	printf("n->head: %d    index: %d\n", n->head, index);
	if (n->id == 0)
		printf("ROOT");
	else {
		for (i = n->head; i < index; i++) {
			printf("%c", Ti[n->Ti][i]);
		}
	}
	Suffix* s = n->suffixes;
	while (s != NULL) {
		printf("[%d]", s->id);
		s = s->next;
	}
	printf(" = %d", n->suffixesSize);

// 	printf(" Ti: %d   sdep: %d   head: %d name: %s", n->Ti, n->sdep, n->head, n->name);
// 		printf(" %s ", (*(n->hook))->name);
	printf("\n");

}

void printST(Node *root) {
	printString(root);
	if (root->child == NULL) {
		if (root->brother != NULL) {
// 			printf("BROTHER\n");
			printST(root->brother);
		}
	} else {
// 		printf("CHILD\n");
		printST(root->child);
		if (root->brother != NULL) {
// 			printf("BROTHER\n");
			printST(root->brother);
		}
	}
}

void AddLeaf(Point *p, int i, int jj) {
// 	printf("pa: %d   pb: %d\n", (p->a)->id, (p->b)->id);
// 	printf("ps: %d\n",p->s);
	if (p->s == 0) {
		Node *node = initNode();
		node->Ti = i;
		node->head = jj;
		node->sdep = ni[i] - jj + 1;
		node->name = Ti[node->Ti];
		// 	printf("namea: %s    sdepa: %d  nameb: %s    sdepb: %d   ps: %d\n", p->a->name, p->a->sdep, p->b->name, p->b->sdep, p->s);
		Node *ancestor = p->a;
		if (ancestor->child == NULL) {
			ancestor->child = node;
			node->hook = &(ancestor->child);
		} else {
			ancestor = ancestor->child;
			while (ancestor->brother != NULL) {
				ancestor = ancestor->brother;
			}
			ancestor->brother = node;
			node->hook = &(ancestor->brother);
		}
		if ((previouslyNode != root) && (node->sdep != 1)) {
			previouslyNode->slink = node;
		}
		if (node->sdep - 1 == 1) {
			node->slink = root;
		}
// 		if (i == 0 && j == 0){
// 			previouslyAdded[1] = node;
// 		}
// 		else {
// 			previouslyAdded[0] = previouslyAdded[1];
// 			previouslyAdded[1] = node;
// 		}
		previouslyNode = node;
	}

	/*if (p->a->child == p->b){
	 Node *split = initNode();
	 split->Ti = p->b->Ti;
	 split->head = p->b->head;
	 split->sdep = 0; //TODO
	 split->child = p->b;
	 split->brother = p->b->brother;
	 p->a->child = split;
	 p->b->head += p->s;
	 p->b->sdep -= p->s;
	 (*(p->b->hook)) = split;
	 Node *aux = initNode();
	 aux->Ti = i;
	 aux->head = j;
	 aux->sdep = ni[i] - j;
	 aux->name = Ti[aux->Ti];
	 p->b->brother = aux;
	 }*/
	else {
		Node *split = initNode();
		split->Ti = p->b->Ti;
		split->head = p->b->head;
		split->sdep = p->s;
// 		printf ("antes: j: %d\n", j);
		j -= split->sdep;
// 		printf ("depois: j: %d  splitsdep: %d\n", j, split->sdep);
		if (p->b->brother != NULL) {
			split->brother = p->b->brother;
			split->brother->hook = &(split->brother);
// 			printf("SPLIT id:  %d   p->b->id: %d    split->brother->id: %d\n", split->id ,p->b->id, split->brother->id, p->b->brother->id);
		}
		split->child = p->b;

// 		Node *aux = p->b;
// 		while(aux->brother != NULL){
// 			aux = aux->brother;
// 		}
// 		aux->brother = node;

		p->b->head = p->b->head + p->s;
		p->b->sdep = p->b->sdep - p->s;
// 		printf("p->b: %d   split: %d   (*(p->b->hook))->id: %d\n", p->b->id, split->id, (*(p->b->hook))->id);
// 		printf("!!!!!!!!!!!!!!rootchildid: %d\n", root->child->id);
// 		printf("pbid: %d   pbhookid: %d\n", p->b->id, (*(p->b->hook))->id);
		split->hook = p->b->hook;
		(*(p->b->hook)) = split;
		p->b->hook = &(split->child);
// 		printf("split: %d   splithookid: %d\n", split->id, (*(split->hook))->id);
// 		printf("pbid: %d   pbhookid: %d\n", p->b->id, (*(p->b->hook))->id);
// 		printf("!!!!!!!!!!!!!!!!rootchildid: %d\n", root->child->id);
		split->slink = root;
		Node *node = initNode();
		node->Ti = i;
		node->head = jj;
		node->sdep = ni[i] - jj + 1;
		node->name = Ti[node->Ti];
		p->b->brother = node;
		node->hook = &(p->b->brother);
// 		printf("SPLIT aux:  %d   p->b->id: %d    p->b->brother->id: %d \n", aux->id ,p->b->id, p->b->brother->id);
// 		previouslyAdded[0] = previouslyAdded[1];
// 		if (p->s == 1){
// 			previouslyAdded[0] = root;
// 			printf("pAdd0: %d   ", previouslyAdded[0]->id);
// 			printf("pAdd1: %d\n", previouslyAdded[1]->id);
// 		}
// 		previouslyAdded[1] = split;

// 		if (p->s == 1){
// 			p->b->slink = root;
// 			p->b->slink = previouslyAdded;
// 			previouslyAdded->slink = aux;
// 		}
// 		previouslyAdded = aux;
// 		if (aux->sdep - 1 == 1){
// 			aux->slink = root;
// 		}
		if (previouslySplit != NULL && previouslySplit->sdep != 1) {
			previouslySplit->slink = split;
		}
		previouslySplit = split;
		previouslyNode->slink = node;
		previouslyNode = node;
	}
}

void SuffixLink(Point *p) {
	// 	if (p->s != 0){ // if not at start of string AKA if in internal node
	// 		p->a = p->a->slink;
	// 		p->b = p->a;
	// 		p->s--;
	// 	}
	// 	else{
	p->a = (p->a)->slink;
	// 		if (p->a->child != NULL){
	// 			p->b = p->a->child;
	// 		}
	// 		else {
	p->b = p->a;
	// 		}
	p->s = p->a->sdep;
	// 		printf("SUFFIXLINK pa: %d    pb: %d\n", p->a->id, p->b->id);
	// 		p->s = (p->a)->sdep;
	// 		printf("PS==0   %d  FOLLOWED SUFFIX LINK\n", p->s);
	// 	}
}

void Descend(Point *p) {
	p->s += 1;
// 	printf("ps: %d  pbsdep: %d   pa: %d pb: %d\n", p->s, p->b->sdep, p->a->id, p->b->id);
	if (p->s == (p->b)->sdep) {
// 		printf("DESCEND NODE\n");
		p->a = p->b;
// 		if (p->a->child != NULL){
// 			p->b = p->a->child;
// 		}
		p->s = 0;
	}
// 	printf("DESCEND\n");
}

bool DescendQ(Point *p, char c) {
// 	printf("c: %c\n", c);
	char curr;
	if ((p->a)->sdep == -1) { // Testing if pointing to sentinel
		p->a = p->a->child;
		p->b = p->a;
// 		p->s = -1;
// 		printf("sentinel %c \n", c);
		return true;
	} else {
// 		printf("ps: %d pa: %d pb: %d\n", p->s, p->a->id, p->b->id);
		if (p->s == 0) {
			if (p->a->child == NULL) {
				return false;
			} else {
				p->b = p->a->child;
			}
			curr = Ti[p->b->Ti][p->b->head + p->s];
// 			curr = Ti[p->b->Ti][p->s];
// 			printf("curr1: %c p->bid: %d   ps: %d   head: %d\n", curr, p->b->id, p->s, p->b->head);
			if (curr == c) {
				p->b = p->a->child;
				return true;
			} else {
				Node * aux = p->b->brother;
				while (aux != NULL) {
					curr = Ti[aux->Ti][aux->head];
// 					printf("curr3: %c p->bid: %d   ps: %d   head: %d\n", curr, p->b->id, p->s, p->b->head);
					if (curr == c) {
						p->b = aux;
// 							printf("match\n");
						return true;
					}
					aux = aux->brother;
				}
				return false;
			}
		} else {
			curr = Ti[p->b->Ti][p->b->head + p->s];
// 			curr = Ti[p->b->Ti][p->s];
// 			printf("curr2: %c p->bid: %d   ps: %d   head: %d\n", curr, p->b->id, p->s, p->b->head);
			return curr == c;
		}

//	// TODO UPDATE p->b
//	printf("c: %c\n", Ti[(p->b)->Ti][p->s]);
// 	if ((p->a)->child == NULL){
// 		return false;
// 	}
// 	else{
// 		//printf("c: %c\n", Ti[(p->b)->Ti][p->s]);
// 	}
// 		return false;
	}
}

Node * buildST(int numberOfStrings) {
	root = initNode();
	previouslyNode = root;
	Node *sentinel = initNode();
	root->slink = sentinel;
	root->hook = &(sentinel->child);
	root->name = "root";
	sentinel->child = root;
	sentinel->sdep = -1;
	sentinel->name = "sentinel";
	Point *p = malloc(sizeof(Point));
	p->a = root;
	p->b = root;
	int i = 0;
	while (i < numberOfStrings) {
		p->s = 0;
		j = 0;
		Ti[i][ni[i]] = '!';
// 		if (i == 0){
// 			Ti[i][ni[i]]= '!';
// 		}
// 		else if (i == 1){
// 			Ti[i][ni[i]]= '$';
// 		}
		while (j <= ni[i]) {
			while (!DescendQ(p, Ti[i][j])) {
// 				if (j != 0){
// 					printf("|||||||||||||||||||||ANTES|||||||||||||||||||\n");
// 					printf("---- p->aID: %d ----  p->bID: %d  ------  p->s: %d  ------\n", p->a->id, p->b->id, p->s);
// 					printST(root->child);
// 					
// 				}
				AddLeaf(p, i, j);
				SuffixLink(p);
// 				if (j != 0){
// 					printST(root->child);
// 					printf("-----------    j: %d   ----------\n", j);
// 				}
// 				if (j != 0){
// 					printf("|||||||||||||||||||||DEPOIS|||||||||||||||||||\n");
// 					printf("---- p->aID: %d ----  p->bID: %d  ------  p->s: %d  ------\n", p->a->id, p->b->id, p->s);
// 					printST(root->child);
// 				}

			}
			Descend(p);
			j++;
		}
		Ti[i][ni[i]] = '-';
		i++;
	}
	free(p);
	free(sentinel);
	return root;
}
void cleanSuffx(Node* n){
	Suffix* curr;
	while ((curr = n->suffixes) != NULL) {
		n->suffixes = n->suffixes->next;
	    free (curr);
	}
}

void clean(Node * node) {
	if (node != NULL) {
		clean(node->child);
		clean(node->brother);
		cleanSuffx(node);
		free(node);
	}
}
Suffix* initSuffix(int id) {
	Suffix* s = (Suffix*) malloc(sizeof(Suffix));
	s->id = id;
	s->next = NULL;
	return s;
}

int *suffixesIdx;
int iter = 1;
typedef struct entry {
	int visited;
	Node* node;
	struct entry* next;
} Entry;
Entry* stack = NULL;
void push(Node* n) {
	Entry* new = (Entry*) malloc(sizeof(Entry));
	new->visited = 0;
	new->node = n;
	new->next = stack;
	stack = new;

}
Entry* pop() {
	Entry* pop = stack;
	stack = stack->next;
	return pop;
}
//void printIndexes() {
//	printf("Suffixes-Indexes [");
//	int i;
//	for (i = 0; i < 3; i++) {
//		printf("%d,", suffixesIdx[i]);
//	}
//	printf("]\n");
//}
void DFS(Node* root) {
	push(root);
	while (stack != NULL) {
		Entry* curr = stack;
//		printString(curr->node);
		if (curr->visited == 0) {
			if (curr->node->child != NULL) {
				push(curr->node->child);
//				printString(stack->node);
				Node* brother = curr->node->child->brother;
				while (brother != NULL) {
					push(brother);
//					printString(stack->node);
					brother = brother->brother;
				}
				curr->visited = 1;
			} else {
				curr = pop();
				curr->node->suffixes = initSuffix(curr->node->Ti);
				curr->node->suffixesSize++;
				free(curr);
//				printString(curr->node);
			}
		} else {
			Entry* popped = pop();
//			printIndexes();
			//check child
			Suffix* childSuffx = curr->node->child->suffixes;
			curr->node->suffixes = initSuffix(childSuffx->id);
			curr->node->suffixesSize++;
			suffixesIdx[childSuffx->id] = iter;
//			printString(curr->node);
			Suffix* currentSuffx = curr->node->suffixes;
			childSuffx = childSuffx->next;
			while (childSuffx != NULL) {
				Suffix* new = initSuffix(childSuffx->id);
				currentSuffx->next = new;
				curr->node->suffixesSize++;
				suffixesIdx[childSuffx->id] = iter;
				childSuffx = childSuffx->next;
//				if (childSuffx != NULL) {
					currentSuffx = currentSuffx->next;
//				}
//				printString(curr->node);
			}
//			printIndexes();
			//check child brothers
			Node* brother = curr->node->child->brother;
			while (brother != NULL) {
//				printString(brother);
				Suffix* brotherSuffx = brother->suffixes;
				while (brotherSuffx != NULL) {
					if (suffixesIdx[brotherSuffx->id] != iter) {
						Suffix* new = initSuffix(brotherSuffx->id);
						currentSuffx->next = new;
						curr->node->suffixesSize++;
						suffixesIdx[brotherSuffx->id] = iter;
//						if (brotherSuffx->next != NULL) {
							currentSuffx = currentSuffx->next;
//						}
					}
					brotherSuffx = brotherSuffx->next;
//					printString(curr->node);
				}
				brother = brother->brother;
//				printIndexes();
//				if (brother != NULL && currentSuffx->next != NULL) {
//					currentSuffx = currentSuffx->next;
//				}
			}
			iter++;
			free(popped);
//			printString(curr->node);
//			printIndexes();
		}

	}
//	printString(root);
//	return NULL;
}

//Suffix* DFS(Node* root) {
//	push(root, NULL);
//	while (stack != NULL) {
//		Entry* curr = pop();
//		printString(curr->node);
//			if (curr->node->child != NULL) {
//				iter++;
//				push(curr->node->child, curr);
//				printString(stack->node);
//				Node* brother = curr->node->child->brother;
//				while (brother != NULL) {
//					push(brother, curr);
//					printString(stack->node);
//					brother = brother->brother;
//				}
//			} else {
//				curr->node->suffixes = initSuffix(curr->node->Ti);
//				printString(curr->node);
//
//				Suffix* currentSuffx = curr->node->suffixes;
//
//				Suffix* parentSuffx = curr->parent->node->suffixes;
//				if (parentSuffx == NULL) {
//					curr->parent->node->suffixes = initSuffix(currentSuffx->id);
//					parentSuffx = curr->parent->node->suffixes;
//					suffixesIdx[currentSuffx->id] = iter;
//					currentSuffx = currentSuffx->next;
//					while (currentSuffx != NULL) {
//						Suffix* new = initSuffix(currentSuffx->id);
//						suffixesIdx[currentSuffx->id] = iter;
//						currentSuffx = currentSuffx->next;
//						parentSuffx->next = new;
//						if (currentSuffx != NULL)
//							parentSuffx = parentSuffx->next;
//					}
//					printString(curr->node);
//					printString(curr->parent->node);
//				} else {
//					while (currentSuffx != NULL) {
//						if (suffixesIdx[currentSuffx->id] != iter) {
//							Suffix* new = initSuffix(currentSuffx->id);
//							suffixesIdx[currentSuffx->id] = iter;
//							if (parentSuffx->next != NULL)
//								parentSuffx = parentSuffx->next;
//							parentSuffx->next = new;
//
//						}
//						currentSuffx = currentSuffx->next;
//						if (currentSuffx != NULL)
//							parentSuffx = parentSuffx->next;
//					}
//					printString(curr->node);
//					printString(curr->parent->node);
//				}
//			}
//
//	}
//	printString(root);
//	return NULL;
//}

//Suffix* DFS(Node *node) {
////	printString(node);
//	if (node->child == NULL) { 		//LEAF-NODE
//		node->suffixes = initSuffix(node->Ti);
//		return node->suffixes;
//
//	} else {							//INTERNAL-NODE
//		printString(node);
//		Suffix* childSuffx = DFS(node->child);
//		node->suffixes = initSuffix(childSuffx->id);
//		Suffix* curr = node->suffixes;
//		suffixesIdx[childSuffx->id] = iter;
//		childSuffx = childSuffx->next;
//		while (childSuffx != NULL) {
//			Suffix* new = initSuffix(childSuffx->id);
//			suffixesIdx[childSuffx->id] = iter;
//			childSuffx = childSuffx->next;
//			curr->next = new;
//			if (childSuffx != NULL)
//				curr = curr->next;
//		}
//		printString(node);
//		Node* aux = node->child->brother;
//		while (aux != NULL) {
//			Suffix* bs = DFS(aux);
//			while (bs != NULL) {
//				printf("suffixes-idx: ");
//				int i;
//				for (i = 0; i < 3; i++) {
//					printf("[%d]", suffixesIdx[i]);
//				}
//				printf("\n");
//				if (suffixesIdx[bs->id] != iter) {
//					Suffix* new = initSuffix(bs->id);
//					suffixesIdx[bs->id] = iter;
//					curr->next = new;
//				}
//				bs = bs->next;
//				if (bs != NULL)
//					curr = curr->next;
//			}
//			aux = aux->brother;
//		}
//		iter++;
//		printString(node);
//		return node->suffixes;
//
//	}
//}

int main() {
	int numOfLines = 0;
	char *number = NULL;
	size_t size;
	if (getline(&number, &size, stdin) == -1) {
		printf("No number\n");
	} else {
		numOfLines = atoi(number);
	}
	suffixesIdx = (int*) malloc(sizeof(int) * numOfLines);
	free(number);
	int i;
	char *line = NULL;
	size_t sizeOfLine;
	char* token;
	Ti = (char**) malloc(sizeof(char*) * numOfLines);
	ni = (int *) malloc(sizeof(int) * numOfLines);
	for (i = 1; i <= numOfLines; i++) {
		getline(&line, &sizeOfLine, stdin);
		token = strtok(line, " ");
		ni[i - 1] = atoi(token);
		token = strtok(NULL, " ");
		token[ni[i - 1]] = '-'; //all strings have '\n' in the end beside the last
		if (i == numOfLines) {
			token[ni[i - 1] + 1] = '\0'; // last string doesnt have '\n'
		}
// 		Ti[i-1] = malloc(sizeof(char) * (ni[i-1] + 1));
		Ti[i - 1] = strdup(token);
	}
	root = buildST(numOfLines);
//	printST(root->child);

	DFS(root);
	printST(root);

	clean(root);
	free(line);
	for (i = 0; i < numOfLines; i++) {
		free(Ti[i]);
	}
	free(Ti);
	free(ni);
	free(suffixesIdx);
	return 0;
}
