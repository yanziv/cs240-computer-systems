#include <assert.h>
#include <stdlib.h>

Node* freshNode(Node* next, int x) {
  Node* n = (Node*)malloc(sizeof(Node));
  n->next = next;
  n->value = x;
}
void addHead(List* list, int x) {
  assert(list);
  list->head = freshNode(list->head, x);
}
void addTail(List* list, int x) {
  assert(list);
  if (list->head) {
    addTailRec(list->head, x);
  } else {
    addHead(list, x);
  }
}
void addTailRec(Node* node, int x) {
  assert(node);
  if (node->next == NULL) {
    node->next = freshNode(NULL, x);
  } else {
    insert(node->next, x);
  }
}
void addTailLoop(Node* node, int x) {
  assert(node);
  while (node->next) {
    node = node->next;
  }
  node->next = freshNode(NULL, x);
}
void insert(List* list, int x) {
  assert(list);
  if (list->head && list->head->value < x) {
    insertRec(list->head, x);
  } else {
    addFirst(list, x);
  }
}
void insertRec(Node* node, int x) {
  assert(node);
  if (node->next && node->next->value < x) {
    insertRec(node->next, x);
  } else {
    node->next = freshNode(node->next, x);
  }
}
void insertLoop(Node* node, int x) {
  assert(node);
  while (node->next && node->next->value < x) {
    node = node->next;
  }
  node->next = freshNode(node->next, x);
}
