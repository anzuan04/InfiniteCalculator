# CODE_DESCRIPTION

The summarized description about InfiniteNumberNode.c and main.c

## InfiniteNumberNode.c

---

### DataStructure

* **typedef struct __NumberNode**    
    * (number|int)[3]<-(prev|NN)|(next|NN)->(number|int)[2]<-(prev|NN)|(next|NN)->(number|int)[DOT] ...
    > (member|struct)

* **typedef struct __NumberList**
    * (head|NN)[3], (dot|NN)[DOT], (tail|NN)[2], (sig|int)[1], (op|int)[0]
    > (member|struct)

* **typedef struct __NumberListNode**
    * (value|NL)[32.32]<-(prev|NLN)|(next|NLN)->(value|NL)[11.41]
    > (member|struct)

* **typedef struct __calQueue**      
    * [ (qHead|NLN)[32.32] | [11.41] | [+] | (qTail|NLN)[-] ]
    > (member|struct)

* **typedef struct __calStack**      
    * [ [-] | (sTop|NLN)[+] ]
    > (member|struct)

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
    
