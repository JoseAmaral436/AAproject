#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int *ni;
char **Ti;


typedef struct node {
	int Ti; /**< The value of i in Ti */
	int head; /**< The path-label start at &(Ti[head]) */
	int sdep; /**< String-Depth */
	struct node *child; /**< Child */
	struct node *brother; /**< brother */
	struct node *slink; /**< Suffix link */
	struct node **hook; /**< What keeps this linked? */
	char* name;
} Node;

typedef struct point {
	Node *a; /**< node above */
	Node *b; /**< node bellow */
	int s; /**< String-Depth */
} Point;


Node* initNode(){
	Node* node = malloc(sizeof(Node));

	node->Ti = 0;
	node->sdep = 0;
	node->head = 0;
	node->brother = NULL;
	node->child = NULL;
	node->hook = NULL;
	node->slink = NULL;

	return node;
}


void AddLeaf(Point *p, int i, int j){
	Node *node = initNode();
	node->Ti = i;
	node->head = j;
	node->sdep = ni[i] - j;
	node->name = Ti[node->Ti];
// 	printf("namea: %s    sdepa: %d  nameb: %s    sdepb: %d\n", p->a->name, p->a->sdep, p->b->name, p->b->sdep);
	Node *ancestor = p->a;
	if (ancestor->child == NULL){
		ancestor->child = node;
		node->hook = &(ancestor->child);
	}
	else{
		ancestor = ancestor->child;
		while(ancestor->brother != NULL){
			ancestor = ancestor->brother;
		}
		ancestor->brother = node;
		node->hook = &(ancestor->brother);
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
	if(p->s != 0){
		Node *split = initNode();
		split->Ti = p->b->Ti;
		split->head = p->b->head;
		split->sdep = 0; //TODO
		split->brother = p->b->brother;
		split->child = p->b;
// 		Node *aux = p->b;
// 		while(aux->brother != NULL){
// 			aux = aux->brother;
// 		}
// 		aux->brother = node;
		
		p->b->head += p->s;
		p->b->sdep -= p->s;
// 		printf("SPLIT:   sdep: %d     head: %d      Ti: %d  nameA: %s    nameB: %s   sdep: %d\n", split->sdep, split->head, split->Ti, p->a->name, p->b->name, p->b->sdep);
		(*(p->b->hook)) = split;
		Node *aux = initNode();
		aux->Ti = i;
		aux->head = j;
		aux->sdep = ni[i] - j;
		aux->name = Ti[aux->Ti];
		p->b->brother = aux;
	}
}

void printString(Node *n){
	int i;
	int index = n->head + n->sdep + 1;
// 	printf("n->head: %d    index: %d\n", n->head, index);
	for(i = n->head; i < index; i++){
		printf("%c", Ti[n->Ti][i]);
	}
// 	printf(" Ti: %d   sdep: %d   head: %d name: %s", n->Ti, n->sdep, n->head, n->name);
// 		printf(" %s ", (*(n->hook))->name);
	printf("\n");
	
}

void printST(Node *root){
	printString(root);
	if (root->child == NULL){
		if (root->brother != NULL){
			printST(root->brother);
		}
	}
	else{
		printST(root->child);
		if (root->brother != NULL){
			printST(root->brother);
		}
	}
	
}


void SuffixLink(Point *p){
	p->a = (p->a)->slink;
	p->b = p->a;
	p->s = (p->a)->sdep;
}


void Descend(Point *p){
	if (p->s == (p->b)->sdep){
		p->a = p->b;
	}
	p->s += 1;
}


bool DescendQ(Point *p, char c){
//	printf("%d\n", (p->a)->sdep);
	if ((p->a)->sdep == -1){ // Testing if pointing to sentinel
		p->a = p->a->child;
		p->b = p->a;
// 		printf("sentinel %c \n", c);
		return true;
	}
	else{
// 		printf("else %c \n", c);
		if(p->a->child == NULL){
			return false;
		}
		char curr = Ti[p->b->Ti][p->b->head + p->s];
		if (curr == c){
			p->b = p->a->child;
			return true;
		}
		else{
			if (p->b->child != NULL){
				Node * aux = p->b->child->brother;
				while(aux != NULL){
					curr = Ti[aux->Ti][aux->head];
// 					printf("c: %c curr: %c   auxTi: %d   head: %d\n", c, curr, aux->Ti, aux->head);
					if (curr == c){
						p->b = aux;
// 						printf("match\n");
						return true;
					}
					aux = aux->brother;
				}
				return false;
			}
			else{
				return false; //TODO
			}
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

Node * buildST(int numberOfStrings){
	Node *root = initNode();
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
	while(i < numberOfStrings){
		p->s = 0;
		int j = 0;
		if (i == 0){
			Ti[i][ni[i]]= '!';
		}
		else if (i == 1){
			Ti[i][ni[i]]= '$';
		}
		while(j <= ni[i]){
			while(!DescendQ(p, Ti[i][j])){
				AddLeaf(p, i, j);
				SuffixLink(p);
			}
			Descend(p);
			j++;
		}
// 		Ti[i][ni[i]]= '-';
		i++;
	}
	return root;
}

int main() {
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
	char* token;
	Ti = (char**) malloc(sizeof(char*) * numOfLines);
	ni = malloc(sizeof(int) * numOfLines);
	for (i = 1; i <= numOfLines; i++) {
		getline(&line, &sizeOfLine, stdin);
		token = strtok(line, " ");
		ni[i-1] = atoi(token);
		token = strtok(NULL, " ");
		token[ni[i-1]] = '-'; //all strings have '\n' in the end beside the last
		if (i == numOfLines) {
			token[ni[i-1] + 1] = '\0'; // last string doesnt have '\n'
		}
// 		Ti[i-1] = malloc(sizeof(char) * (ni[i-1] + 1));
		Ti[i-1] = strdup(token);
	}
	Node * root = buildST(numOfLines);
	printST(root->child);
	return 0;
}
