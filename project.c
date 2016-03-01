#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct info {
  int numOfOcc;
  char *string;
} Info;


typedef struct node {
/*  int numOfOcc;*/
  const void *value;
  struct node *left;
  struct node *right;
} Node;

void *root;

static int compar(const void *pa, const void *pb){
  char *firstString = ((Info *) pa)->string;
  char *secondString = ((Info *) pb)->string;
  
  return strcmp(firstString, secondString);
}

void *tsearch(const void *key, void **rootp, int (*compar)(const void *, const void *)){
  if (*rootp == NULL){
    *rootp = malloc(sizeof(Node));
    ((Node *)(*rootp))->value = key;
    ((Node *)(*rootp))->left = NULL;
    ((Node *)(*rootp))->right = NULL;
    return *rootp;
    //printf("%d\n", key->numOfOcc);
  }
  else{
    Node *aux = *rootp;
    int compare;
    while(1){
      compare = (compar(aux->value, key));
      if(compare < 0){
	if(aux->left == NULL){
	  Node *new = malloc(sizeof(Node));
	  new->value = key;
	  new->left = NULL;
	  new->right = NULL;
	  aux->left = new;
	  //printf("%d\n",key->numOfOcc);
	  return new;
	}
	else{
	  aux = aux->left;
	}
      }
      else if(compare > 0){
	if(aux->right == NULL){
	  Node *new = malloc(sizeof(Node));
	  new->value = key;
	  new->left = NULL;
	  new->right = NULL;
	  aux->right = new;
	  //printf("%d\n",key->numOfOcc);
	  return new;
	}
	else{
	  aux = aux->right;
	}
      }
      else{
	//key->numOfOcc = key->numOfOcc + 1;
	//printf("%d\n",key->numOfOcc);
	return aux;
      }
    }
  }
}

void * tfind(const void *key, void **rootp, int (*compar)(const void *, const void *)){
  if (*rootp == NULL){
    return NULL;
  }
  else{
    Node *aux = *rootp;
    int compare;
    while(1){
      compare = (compar(aux->value, key));
      if(compare < 0){
	if(aux->left == NULL){
	  return NULL;
	}
	else{
	  aux = aux->left;
	}
      }
      else if(compare > 0){
	if(aux->right == NULL){
	  return NULL;
	}
	else{
	  aux = aux->right;
	}
      }
      else{
	return aux;
      }
    }
  }
}

void * tdelete(const void *key, void **rootp, int (*compar)(const void *, const void *)){ //TODO IF REMOVE ROOT
  if (*rootp == NULL){
    return NULL;
  }
  else{
    Node *parent = NULL;
    Node *curr = *rootp;
    int parentage = -1; //0 if left, 1 if right
    int compare;
    while(1){
      compare = (compar(curr->value, key));
      if(compare < 0){
	if(curr->left == NULL){
	  printf("NULL\n");
	  return NULL;
	}
	else{
	  parent = curr;
	  curr = curr->left;
	  parentage = 0;
	}
      }
      else if(compare > 0){
	if(curr->right == NULL){
	  printf("NULL\n");
	  return NULL;
	}
	else{
	  parent = curr;
	  curr = curr->right;
	  parentage = 1;
	}
      }
      else{
	if ((curr->left == NULL) && (curr->right == NULL)){ // no CHILD
	  free((void *) curr->value);
	  free(curr);
	  if (parentage == 0){
	    parent->left = NULL;
	  }
	  else if (parentage == 1){
	    parent->right = NULL;
	  }
	}
	else if ((!(curr->left == NULL)) != (!(curr->right == NULL))){ //XOR , only one child
	  free((void *) curr->value);
	  free(curr);
	  if (parentage == 0){
	    if (curr->left == NULL){
	      parent->left = curr->right;
	    }
	    else{
	      parent->left = curr->left;
	    }
	  }
	  else if (parentage == 1){
	    if (curr->left == NULL){
	      parent->right = curr->right;
	    }
	    else{
	      parent->right = curr->left;
	    }
	  }
	}
	else{ // TODO 2 CHILDREN
	  
	}
	return parent; // RETURN PARENT of deleted node
      }
    }
  }
}

int main(){
  root = NULL;
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
    int lineSize = strlen(line);
    if (i != numOfLines){
      line[lineSize - 1] = '\0'; //all strings have '\n' in the end beside the last
    }
    else{
      line[lineSize] = '\0'; // last string doesnt have '\n'
    }
    void * result;
    switch(line[0]){
      case 'A':
	memmove(line, line+1, strlen(line)); // take first and
	memmove(line, line+1, strlen(line)); // second letter out
	Info * info = malloc(sizeof(Info));
	info->string = malloc(sizeof(char) * lineSize);
	strcpy(info->string, line);
	result = tsearch(info, &root, compar);
	((Info *)((Node *) result)->value)->numOfOcc += 1;
	printf("%d\n", ((Info *)((Node *) result)->value)->numOfOcc);
	break;
      case 'F':
	memmove(line, line+1, strlen(line)); //TODO TEST MEMORY LEAK
	memmove(line, line+1, strlen(line));
	Info * find = malloc(sizeof(Info)); //this makes no
	find->string = malloc(sizeof(char) * lineSize); //sense ... why should we do new Info* just to find
	strcpy(find->string, line); //How should we do it ???
	result = tfind(find, &root, compar);
	if (result == NULL){
	  printf("NULL\n");
	}
	else{
	  printf("%d\n", ((Info *)((Node *) result)->value)->numOfOcc);
	}
	free(find->string);
	free(find);
	break;
      case 'D':
	memmove(line, line+1, strlen(line));
	memmove(line, line+1, strlen(line));
	Info * delete = malloc(sizeof(Info)); //this makes no
	delete->string = malloc(sizeof(char) * lineSize); //sense ... why should we do new Info* just to delete
	strcpy(delete->string, line); //How should we do it ???
	result = tfind(find, &root, compar); // CAN WE USE FIND IN CASE D ? 
	if (result == NULL){
	  printf("NULL\n");
	}
	else{
	  printf("%d\n", ((Info *)((Node *) result)->value)->numOfOcc); // NEED TO PRINT BEFORE DELETING
	  result = tdelete(delete, &root, compar);
	  free(delete->string);
	  free(delete);
	}
	break;
    }
  }
  return 0;
}