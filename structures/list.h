#ifndef __LIST_H_
#define __LIST_H_

typedef struct Node {
  struct Node* next;
  int value;
} Node;


typedef struct List {
  struct Node* head;
} List;

#endif // __LIST_H_
