# CODE_DESCRIPTION

The summarized description about InfiniteNumberNode.c and main.c

## InfiniteNumberNode.c

---

### DataStructure

* **typedef struct __NumberNode**

| member(struct) | val | val | val | val | val |
| :-: | :-: | :-: | :-: | :-: | :-: |
| next(NN) | 5 | DOT | 1 | 2 | NULL |
| prev(NN) | NULL | 3 | 5 | DOT | 1 |
| number(int) | 3 | 5 | DOT | 1 | 2 |

> ex : 35.12

* **typedef struct __NumberList**
   
| member(struct) | val |
| :-: | :-: |
| head(NN) | 3 |
| tail(NN) | 2 |
| dot(int) | DOT |
| op(int) | 0 |
| sig(int) | 1 |

> op ? operation : not operation
> sig ? + : -
> ex : 35.12

* **typedef struct __NumberListNode**

| member(struct) | val | val | val |
| :-: | :-: | :-: | :-: |
| next(NLN) | 31.63 | ADD | NULL | 
| prev(NLN) | NULL | 35.12 | 31.63 |
| value(NL) | 35.12 | 31.63 | ADD |

> ex : 35.12 + 31.63 in queue by conversing infix to postfix

* **typedef struct __calQueue**

| member(struct) | val |
| :-: | :-: |
| qHead(NLN) | 35.12 |
| qTail(NLN) | ADD |

> ex : 35.12 + 31.63 in queue by conversing infix to postfix

* **typedef struct __calStack**  

| member(struct) | val |
| :-: | :-: |
| sTop(NLN) | ADD |

> ex : 35.12 + 31.63 in stack by conversing infix to postfix

### Operation Method

NumberListNode* add(NumberListNode* val1, NumberListNode* val2);
NumberListNode* subtract(NumberListNode* val1, NumberListNode* val2);
NumberListNode* multiply(NumberListNode* val1, NumberListNode* val2);
NumberListNode* divide(NumberListNode* val1, NumberListNode* val2);

### DataStructure Method

void NumberList_push_back(NumberList* nl, int val, NumberNode* now);
void NumberList_push_front(NumberList* nl, int val, NumberNode* now);
void queue_push(queue* Q, NumberListNode* nowNode);
NumberListNode* queue_pop(queue* Q);
void stack_push(stack* S, NumberListNode* now);
NumberListNode* stack_pop(stack* S);
void stack_push_op(stack* S, queue* Q, int opInput);

### OperationUnit Method

int compareAbsoluteValue(NumberListNode* val1, NumberListNode* val2);
NumberListNode* divide_by_2(NumberListNode* val);
void divide_by_10(NumberListNode* val);
void product_by_10(NumberListNode* val);
void NumberList_push_dot(NumberList* nl);
void delete_zero(NumberListNode* val);

## Main.c

---

### 중위표기식->후위표기식 전환 과정

    if(ch == ' ' || ch == '\t' || ch == '\n')
    if (ch >= '0' && ch <= '9')
    else if (ch == '.')
    else if (ch == '*' || ch == '/' || ch == '+' || ch == '-')
    else if (ch == '(')
    else if (ch == ')')

### 후위표기식 계산 과정

    if (now->value->op == 0)

    else
        case ADD:
        case SUB:
        case MUL:
        case DIV:
    
