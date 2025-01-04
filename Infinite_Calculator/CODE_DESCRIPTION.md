# CODE_DESCRIPTION

The summarized description about InfiniteNumberNode.c and main.c

## InfiniteNumberNode.c

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
| next(NLN) | 38.995 | ADD | NULL | 
| prev(NLN) | NULL | 35.12 | 38.995 |
| value(NL) | 35.12 | 38.995 | ADD |

> ex : 35.12 + 38.995 in queue by conversing infix to postfix

* **typedef struct __calQueue**

| member(struct) | val |
| :-: | :-: |
| qHead(NLN) | 35.12 |
| qTail(NLN) | ADD |

> ex : 35.12 + 38.995 in queue by conversing infix to postfix

* **typedef struct __calStack**  

| member(struct) | val |
| :-: | :-: |
| sTop(NLN) | ADD |

> ex : 35.12 + 38.995 in stack by conversing infix to postfix

---

### Operation Method

* **NumberListNode\* add(NumberListNode\* val1, NumberListNode\* val2);**
    1. **Definition To Setup**

                 ic1     fc1            **NN pointer**
                  |       |
                  v       v  
            +---+---+---+---+---+
            | 3 | 5 | . | 1 | 2 |       **NLN val1**
            +---+---+---+---+---+

                 ic2     fc2            **NN pointer**
                  |       |
                  v       v  
            +---+---+---+---+---+---+
            | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
            +---+---+---+---+---+---+

            +---+
            |   |                       **NLN ret**
            +---+

            int carry = 0

        > ex : 35.12 + 38.995

    2. **Addition Process for Simple Sum**

    * ***float Addition process***

        1. *add fc1 + fc2 then store result to tail of ret*

                     ic1     fc1            **NN pointer**
                      |       |
                      v       v  
                +---+---+---+---+---+
                | 3 | 5 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                     ic2     fc2            **NN pointer**
                      |       |
                      v       v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +----+
                | 10 |                      **NLN ret**
                +----+

            > But if fc1->next == NULL or fc2->next == NULL, the number indicated by the pointer is not added

        2. *move fc1 and fc2 to next then repeat 1.*

                     ic1         fc1        **NN pointer**
                      |           |
                      v           v  
                +---+---+---+---+---+
                | 3 | 5 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                     ic2         fc2        **NN pointer**
                      |           |
                      v           v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +----+----+
                | 10 | 11 |                 **NLN ret**
                +----+----+

            > But if fc1->next==NULL|fc2->next==NULL, the pointer does not move
        
        3. *again 1.~2.*

                     ic1         fc1        **NN pointer**
                      |           |
                      v           v  
                +---+---+---+---+---+
                | 3 | 5 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                     ic2             fc2    **NN pointer**
                      |               |
                      v               v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +----+----+---+
                | 10 | 11 | 5 |             **NLN ret**
                +----+----+---+

        4. process the carry in ret

                     ic1         fc1        **NN pointer**
                      |           |
                      v           v  
                +---+---+---+---+---+
                | 3 | 5 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                     ic2             fc2    **NN pointer**
                      |               |
                      v               v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +---+---+---+
                | 1 | 1 | 5 |               **NLN ret**
                +---+---+---+

                int carry = 1

            > Iterate each ret->number, and handle the carry if the result is greater than or equal to 10.
        
        5. add DOT to ret

                     ic1         fc1        **NN pointer**
                      |           |
                      v           v  
                +---+---+---+---+---+
                | 3 | 5 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                     ic2             fc2    **NN pointer**
                      |               |
                      v               v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +---+---+---+---+
                | . | 1 | 1 | 5 |           **NLN ret**
                +---+---+---+---+

                int carry = 1

    * ***int Addition process***
    
        1. *calculate (ic1 + ic2 + carry)%10 then store result to head of ret*

                     ic1         fc1        **NN pointer**
                      |           |
                      v           v  
                +---+---+---+---+---+
                | 3 | 5 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                     ic2             fc2    **NN pointer**
                      |               |
                      v               v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +---+---+---+---+---+
                | 4 | . | 1 | 1 | 5 |       **NLN ret**
                +---+---+---+---+---+

                int carry = 1
            
            > int carry = (ic1 + ic2 + carry)/10

        2. *move ic1 and ic2 to prev then repeat 1.*

                 ic1         fc1            **NN pointer**
                  |           |
                  v           v  
                +---+---+---+---+---+
                | 3 | 5 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                 ic2         fc2            **NN pointer**
                  |           |
                  v           v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +---+---+---+---+---+---+
                | 7 | 4 | . | 1 | 1 | 5 |   **NLN ret**
                +---+---+---+---+---+---+

* **NumberListNode\* subtract(NumberListNode\* val1, NumberListNode\* val2);**
    
    1. **Definition To Setup**

                 ic1     fc1            **NN pointer**
                  |       |
                  v       v  
            +---+---+---+---+---+
            | 3 | 9 | . | 1 | 2 |       **NLN val1**
            +---+---+---+---+---+

                 ic2     fc2            **NN pointer**
                  |       |
                  v       v  
            +---+---+---+---+---+---+
            | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
            +---+---+---+---+---+---+

            +---+
            |   |                       **NLN ret**
            +---+

            int carry = 0

        > ex : 39.12 + 38.995

    2. **Subtraction Process for Simple Sum**

    * ***float subtraction process***

        1. *subtract 10 + fc1 - fc2 then store result to tail of ret*

                     ic1     fc1            **NN pointer**
                      |       |
                      v       v  
                +---+---+---+---+---+
                | 3 | 9 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                     ic2     fc2            **NN pointer**
                      |       |
                      v       v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +---+
                | 2 |                       **NLN ret**
                +---+
            
            > But if fc1->next == NULL or fc2->next == NULL, the number indicated by the pointer is not calculated

        2. *move fc1 and fc2 to next then repeat 1.*

                     ic1         fc1        **NN pointer**
                      |           |
                      v           v  
                +---+---+---+---+---+
                | 3 | 9 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                     ic2         fc2        **NN pointer**
                      |           |
                      v           v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +---+---+
                | 2 | 3 |                   **NLN ret**
                +---+---+

            > But if fc1->next==NULL|fc2->next==NULL, the pointer does not move
        
        3. *again 1.~2.*

                     ic1         fc1        **NN pointer**
                      |           |
                      v           v  
                +---+---+---+---+---+
                | 3 | 9 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                     ic2             fc2    **NN pointer**
                      |               |
                      v               v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +---+---+---+
                | 2 | 3 | 5 |               **NLN ret**
                +---+---+---+

        4. process the borrowing in ret

                     ic1         fc1        **NN pointer**
                      |           |
                      v           v  
                +---+---+---+---+---+
                | 3 | 9 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                     ic2             fc2    **NN pointer**
                      |               |
                      v               v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +---+---+---+
                | 1 | 2 | 5 |               **NLN ret**
                +---+---+---+

                int borrow = 1

            > Iterate each ret->number, and handle the borrowing if the result is greater than or equal to 10.
        
        5. add DOT to ret

                     ic1         fc1        **NN pointer**
                      |           |
                      v           v  
                +---+---+---+---+---+
                | 3 | 9 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                     ic2             fc2    **NN pointer**
                      |               |
                      v               v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +---+---+---+---+
                | . | 1 | 1 | 5 |           **NLN ret**
                +---+---+---+---+

                int borrow = 1

    * ***int subtraction process***
    
        1. *calculate (10 + ic1 - ic2 - borrow) then store result to head of ret*

                     ic1         fc1        **NN pointer**
                      |           |
                      v           v  
                +---+---+---+---+---+
                | 3 | 9 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                     ic2             fc2    **NN pointer**
                      |               |
                      v               v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +---+---+---+---+---+
                | 0 | . | 1 | 1 | 5 |       **NLN ret**
                +---+---+---+---+---+

                int borrow = 0
            
            > int borrow = !((10 + ic1 - ic2 - borrow) % 10)

        2. *move ic1 and ic2 to prev then repeat 1.*

                 ic1         fc1            **NN pointer**
                  |           |
                  v           v  
                +---+---+---+---+---+
                | 3 | 9 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                 ic2         fc2            **NN pointer**
                  |           |
                  v           v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +---+---+---+---+---+---+
                | 0 | 0 | . | 1 | 1 | 5 |   **NLN ret**
                +---+---+---+---+---+---+
        
        3. *delete unnecessary zero*
        
                 ic1         fc1            **NN pointer**
                  |           |
                  v           v  
                +---+---+---+---+---+
                | 3 | 9 | . | 1 | 2 |       **NLN val1**
                +---+---+---+---+---+

                 ic2         fc2            **NN pointer**
                  |           |
                  v           v  
                +---+---+---+---+---+---+
                | 3 | 8 | . | 9 | 9 | 5 |   **NLN val2**
                +---+---+---+---+---+---+

                +---+---+---+---+---+
                | 0 | . | 1 | 1 | 5 |       **NLN ret**
                +---+---+---+---+---+

* **NumberListNode* multiply(NumberListNode* val1, NumberListNode* val2);**

* **NumberListNode* divide(NumberListNode* val1, NumberListNode* val2);**

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
    
