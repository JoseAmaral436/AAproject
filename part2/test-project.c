#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct suffix {
	int id;
	struct suffix * next;
} Suffix;

typedef struct node {
	int Ti; /**< The value of i in Ti */
	int head; /**< The path-label start at &(Ti[head]) */
	int sdep; /**< String-Depth */
	struct node *child; /**< Child */
	struct node *brother; /**< brother */
	struct node *slink; /**< Suffix link */
	struct node **hook; /**< What keeps this linked? */
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
Node *root = NULL;
Node *previouslyNode = NULL;
Node *previouslySplit = NULL;
int *result = NULL;

Node* initNode() {
	Node* node = malloc(sizeof(Node));
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

void AddLeaf(Point *p, int i, int j) {

	if (p->a == p->b) {
		Node *node = initNode();

		node->Ti = i;
		node->head = j - p->s;
		node->sdep = ni[i] + 1;

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

		if (previouslySplit != NULL
				&& ((previouslySplit->sdep - previouslySplit->head) > 1)) {
			previouslySplit->slink = p->a;
			previouslySplit = NULL;
		}
		previouslyNode = node;
		p->b = node;
	}

	else {
		Node *split = initNode();

		split->Ti = p->b->Ti;
		split->head = p->b->head;
		split->sdep = p->s + p->b->head;
		if (p->b->brother != NULL) {
			split->brother = p->b->brother;
			split->brother->hook = &(split->brother);
		}
		split->child = p->b;

		split->hook = p->b->hook;
		(*(p->b->hook)) = split;
		p->b->hook = &(split->child);

		split->slink = root;
		Node *node = initNode();
		node->Ti = i;
		node->head = j - p->s;
		node->sdep = ni[i] + 1;

		p->b->brother = node;
		node->hook = &(p->b->brother);

		if (previouslySplit != NULL) {

			if (((previouslySplit->sdep - previouslySplit->head)
					<= (split->sdep - split->head))
					&& (previouslySplit->sdep - previouslySplit->head) > 1) {
				previouslySplit->slink = p->a;
			} else if ((previouslySplit->sdep - previouslySplit->head) > 1) {
				previouslySplit->slink = split;
			}
		}
		previouslySplit = split;
		if (previouslyNode != root) {
			previouslyNode->slink = node;
		}
		p->a = split;
		previouslyNode = node;
		p->b = node;
	}
}

void Descend(Point *p) {
	p->s += 1;

	if ((p->b->head + p->s) == (p->b)->sdep) {
		p->a = p->b;
	}
}

bool DescendQ(Point *p, char c) {

	char curr;
	/*Testing if pointing to sentinel*/
	if ((p->a)->sdep == -1) {
		p->a = p->a->child;
		p->b = p->a;

		return true;
	} else {

		if (p->a == p->b) {
			if (p->a->child == NULL) {
				return false;
			} else {
				if (p->b == p->a) {
					p->b = p->a->child;
				}
			}
			curr = Ti[p->b->Ti][p->s + p->b->head];

			if (curr == c) {
				p->b = p->a->child;
				return true;
			} else {
				Node * aux = p->b->brother;
				while (aux != NULL) {
					curr = Ti[aux->Ti][aux->head + p->s];

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

			return curr == c;
		}
	}
}

void SuffixLink(Point *p) {
	int head = p->s - 1;
	Node *phead = p->b;

	p->a = p->a->slink;
	p->b = p->a;
	p->s = p->b->sdep - p->b->head;

	int index = phead->head + 1;
	while (p->b->sdep - p->b->head < head) {

		DescendQ(p, Ti[phead->Ti][index]);

		if (head < p->b->sdep - p->b->head) {

			break;
		}
		p->s = p->s + (p->b->sdep - p->b->head) - (p->a->sdep - p->a->head);
		index = index + (p->b->sdep - p->b->head) - (p->a->sdep - p->a->head);

		if ((p->b->head + p->s) == (p->b)->sdep) {
			p->a = p->b;
		}
		if (p->b->sdep - p->b->head == head) {
			p->a = p->b;
		}

	}
	p->s = head;

	if (p->a == p->b && previouslySplit != NULL) {
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
					&& result[curr->node->suffixesSize - 2]
							< curr->node->sdep - curr->node->head) {
				result[curr->node->suffixesSize - 2] = curr->node->sdep
						- curr->node->head;
			}
			free(popped);
		}
	}
}

void rand_string(char *str, size_t size) {
	const char charset[] = "ACGT";
	if (size) {
		--size;
		for (size_t n = 0; n < size; n++) {
			int key = rand() % (int) (sizeof charset - 1);
			str[n] = charset[key];
		}
	}
}
int main(int argc, char** argv) {
	int seed;
	FILE *f = fopen("/dev/urandom", "r");
	fread(&seed, sizeof(seed), 1, f);
	fclose(f);
	srand(seed);

	int avg;
	int MAX[16] = { 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 1000, 2500, 5000, 7500, 10000, 20000 };
	/* 				0	1	2	3	4	5   6	 7	  8		9	 10	*/
	int ITERATIONS = 1;
	float TOTAL_TIME_UKKONEN = 0;
	float TOTAL_TIME = 0;
	for (avg = 0; avg < ITERATIONS; avg++) {

		int numOfLines = 5;
		int i;
		suffixesIdx = (int*) malloc(sizeof(int) * numOfLines);
		for (i = 0; i < numOfLines; i++) {
			suffixesIdx[i] = 0;
		}

		Ti = (char**) malloc(sizeof(char*) * numOfLines);
		ni = (int *) malloc(sizeof(int) * numOfLines);
		for (i = 0; i < numOfLines; i++) {
			ni[i] = MAX[atoi(argv[1])];/*rand() % (100 + 1 - 0) + 1;*/
			Ti[i] = (char*) malloc(sizeof(char) * (ni[i] + 1));
			rand_string(Ti[i], ni[i] + 1);
		}
		result = (int*) malloc(sizeof(int) * (numOfLines - 1));
		for (i = 0; i < numOfLines - 1; i++) {
			result[i] = 0;
		}

		clock_t start = clock();
		root = buildST(numOfLines);
		clock_t end = clock();
		double time = (double) (end - start) / CLOCKS_PER_SEC; /*[s]*/
		TOTAL_TIME_UKKONEN += (time);

		DFS(root);
		end = clock();
		time = (double) (end - start) / CLOCKS_PER_SEC; /*[s]*/
		TOTAL_TIME += time;

		clean(root);
		for (i = 0; i < numOfLines; i++) {
			free(Ti[i]);
		}
		free(Ti);
		free(ni);
		free(suffixesIdx);
		free(result);
	}
	printf("ITERATIONS: %d MAX_SIZE: %d\n", ITERATIONS, MAX[atoi(argv[1])]);
	printf("TOTAL_TIME_UKKONEN[ms]: %f\n", (TOTAL_TIME_UKKONEN * 1000) / ITERATIONS);
	printf("TOTAL_TIME[ms]: %f\n", (TOTAL_TIME * 1000) / ITERATIONS);
	return 0;
}
