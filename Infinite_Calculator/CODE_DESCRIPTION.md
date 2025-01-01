# CODE_DESCRIPTION




## InfiniteNumberNode.c


* **typedef struct __NumberNode** > (member|struct)    
    * (number|int)[3]<-(prev|NN)|(next|NN)->(number|int)[2]<-(prev|NN)|(next|NN)->(number|int)[DOT] ...


* **typedef struct __NumberList** > (member|struct)
    * (head|NN)[3], (dot|NN)[DOT], (tail|NN)[2], (sig|int)[1], (op|int)[0]


* **typedef struct __NumberListNode** > (member|struct)
    * (value|NL)[32.32]<-(prev|NLN)|(next|NLN)->(value|NL)[11.41]


* **typedef struct __calQueue** > (member|struct)      
    * [ (qHead|NLN)[32.32] | [11.41] | [+] | (qTail|NLN)[-] ]


* **typedef struct __calStack** > (member|struct)      
    * [ [-] | (sTop|NLN)[+] ]


NumberListNode* add(NumberListNode* val1, NumberListNode* val2);
NumberListNode* subtract(NumberListNode* val1, NumberListNode* val2);
NumberListNode* multiply(NumberListNode* val1, NumberListNode* val2);
NumberListNode* divide(NumberListNode* val1, NumberListNode* val2);

void NumberList_push_back(NumberList* nl, int val, NumberNode* now);
void NumberList_push_front(NumberList* nl, int val, NumberNode* now);
void queue_push(queue* Q, NumberListNode* nowNode);
NumberListNode* queue_pop(queue* Q);
void stack_push(stack* S, NumberListNode* now);
NumberListNode* stack_pop(stack* S);
void stack_push_op(stack* S, queue* Q, int opInput);
    
int compareAbsoluteValue(NumberListNode* val1, NumberListNode* val2);
NumberListNode* divide_by_2(NumberListNode* val);
void divide_by_10(NumberListNode* val);
void product_by_10(NumberListNode* val);
void NumberList_push_dot(NumberList* nl);
void delete_zero(NumberListNode* val);

main.c

중위표기식->후위표기식 전환 과정
    if(ch == ' ' || ch == '\t' || ch == '\n')
    if (ch >= '0' && ch <= '9')
    else if (ch == '.')
    else if (ch == '*' || ch == '/' || ch == '+' || ch == '-')
    else if (ch == '(')
    else if (ch == ')')
후위표기식 계산 과정
    if (now->value->op == 0)

    else
        case ADD:
        case SUB:
        case MUL:
        case DIV:
    
