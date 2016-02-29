#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  int numOfOcc;
  char *string;
  struct node *left;
  struct node *right;
} Node;

Node *root;

void tsearch(char *s){
  int size = strlen(s);
  if (root == NULL){
    root = malloc(sizeof(Node));
    root->string = malloc(sizeof(char) * size);
    strcpy(root->string, s);
    root->numOfOcc = 1;
    root->left = NULL;
    root->right = NULL;
    printf("%d\n",root->numOfOcc);
  }
  else{
    Node *aux = root;
    while(1){
      if((strcmp(aux->string, s)) < 0){
	if(aux->left == NULL){
	  Node *new = malloc(sizeof(Node));
	  new->string = malloc(sizeof(char) * size);
	  strcpy(new->string, s);
	  new->numOfOcc = 1;
	  new->left = NULL;
	  new->left = NULL;
	  aux->left = new;
	  printf("%d\n",new->numOfOcc);
	  break;
	}
	else{
	  aux = aux->left;
	}
      }
      else if((strcmp(aux->string, s)) > 0){
	if(aux->right == NULL){
	  Node *new = malloc(sizeof(Node));
	  new->string = malloc(sizeof(char) * size);
	  strcpy(new->string, s);
	  new->numOfOcc = 1;
	  new->left = NULL;
	  new->right = NULL;
	  aux->right = new;
	  printf("%d\n",new->numOfOcc);
	  break;
	}
	else{
	  aux = aux->right;
	}
      }
      else{
	aux->numOfOcc = aux->numOfOcc + 1;
	printf("%d\n",aux->numOfOcc);
	break;
      }
    }
  }
}

Node * tfind(char *s){
  if (root == NULL){
    return NULL;
  }
  else{
    Node *aux = root;
    while(1){
      if((strcmp(aux->string, s)) < 0){
	if(aux->left == NULL){
	  return NULL;
	}
	else{
	  aux = aux->left;
	}
      }
      else if((strcmp(aux->string, s)) > 0){
	if(aux->right == NULL){
	  return NULL;
	}
	else{
	  aux = aux->right;
	}
      }
      else{
	printf("%d\n",aux->numOfOcc);
	return aux;
      }
    }
  }
}

/*void tdelete(char *s){
  if (root == NULL){
    return NULL;
  }
  else{
    Node *aux = root;
    while(1){
      if((strcmp(aux->string, s)) < 0){
	if(aux->left == NULL){
	  return NULL;
	}
	else{
	  aux = aux->left;
	}
      }
      else if((strcmp(aux->string, s)) > 0){
	if(aux->right == NULL){
	  return NULL;
	}
	else{
	  aux = aux->right;
	}
      }
      else{
	printf("%d\n",aux->numOfOcc);
	return aux;
      }
    }
  }
}*/

int main(){
  int numOfLines;
  char *number = NULL;
  size_t size;
  if (getline(&number, &size, stdin) == -1) {
        printf("No number\n");
  }
  else{
    numOfLines = atoi(number);
  }
  printf("%d\n", numOfLines);
  int i;
  char *line = NULL;
  size_t sizeOfLine;
  for(i = 1; i <= numOfLines; i++){
    getline(&line, &sizeOfLine, stdin);
    line[8] = '\0'; //TODO ASK IF ALL STRINGS HAVE SAME LENGTH
    switch(line[0]){
      case 'A':
	memmove(line, line+1, strlen(line));
	memmove(line, line+1, strlen(line));
	tsearch(line);
	break;
      case 'F':
	memmove(line, line+1, strlen(line));
	memmove(line, line+1, strlen(line));
	tfind(line);
	break;
      case 'D':
	memmove(line, line+1, strlen(line));
	memmove(line, line+1, strlen(line));
	printf("D aind n feito\n");
	//tdelete(line);
	break;
    }
  }
  free(line);
  return 0;
}