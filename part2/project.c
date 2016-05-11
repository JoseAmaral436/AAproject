#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sys/types.h"
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
/*	char* name;*/
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
int *result;

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
	printf("node %d [%d, %d] ", n->id, n->head, n->sdep);
	if (n->child != NULL){
		printf("with child %d ", n->child->id);
	}
	if (n->brother != NULL){
		printf("with brother %d ", n->brother->id);
	}
	if (n->slink != NULL) {
		printf("slink %d -> ", n->slink->id);
	}
	printf("\"");
	for (i = n->head; i < n->sdep; i++) {
		printf("%c", Ti[n->Ti][i]);
	}
	printf("\" ");
	Suffix* s = n->suffixes;
	while (s != NULL) {
		printf("[%d]", s->id);
		s = s->next;
	}
	printf(" = %d", n->suffixesSize);
	printf("\n");
}

void printST(Node *root) {
	printString(root);
	if (root->child == NULL) {
		if (root->brother != NULL) {
			printST(root->brother);
		}
	} else {
		printST(root->child);
		if (root->brother != NULL) {
			printST(root->brother);
		}
	}
}

void AddLeaf(Point *p, int i, int j) {
 	/*printf("AddLeaf pa: %d   pb: %d\n", (p->a)->id, (p->b)->id);*/
 	/*printf("AddLeaf ps: %d   p->b->head: %d   p->b->sdep: %d\n", p->s, p->b->head, p->b->sdep);*/
	if (p->a == p->b) {
		Node *node = initNode();
		/*printf("AddLeaf NORMAL j: %d p->b->sdep: %d  p->b->head: %d p->s: %d id: %d\n", j, p->b->sdep, p->b->head, p->s, node->id);*/
		node->Ti = i;
		node->head = j - p->s;/* - (p->b->sdep - p->b->head);*/
		node->sdep = ni[i] + 1;
		/*node->name = Ti[node->Ti];
		printf("namea: %s    sdepa: %d  nameb: %s    sdepb: %d   ps: %d\n", p->a->name, p->a->sdep, p->b->name, p->b->sdep, p->s);*/
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
		if ((previouslyNode != root) && (node->sdep - node->head != 1)) {
			previouslyNode->slink = node;
		}
		if (node->sdep - node->head - 1 == 1) {
			node->slink = root;
		}
		/*if (i == 0 && j == 0){
			previouslyAdded[1] = node;
		}
		else {
			previouslyAdded[0] = previouslyAdded[1];
			previouslyAdded[1] = node;
		}*/
		if (previouslySplit != NULL && ((previouslySplit->sdep - previouslySplit->head) > 1)) {
				previouslySplit->slink = p->a;
				previouslySplit = NULL;
		}
		previouslyNode = node;
		p->b = node;
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
		/*printf("AddLeaf SPLIT  j: %d splitid: %d\n", j, split->id);*/
		split->Ti = p->b->Ti;
		split->head = p->b->head;
		split->sdep = p->s + p->b->head;
		if (p->b->brother != NULL) {
			split->brother = p->b->brother;
			split->brother->hook = &(split->brother);
		}
		split->child = p->b;

		/*Node *aux = p->b;
		while(aux->brother != NULL){
			aux = aux->brother;
		}
		aux->brother = node;

		p->b->head = p->s;
		p->b->sdep = p->b->sdep - split->sdep;
		printf("p->b: %d   split: %d   (*(p->b->hook))->id: %d\n", p->b->id, split->id, (*(p->b->hook))->id);
		printf("!!!!!!!!!!!!!!rootchildid: %d\n", root->child->id);
		printf("pbid: %d   pbhookid: %d\n", p->b->id, (*(p->b->hook))->id);*/
		split->hook = p->b->hook;
		(*(p->b->hook)) = split;
		p->b->hook = &(split->child);
		/*printf("split: %d   splithookid: %d\n", split->id, (*(split->hook))->id);
		printf("pbid: %d   pbhookid: %d\n", p->b->id, (*(p->b->hook))->id);
		printf("!!!!!!!!!!!!!!!!rootchildid: %d\n", root->child->id);*/
		split->slink = root;
		Node *node = initNode();
		node->Ti = i;
		node->head = j - p->s;
		node->sdep = ni[i] + 1;
		/*node->name = Ti[node->Ti];*/
		p->b->brother = node;
		node->hook = &(p->b->brother);
		/*printf("SPLIT aux:  %d   p->b->id: %d    p->b->brother->id: %d \n", aux->id ,p->b->id, p->b->brother->id);
		previouslyAdded[0] = previouslyAdded[1];
		if (p->s == 1){
			previouslyAdded[0] = root;
			printf("pAdd0: %d   ", previouslyAdded[0]->id);
			printf("pAdd1: %d\n", previouslyAdded[1]->id);
		}
		previouslyAdded[1] = split;

		if (p->s == 1){
			p->b->slink = root;
			p->b->slink = previouslyAdded;
			previouslyAdded->slink = aux;
		}
		previouslyAdded = aux;
		if (aux->sdep - 1 == 1){
			aux->slink = root;
		}*/
		/*if (previouslySplit != NULL && ((previouslySplit->sdep - previouslySplit->head) > 1)) {*/
		if (previouslySplit != NULL){
			/*printf("SPLIT previouslySplitsdep-head:  %d   split-head:  %d previouslySplitid: %d splitid: %d\n", (previouslySplit->sdep - previouslySplit->head), (split->sdep - split->head), previouslySplit->id, split->id);*/
			/*if (((previouslySplit->sdep - previouslySplit->head) == ((split->sdep - split->head) + 1)) && ((previouslySplit->sdep - previouslySplit->head) > 1)){
				previouslySplit->slink = split;
			}
			else if ((((previouslySplit->sdep - previouslySplit->head) + 1) == (split->sdep - split->head)) && ((split->sdep - split->head) > 1)){
				split->slink = previouslySplit;
			}
			else if ((previouslySplit->sdep - previouslySplit->head) > 1){
				previouslySplit->slink = p->a;
			}*/
			/*printf("previouslySplit: %d   split: %d\n", previouslySplit->sdep - previouslySplit->head, split->sdep - split->head);*/
			if (((previouslySplit->sdep - previouslySplit->head) <= (split->sdep - split->head)) && (previouslySplit->sdep - previouslySplit->head) > 1){
				previouslySplit->slink = p->a;
			}
			else if ((previouslySplit->sdep - previouslySplit->head) > 1) {
				previouslySplit->slink = split;
			}
		}
		previouslySplit = split;
		if (previouslyNode != root){
			previouslyNode->slink = node;
		}
		p->a = split;
		previouslyNode = node;
		p->b = node;
	}
}

void Descend(Point *p) {
	p->s += 1;
	/*printf("Descend   p->s: %d   p->b->id: %d    (p->b)->sdep: %d p->b->head: %d\n", p->s, p->b->id, (p->b)->sdep, p->b->head);*/
	if ((p->b->head + p->s) == (p->b)->sdep) {
		p->a = p->b;
	}
}

bool DescendQ(Point *p, char c) {
	/*printf("DescendQ c: %c ps: %d pa: %d pb: %d\n", c, p->s, p->a->id, p->b->id);*/
	char curr;
	/*Testing if pointing to sentinel*/
	if ((p->a)->sdep == -1) { 
		p->a = p->a->child;
		p->b = p->a;
		/*printf("sentinel %c \n", c);*/
		return true;
	} else {
		/*printf("DescendQ p->b->head:%d    p->b->sdep: %d\n", p->b->head, p->b->sdep);*/
		if (p->a == p->b) {
			if (p->a->child == NULL) {
				return false;
			} else {
				if (p->b == p->a) {
					p->b = p->a->child;
				}
			}
			curr = Ti[p->b->Ti][p->s + p->b->head];
			/*printf("DescendQ curr1: %c p->bid: %d   ps: %d   head: %d\n", curr, p->b->id, p->s, p->b->head);*/
			if (curr == c) {
				p->b = p->a->child;
				return true;
			} else {
				Node * aux = p->b->brother;
				while (aux != NULL) {
					curr = Ti[aux->Ti][aux->head + p->s];
					/*if (aux->head + p->s > ni[aux->Ti]){
						printf("*************************************************************************\n");
						printf("DescendQ curr3: %c aux->Ti: %d   aux->head + p->s: %d\n", curr, aux->Ti, aux->head + p->s);
					}*/
					/*printf("DescendQ curr3: %c p->bid: %d   ps: %d   head: %d\n", curr, p->b->id, p->s, p->b->head);*/
					if (curr == c) {
						p->b = aux;
						return true;
					}
					aux = aux->brother;
				}
				p->b = p->a;
				return false;
			}
		} else {
			curr = Ti[p->b->Ti][p->b->head + p->s];
			/*printf("DescendQ curr2: %c p->bid: %d   ps: %d   head: %d\n", curr, p->b->id, p->s, p->b->head);*/
			return curr == c;
		}
	}
}

void SuffixLink(Point *p) {
	int head = p->s - 1;
	Node *phead = p->b;
	/*printf("SuffixLink BEFORE JUMP p->s: %d    p->aid: %d  p->bid: %d  p->a->slink: %d    phead: %d\n", p->s, p->a->id, p->b->id, p->a->slink->id, phead->id);*/
	p->a = p->a->slink;
	p->b = p->a;
	p->s = p->b->sdep - p->b->head;
	/*printf("SuffixLink AFTER JUMP p->s: %d    p->aid: %d   p->bid: %d\n", p->s, p->a->id, p->b->id);*/
	int index = phead->head + 1;
	while (p->b->sdep - p->b->head < head) {
		/*printf("SuffixLink before DescendQ with %d\n", index);*/
		/*printf("||||||||||||||||index: %d|||||||||||||||||||\n", index);*/
		DescendQ(p, Ti[phead->Ti][index]);
		
		/*if (head < p->b->sdep - p->b->head){
			printf("BREAK\n");
			break;
		}*/
		/*p->s = p->b->sdep - p->b->head;
		index = index + p->b->sdep - p->a->sdep;*/
		p->s++;
		index++;
		if ((p->b->head + p->s) == (p->b)->sdep) {
			p->a = p->b;
		}
		if (p->b->sdep - p->b->head == head) {
			p->a = p->b;
		}
		
		/*if ((p->b->head + p->s) == (p->b)->sdep) {
			p->a = p->b;
			
		}*/
		/*if((p->b->head + p->s) > (p->b)->sdep){
			printf("||||||||||||||||index: %d|||||||||||||||||||\n", index);
			p->s = (p->b->sdep - p->b->head);
			p->a = p->b;
			Node *aux = p->a;
			index = index + (p->b->sdep - p->b->head);
			printf("index: %d ********************0\n", index);
			while(DescendQ(p, Ti[phead->Ti][index])){
				printf("**********************index: %d ********************0\n", index);
				if ((p->b->sdep - p->b->head) > head){
					printf("******************************************1\n");
					break;
				}
				else if ((p->b->sdep - p->b->head) == head){
					if (Ti[p->b->Ti][p->b->sdep-1] != '-'){
						p->a = p->b;
					}
					printf("******************************************3\n");
					break;
				}
				printf("**********************p->bid: %d p->aid: %d auxid: %d\n", p->b->id, p->a->id, aux->id);
				p->s = p->s + (p->b->sdep - aux->sdep);
				index = index + (p->b->sdep - p->b->head);
				p->a = p->b;
				aux = p->a;
			}
			p->s = head;
		}*/
		/*printf("SuffixLink p->b->sdep: %d p->b->head: %d head: %d c: %c\n", p->b->sdep, p->b->head, head, Ti[phead->Ti][index]);*/
	}
	p->s = head;
	/*printf("After suffixjump: pbid: %d    p->s: %d\n", p->b->id, p->s);*/
	if (p->a == p->b && previouslySplit != NULL){
			previouslySplit->slink = p->a;
			previouslySplit = NULL;
	}
}

Node * buildST(int numberOfStrings) {
	root = initNode();
	previouslyNode = root;
	Node *sentinel = initNode();
	root->slink = sentinel;
	root->hook = &(sentinel->child);
	sentinel->child = root;
	sentinel->sdep = -1;
	Point *p = malloc(sizeof(Point));
	p->a = root;
	p->b = root;
	int i = 0;
	while (i < numberOfStrings) {
		p->s = 0;
		int j = 0;
		Ti[i][ni[i]] = '!';
		while (j <= ni[i]) {
			while (!DescendQ(p, Ti[i][j])) {
				AddLeaf(p, i, j);
				/*if (j != 0){
					printST(root->child);
					printf("---------------------\n");
				}*/
				SuffixLink(p);
			}
			Descend(p);
			j++;
		}
		Ti[i][ni[i]] = '-';
		i++;
		p->a = root;
		p->b = root;
		previouslySplit = NULL;
		previouslyNode = root;
	}
	free(p);
	free(sentinel);
	return root;
}

void cleanSuffx(Node* n) {
	Suffix* curr;
	while ((curr = n->suffixes) != NULL) {
		n->suffixes = n->suffixes->next;
		free(curr);
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


void DFS(Node* root) {
	push(root);
	while (stack != NULL) {
		Entry* curr = stack;
		if (curr->visited == 0) {
			if (curr->node->child != NULL) {
				push(curr->node->child);
				Node* brother = curr->node->child->brother;
				while (brother != NULL) {
					push(brother);
					brother = brother->brother;
				}
				curr->visited = 1;
			} else {
				curr = pop();
				curr->node->suffixes = initSuffix(curr->node->Ti);
				curr->node->suffixesSize++;
				free(curr);
			}
		} else {
			Entry* popped = pop();
			/*check child*/
			Suffix* childSuffx = curr->node->child->suffixes;
			curr->node->suffixes = initSuffix(childSuffx->id);
			curr->node->suffixesSize++;
			suffixesIdx[childSuffx->id] = iter;
			Suffix* currentSuffx = curr->node->suffixes;
			childSuffx = childSuffx->next;
			while (childSuffx != NULL) {
				Suffix* new = initSuffix(childSuffx->id);
				currentSuffx->next = new;
				curr->node->suffixesSize++;
				suffixesIdx[childSuffx->id] = iter;
				childSuffx = childSuffx->next;
				currentSuffx = currentSuffx->next;
			}
			/*check child brothers*/
			Node* brother = curr->node->child->brother;
			while (brother != NULL) {
				Suffix* brotherSuffx = brother->suffixes;
				while (brotherSuffx != NULL) {
					if (suffixesIdx[brotherSuffx->id] != iter) {
						Suffix* new = initSuffix(brotherSuffx->id);
						currentSuffx->next = new;
						curr->node->suffixesSize++;
						suffixesIdx[brotherSuffx->id] = iter;
						currentSuffx = currentSuffx->next;
					}
					brotherSuffx = brotherSuffx->next;
				}
				brother = brother->brother;
			}
			iter++;
			if (curr->node->suffixesSize > 1
					&& result[curr->node->suffixesSize - 2] < curr->node->sdep - curr->node->head) {
				result[curr->node->suffixesSize - 2] = curr->node->sdep - curr->node->head;
			}
			free(popped);
		}
	}
}

ssize_t getline(char **lineptr, size_t *n, FILE *stream);
char *strdup(const char *s);

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
		token[ni[i - 1]] = '-'; /*all strings have '\n' in the end beside the last*/
		if (i == numOfLines) {
			token[ni[i - 1] + 1] = '\0'; /*last string doesnt have '\n'*/
		}
		Ti[i - 1] = strdup(token);
	}
	root = buildST(numOfLines);

	result = (int*) malloc(sizeof(int) * (numOfLines - 1));
	for (i = 0; i < numOfLines - 1; i++) {
		result[i] = 0;
	}
	DFS(root);

	/*printST(root);*/
	
	/*fix if inconsistencia aula 29/03*/
	for (i = numOfLines-2; i > 0 ; i--) {
		if (result[i] > result[i-1]){
			result[i-1] = result[i];
		}
	}
	/*print result*/
	for (i = 0; i < numOfLines - 1; i++) {
		printf("%d ", result[i]);
	}
	printf("\n");
	clean(root);
	free(line);
	for (i = 0; i < numOfLines; i++) {
		free(Ti[i]);
	}
	free(Ti);
	free(ni);
	free(suffixesIdx);
	free(result);
	return 0;
}
