#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_SIZE 100000
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define OPEN_BRACKET 1000000
#define DOT -1000000000
typedef struct __NumberNode {
    int number;
    struct __NumberNode* next;
    struct __NumberNode* prev;
} NumberNode;

// directionless Number list
typedef struct __NumberList {
    struct __NumberNode* head;
    struct __NumberNode* tail;
    struct __NumberNode* dot;
    int sig;
    int op;
} NumberList;

// A structure providing directionality for concatenating elements in a list of
// real numbers, both front and back
typedef struct __NumberListNode {
    NumberList* value;
    struct __NumberListNode* prev;
    struct __NumberListNode* next;
} NumberListNode;

typedef struct __calQueue {
    NumberListNode* qHead;
    NumberListNode* qTail;
} queue;

typedef struct __calStack {
    NumberListNode* sTop;
} stack;

void NumberList_push_back(NumberList* nl, int val, NumberNode* now);
void NumberList_push_front(NumberList* nl, int val, NumberNode* now);
NumberListNode* makeNumberListNode();
NumberListNode* copyNumberListNode(NumberListNode* val);
NumberList* makeNumberList();
NumberNode* makeNumberNode();
void queue_push(queue* Q, NumberListNode* nowNode);
NumberListNode* queue_pop(queue* Q);
void stack_push(stack* S, NumberListNode* now);
NumberListNode* stack_pop(stack* S);
void NumberList_Clear(NumberList* nl);
void NumberListNode_Clear(NumberListNode* nln);
void stack_push_op(stack* S, queue* Q, int opInput);
void mallocAssert(void* p);
void print_NumberListNode(NumberListNode* nln);

void initializeQueue(queue* q);
void initializeStack(stack* s);
void initializeNumberNode(NumberNode* node);
void initializeNumberList(NumberList* list);
void initializeNumberListNode(NumberListNode* node);

int compareAbsoluteValue(NumberListNode* val1, NumberListNode* val2);
NumberListNode* divide_by_2(NumberListNode* val);
void divide_by_10(NumberListNode* val);
void product_by_10(NumberListNode* val);
void NumberList_push_dot(NumberList* nl);
void delete_zero(NumberListNode* val);

NumberListNode* add(NumberListNode* val1, NumberListNode* val2);
NumberListNode* subtract(NumberListNode* val1, NumberListNode* val2);
NumberListNode* multiply(NumberListNode* val1, NumberListNode* val2);
NumberListNode* divide(NumberListNode* val1, NumberListNode* val2);

// for debug
void print_stack_top(stack* s);
void print_queue(queue* q);