#define _XOPEN_SOURCE 700
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
 	/*printf("AddLeaf pa: %d   pb: %d\n", (p->a)->id, (p->b)->id);
 	printf("AddLeaf ps: %d   p->b->head: %d   p->b->sdep: %d\n", p->s, p->b->head, p->b->sdep);*/
	if (p->a == p->b) {
		Node *node = initNode();
		printf("AddLeaf NORMAL j: %d p->b->sdep: %d  p->b->head: %d p->s: %d id: %d\n", j, p->b->sdep, p->b->head, p->s, node->id);
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
/*			if (previouslySplit->sdep - previouslySplit->head == p->a->sdep - p->a->head + 1){
				previouslySplit->slink = p->a;
				previouslySplit = NULL;
			}
			else{
				TODO
				previouslySplit->slink = p->a->parent;
				previouslySplit = NULL;
			}*/
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
		printf("AddLeaf SPLIT   splitid: %d\n", split->id);
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
	/*printf("SuffixLink BEFORE JUMP p->s: %d    p->aid: %d    p->a->slink: %d    phead: %d\n", p->s, p->a->id, p->a->slink->id, phead->id);*/
	p->a = p->a->slink;
	p->b = p->a;
	p->s = p->a->sdep - p->a->head;
	/*printf("SuffixLink AFTER JUMP p->s: %d    p->aid: %d   p->bid: %d\n", p->s, p->a->id, p->b->id);*/
	int index = phead->head + 1;
	if (p->b->sdep < head) {
		/*printf("SuffixLink before DescendQ with %d\n", index);*/

		DescendQ(p, Ti[phead->Ti][index]);
		p->s = head;
		
		if ((p->b->head + p->s) == (p->b)->sdep) {
			p->a = p->b;
			
		}
		if((p->b->head + p->s) > (p->b)->sdep){
			/*printf("|||||||||||||||||||||||||||||||||||\n");
			int x = 0;*/
			p->s = (p->b->sdep - p->b->head);
			p->a = p->b;
			Node *aux = p->a;
			index = index + (p->b->sdep - p->b->head);
			bool desc = DescendQ(p, Ti[phead->Ti][index]);
			while(desc){
				/*printf("**********************index: %d ********************0\n", index);*/
				/*if ((p->b->sdep - p->b->head) == head){
					printf("******************************************1\n");
					if (Ti[p->b->Ti][p->b->sdep-1] != '-'){
						p->a = p->b;
					}
					x = 1;
					break;
				}
				else if ((p->b->sdep - p->b->head) > head){
					printf("******************************************2\n");
					break;
				}*/
				if ((p->b->sdep - p->b->head) > head){
					printf("******************************************2\n");
					break;
				}
				else if ((p->b->sdep - p->b->head) == head){
					if (Ti[p->b->Ti][p->b->sdep-1] != '-'){
						p->a = p->b;
					}
					printf("******************************************2\n");
					break;
				}
				/*printf("**********************p->bid: %d p->aid: %d auxid: %d\n", p->b->id, p->a->id, aux->id);*/
				p->s = (p->b->sdep - p->b->head);
				index = index + (p->b->sdep - aux->sdep);
				p->a = p->b;
				aux = p->a;
				/*printf("**********************index: %d ********************3\n", index);*/
				desc = DescendQ(p, Ti[phead->Ti][index]);
				/*if (!desc){
					printf("******************************************4\n");
					x = 1;
					break;
				}*/
			}
			/*if (!desc){
				x = 1;
			}
			else if (p->a != aux && x == 0){
				p->a = aux;
			}*/
			p->s = head;
		}
		/*CODE-----------------------------------------------
		while ((p->b->head + p->s) > (p->b)->sdep){
			printf("################################################\n");
			p->a = p->b;
			p->s = (p->b->sdep - p->b->head);
			head = head - (p->b->sdep - p->b->head);
			index = index + (p->b->sdep - p->b->head);
			DescendQ(p, Ti[phead->Ti][index]);
			p->s++;
			printf("SuffixLink p->s: %d    p->aid: %d   p->bid: %d   p->b->head: %d   p->b->sdep: %d\n", p->s, p->a->id, p->b->id, p->b->head, p->b->sdep);
			if ((p->b->head + p->s) == (p->b)->sdep) {
				p->a = p->b;
			}
// 			if ((p->b->head + p->s) < (p->b)->sdep){
// 				printf("!!!!!!!!!!!!!!!SuffixLink p->s: %d    p->aid: %d   p->bid: %d   p->b->head: %d   p->b->sdep: %d\n", p->s, p->a->id, p->b->id, p->b->head, p->b->sdep);
// 				p->s = head;
// 			}
		}
		CODE-----------------------------------------------*/
		
		/*if (p->s == (p->b)->sdep){
			p->a = p->b;
		}
		p->s = head + p->b->head;
		printf("SuffixLink after Descend p->a->id: %d p->b->id %d p->s: %d\n", p->a->id, p->b->id, p->s);
		if (p->s == -1){
			p->a = p->a->child;
			p->b = p->a;
// 			p->s = p->a->sdep;
		}
		else{

			p->s = head;
		}*/
		/*printf("SuffixLink AFTER IF p->s: %d    p->aid: %d    p->bid: %d\n", p->s, p->a->id, p->b->id);*/
	}

	/*if (p->b->sdep < head){
		printf("while conditions: p->s: %d    head: %d\n", p->s, head);
// 		DescendQ(p, Ti[p->b->Ti][p->b->head + p->s]);
		DescendQ(p, Ti[previouslySplit->Ti][previouslySplit->head + p->s]);
		Descend(p);
		if (head > 0){
			p->s += head;
		}
		printf("After DescendQ: pbid: %d    p->s: %d\n", p->b->id, p->s);
	}

	if (p->b->sdep < head){
		printf("SuffixLink while conditions: pbsdep: %d    head: %d\n", p->b->sdep, head);
		if ((p->a)->sdep == -1){ // Testing if pointing to sentinel
			printf("SuffixLink before p->a->id: %d  p->b->id: %d\n", p->a->id, p->b->id);
			p->a = p->a->child;
			p->b = p->a;
			printf("SuffixLink after p->a->id: %d  p->b->id: %d\n", p->a->id, p->b->id);
		}
		DescendQ(p, Ti[p->b->Ti][p->s + 1]);
		p->s += 1;
		if ((p->s - p->b->head) == (p->b)->sdep){
			printf("SuffixLink GOING TO P->B\n");
			p->a = p->b;
		}
		else{
			p->s = head + 1;
		}
// 	printf("DESCEND\n");
		printf("SuffixLink After DescendQ: pbid: %d    p->s: %d\n", p->b->id, p->s);
	}
	p->s = head;
	printf("After suffixjump: pbid: %d    p->s: %d\n", p->b->id, p->s);*/
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
				SuffixLink(p);
				if (j != 0){
					printST(root->child);
					printf("---------------------\n");
				}
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

int main() {
	int numOfLines;
	scanf("%d\n", &numOfLines);
	if (numOfLines == 0) {
		printf("No number\n");
	}
	suffixesIdx = (int*) malloc(sizeof(int) * numOfLines);
	int i;
	Ti = (char**) malloc(sizeof(char*) * numOfLines);
	ni = (int *) malloc(sizeof(int) * numOfLines);
	for (i = 0; i < numOfLines; i++) {
		scanf("%d", &ni[i]);
		Ti[i] = (char*) malloc(sizeof(char) * (ni[i] + 1));
		scanf("%s", Ti[i]);
		Ti[i][ni[i]] = '-';
	}
	root = buildST(numOfLines);

	result = (int*) malloc(sizeof(int) * (numOfLines - 1));
	memset(result, 0, numOfLines-1);

	DFS(root);

	printST(root);

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
	for (i = 0; i < numOfLines; i++) {
		free(Ti[i]);
	}
	free(Ti);
	free(ni);
	free(suffixesIdx);
	free(result);
	return 0;
}
