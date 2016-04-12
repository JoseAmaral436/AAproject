#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int *ni;
char **generalizedString;


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


void AddLeaf(Point *p, int i, int j){
	Node *node = malloc(sizeof(Node));
	node->Ti = i;
	node->head = j;
	node->sdep = ni[i] - j;
	node->child = NULL;
	node->brother = NULL;
	node->slink = NULL;
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
}
void printString(Node *n, char* string[]){
	int i;
	int index = n->head + n->sdep + 1;
	for(i = n->head; i < index; i++){
		printf("%c", string[n->Ti][i]);
	}
	printf("\n");
	
}

void printST(Node *root, char* string[]){
	if (root->child == NULL){
		printString(root, string);
		if (root->brother != NULL){
			printST(root->brother, string);
		}
	}
	else{
		printST(root->child, string);
		if (root->brother != NULL){
			printST(root->brother, string);
		}
	}
	
}


/*void SuffixLink(Point *p){
	p->s = 0;
	if(p->a == p->b){
		p->a = (p->a)->slink;
		p->b = (p->b)->slink;
	}
}*/


/*void Descend(Point *p){
	p->s += 1;
	if(p->a == p->b){
		p->b = (p->a)->child; //TODO pode ser brother ?
	}
}*/

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
		return true;
	}
	else{
		if(p->a->child == NULL)
			return false;
		char curr = generalizedString[(p->a->child)->Ti][p->s];

		return curr == c;




//	// TODO UPDATE p->b
//	printf("c: %c\n", generalizedString[(p->b)->Ti][p->s]);
// 	if ((p->a)->child == NULL){
// 		return false;
// 	}
// 	else{
// 		//printf("c: %c\n", generalizedString[(p->b)->Ti][p->s]);
// 	}
		return false;
	}
}

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
Node * buildST(char* string[], int ni[],int numberOfStrings){
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
		while(j <= ni[i]){
			//DescendQ(p, string[i][j]);
			while(!DescendQ(p, string[i][j])){
				AddLeaf(p, i, j);
				SuffixLink(p);
			}
//			printf("c: %c %d %d\n", string[i][j], (p->a)->sdep, (p->b)->sdep);
			Descend(p);
			j++;
		}
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
	generalizedString = (char**) malloc(sizeof(char*) * numOfLines);
	ni = malloc(sizeof(int) * numOfLines);
	for (i = 1; i <= numOfLines; i++) {
		getline(&line, &sizeOfLine, stdin);
		int lineSize = strlen(line);
		if (i != numOfLines) {
			line[lineSize - 1] = '-'; //all strings have '\n' in the end beside the last
		} else {
			line[lineSize] = '-'; // last string doesnt have '\n'
		}
		token = strtok(line, " ");
		ni[i-1] = atoi(token);
		token = strtok(NULL, " ");
		generalizedString[i-1] = malloc(sizeof(char) * ni[i-1]);
		strcpy(generalizedString[i-1], token);
	}
	Node * root = buildST(generalizedString, ni, numOfLines);
	printST(root, generalizedString);
	return 0;
}
