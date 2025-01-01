#include "infiniteNumberNode.h"

void initializeQueue(queue* q) {
    q->qHead = NULL;
    q->qTail = NULL;
}
void initializeStack(stack* s) { s->sTop = NULL; }
void initializeNumberNode(NumberNode* node) {
    node->number = 0;
    node->prev = NULL;
    node->next = NULL;
}
void initializeNumberList(NumberList* list) {
    list->head = NULL;
    list->tail = NULL;
    list->dot = NULL;
    list->op = 0;
    list->sig = 1;
}
void initializeNumberListNode(NumberListNode* node) {
    node->prev = NULL;
    node->next = NULL;
    node->value = NULL;
}
void NumberList_push_back(NumberList* nl, int val, NumberNode* now) {
    now->number = val;
    if (nl->tail == NULL) {
        nl->head = now;
        nl->tail = now;
    } else {
        nl->tail->next = now;
        if (now == NULL) {
            printf("error : NULLNumber pushed in NumberList\n");
            exit(1);
        }
        now->prev = nl->tail;
        nl->tail = now;
    }
}
void NumberList_push_front(NumberList* nl, int val, NumberNode* now) {
    now->number = val;
    if (nl->head == NULL) {
        nl->head = now;
        nl->tail = now;
    } else {
        nl->head->prev = now;
        if (now == NULL) {
            printf("error : NULLNumber pushed in NumberList\n");
            exit(1);
        }
        now->next = nl->head;
        nl->head = now;
    }
}
NumberListNode* makeNumberListNode() {
    NumberList* now = makeNumberList();
    NumberListNode* nowNode = (NumberListNode*)malloc(sizeof(NumberListNode));
    mallocAssert(nowNode);
    initializeNumberListNode(nowNode);
    nowNode->value = now;
    return nowNode;
}

NumberListNode* copyNumberListNode(NumberListNode* val) {
    NumberListNode* ret = makeNumberListNode();
    NumberNode* now = val->value->head;
    while (now != NULL) {
        NumberNode* temp = makeNumberNode();
        NumberList_push_back(ret->value, now->number, temp);
        if (now->number == DOT) ret->value->dot = temp;
        now = now->next;
    }
    return ret;
}
NumberList* makeNumberList() {
    NumberList* now = (NumberList*)malloc(sizeof(NumberList));
    mallocAssert(now);
    initializeNumberList(now);
    return now;
}
NumberNode* makeNumberNode() {
    NumberNode* now = (NumberNode*)malloc(sizeof(NumberNode));
    mallocAssert(now);
    initializeNumberNode(now);
    return now;
}

void NumberList_push_dot(NumberList* nl) {
    NumberNode* temp = makeNumberNode();
    NumberList_push_back(nl, DOT, temp);
    nl->dot = temp;
    nl->tail = temp;
}
void queue_push(queue* que, NumberListNode* nowNode) {
    if (que->qHead == NULL) {
        que->qHead = nowNode;
        que->qTail = nowNode;
    } else {
        que->qTail->next = nowNode;
        if (nowNode->value->op == 0 && nowNode->value->head == NULL) {
            printf("error : NULLList pushed in queue\n");
            exit(1);
        }
        nowNode->prev = que->qTail;
        que->qTail = nowNode;
    }
}

NumberListNode* queue_pop(queue* que) {
    if (que->qHead == NULL) {
        printf("error : empty Queue pop!!\n");
        exit(1);
    }
    NumberListNode* ret = que->qHead;
    if (que->qHead == que->qTail) {
        que->qTail = NULL;
    }
    que->qHead = que->qHead->next;
    if (que->qHead != NULL) que->qHead->prev = NULL;
    ret->prev = NULL;
    ret->next = NULL;
    return ret;
}

void stack_push(stack* stk, NumberListNode* nowNode) {
    if (stk->sTop == NULL) {
        stk->sTop = nowNode;
    } else {
        stk->sTop->next = nowNode;
        if (nowNode == NULL) {
            printf("error : NULLList pushed in stack\n");
            exit(1);
        }
        nowNode->prev = stk->sTop;
        stk->sTop = nowNode;
    }
}

NumberListNode* stack_pop(stack* stk) {
    if (stk->sTop == NULL) {
        printf("error : empty stack pop!!\n");
        exit(1);
    }
    NumberListNode* ret = stk->sTop;
    stk->sTop = stk->sTop->prev;
    if (stk->sTop != NULL) stk->sTop->next = NULL;
    ret->prev = NULL;
    ret->next = NULL;
    return ret;
}

void mallocAssert(void* p) {
    if (p == NULL) {
        printf("error : memory exceed!!\n");
        exit(1);
    }
}

void NumberList_Clear(NumberList* nl) {
    while (nl->head != NULL) {
        NumberNode* temp = nl->head;
        nl->head = nl->head->next;
        free(temp);
    }
    free(nl);
}

void NumberListNode_Clear(NumberListNode* nln) {
    NumberList_Clear(nln->value);
    free(nln);
}

void stack_push_op(stack* stk, queue* que, int opInput) {
    while (stk->sTop != NULL && (stk->sTop->value->op) != OPEN_BRACKET &&
           (stk->sTop->value->op) / 2 >= opInput / 2) {
        queue_push(que, stack_pop(stk));
    }
    NumberList* now = (NumberList*)malloc(sizeof(NumberList));
    mallocAssert(now);
    initializeNumberList(now);
    now->op = opInput;
    NumberListNode* node = makeNumberListNode();
    node->value = now;
    stack_push(stk, node);
}

void print_NumberListNode(NumberListNode* nln) {
    if (nln == NULL) {
        printf("error : number is empty\n");
        exit(1);
    }
    if (nln->value == NULL) {
        printf("error : NumberList is empty\n");
        exit(1);
    }
    if (nln->value->op == 0) {
        if (nln->value->sig == -1) printf("-");
        NumberNode* now = nln->value->head;
        while (now != NULL) {
            if (now->number == DOT) {
                if (now != nln->value->tail) printf(".");
            } else {
                printf("%d", now->number);
            }
            now = now->next;
        }
    } else {
        printf("op : %d ", nln->value->op);
    }
    printf("\n");
}

/*
 * add operation
 * The result of the calculation of val1 + val2 is stored in the
 * NumberListNode ret, and ret is returned.
 * coded by 김현식, 이동철
 */
NumberListNode* add(NumberListNode* val1, NumberListNode* val2) {
    NumberListNode* ret = makeNumberListNode();
    ret->value->sig = val1->value->sig;
    NumberNode* i_current1 =
        val1->value->dot->prev;  // Pointer used to traverse the integer part of
                                 // linked list(val1,val2)
    NumberNode* i_current2 = val2->value->dot->prev;

    NumberNode* f_current1 =
        val1->value->dot->next;  // Pointer used to traverse the decimal part of
                                 // linked list(val1,val2)
    NumberNode* f_current2 = val2->value->dot->next;

    NumberNode* now;  // Pointer used to make NumberNode for linked list(ret)

    int carry = 0;  // number used to carry

    while (f_current1 != NULL ||
           f_current2 !=
               NULL) {  // loop used to calculate decimal part of linked
                        // list(val1, val2) and add calculate result to linked
                        // list(ret) ex) 123.45+123.35, calculate result of this
                        // loop is 710(decimal part)
        int float1 = (f_current1 != NULL) ? f_current1->number : 0;
        int float2 = (f_current2 != NULL) ? f_current2->number : 0;

        int sum = float1 + float2;

        now = makeNumberNode();
        NumberList_push_back(ret->value, sum, now);

        if (f_current1 != NULL) f_current1 = f_current1->next;
        if (f_current2 != NULL) f_current2 = f_current2->next;
    }

    NumberNode* r_current =
        ret->value->tail;  // Pointer used to traverse the decimal part of
                           // linked list(ret)

    while (r_current != NULL) {  // loop used to carry decimal part of linked
                                 // list(ret) ex) 123.45+123.35, calculate
                                 // result of this loop is 80(decimal part)
        int sum = r_current->number + carry;
        carry = sum / 10;
        r_current->number = (sum) % 10;

        r_current = r_current->prev;
    }

    now = makeNumberNode();
    NumberList_push_front(
        ret->value, DOT,
        now);  // add dot to linked list(ret) ex) 123.45+123.35,
               // calculate result of this loop is .80(decimal part)
    ret->value->dot = ret->value->head;

    while (i_current1 != NULL || i_current2 != NULL ||
           carry != 0) {  // loop used to calculate and carry integer part of
                          // linked list(val1, val2) and add calculate result to
                          // linked list(ret)
        int digit1 = (i_current1 != NULL) ? i_current1->number
                                          : 0;  // ex) 123.45+123.35, calculate
                                                // result of this loop is 246.80
        int digit2 = (i_current2 != NULL) ? i_current2->number : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / 10;

        now = makeNumberNode();
        NumberList_push_front(ret->value, sum % 10, now);

        if (i_current1 != NULL) i_current1 = i_current1->prev;
        if (i_current2 != NULL) i_current2 = i_current2->prev;
    }

    delete_zero(ret);

    if (ret->value->dot == NULL) {
        NumberList_push_dot(ret->value);
    }

    NumberListNode_Clear(val1);
    NumberListNode_Clear(val2);

    return ret;
}
/*
 * subtract operation
 * The result of the calculation of val1 - val2 is stored in the
 * NumberListNode ret, and ret is returned.
 * coded by 조신붕, 후상우
 */

NumberListNode* subtract(NumberListNode* val1, NumberListNode* val2) {
    NumberListNode* ret = makeNumberListNode();
    ret->value->sig = val1->value->sig;

    NumberNode* i_current1 =
        val1->value->dot
            ->prev;  // Pointer used to traverse the first linked list
    NumberNode* i_current2 =
        val2->value->dot
            ->prev;  // Pointer for traversing the second linked list
    NumberNode* f_current1 = val1->value->dot->next;
    NumberNode* f_current2 =
        val2->value->dot->next;  // Pointer used to traverse the result list
    NumberNode* now;
    int borrow = 0;  // Used to track borrowing status

    // Iterate over numbers
    while (f_current1 != NULL || f_current2 != NULL) {
        // Gets a number, using 0 if node is NULL
        int float1 = (f_current1 != NULL) ? f_current1->number : 0;
        int float2 = (f_current2 != NULL) ? f_current2->number : 0;
        int diff = 10 + float1 - float2;

        now = makeNumberNode();
        NumberList_push_back(ret->value, diff, now);

        if (f_current1 != NULL) f_current1 = f_current1->next;
        if (f_current2 != NULL) f_current2 = f_current2->next;
    }

    NumberNode* r_current = ret->value->tail;

    while (r_current != NULL) {
        int diff = r_current->number - borrow;

        if (diff < 10) {
            borrow = 1;
        } else {
            diff = diff % 10;
            borrow = 0;
        }

        r_current->number = diff;

        r_current = r_current->prev;
    }

    now = makeNumberNode();
    NumberList_push_front(ret->value, DOT, now);
    ret->value->dot = ret->value->head;

    if (ret->value->head->number == 0 && ret->value->dot == ret->value->tail) {
        ret->value->sig = 1;
    }

    while (i_current1 != NULL) {
        int digit1 = (i_current1 != NULL) ? i_current1->number : 0;
        int digit2 = (i_current2 != NULL) ? i_current2->number : 0;

        int diff = 10 + digit1 - digit2 - borrow;
        if (diff < 10) {
            borrow = 1;
        } else {
            borrow = 0;
        }

        now = makeNumberNode();
        NumberList_push_front(ret->value, diff % 10, now);

        if (i_current1 != NULL) i_current1 = i_current1->prev;
        if (i_current2 != NULL) i_current2 = i_current2->prev;
    }
    // Remove leading zeros
    delete_zero(ret);

    if (ret->value->dot == NULL) {
        NumberList_push_dot(ret->value);
    }

    NumberListNode_Clear(val1);
    NumberListNode_Clear(val2);
    return ret;
}

/*
 * multiply operation
 * coded by 한준호, 이동철
 */
NumberListNode* multiply(NumberListNode* val1, NumberListNode* val2) {
    NumberListNode* ret = makeNumberListNode();
    ret->value->sig = val1->value->sig * val2->value->sig;
    NumberNode* initializer = makeNumberNode();
    NumberList_push_back(ret->value, 0, initializer);
    initializer = makeNumberNode();
    NumberList_push_back(ret->value, DOT, initializer);
    ret->value->dot = initializer;

    NumberListNode *sumOverDot[4], *sumUnderDot[4];
    sumOverDot[0] = copyNumberListNode(val1);
    for (int i = 1; i < 4; i++) {
        NumberListNode *temp1, *temp2;
        temp1 = copyNumberListNode(sumOverDot[i - 1]);
        temp2 = copyNumberListNode(sumOverDot[i - 1]);
        sumOverDot[i] = add(temp1, temp2);
    }
    NumberNode* now = val2->value->dot->prev;
    while (now != NULL) {
        for (int i = 0; i < 4; i++) {
            if ((now->number) & (1 << i)) {
                NumberListNode* temp = copyNumberListNode(sumOverDot[i]);
                ret = add(ret, temp);
            }
        }
        for (int i = 0; i < 4; i++) {
            product_by_10(sumOverDot[i]);
        }
        now = now->prev;
    }

    sumUnderDot[0] = copyNumberListNode(val1);
    divide_by_10(sumUnderDot[0]);
    for (int i = 1; i < 4; i++) {
        NumberListNode *temp1, *temp2;
        temp1 = copyNumberListNode(sumUnderDot[i - 1]);
        temp2 = copyNumberListNode(sumUnderDot[i - 1]);
        sumUnderDot[i] = add(temp1, temp2);
    }
    now = val2->value->dot->next;
    while (now != NULL) {
        for (int i = 0; i < 4; i++) {
            if ((now->number) & (1 << i)) {
                NumberListNode* temp = copyNumberListNode(sumUnderDot[i]);
                ret = add(ret, temp);
            }
        }
        for (int i = 0; i < 4; i++) {
            divide_by_10(sumUnderDot[i]);
        }
        now = now->next;
    }

    for (int i = 0; i < 4; i++) {
        NumberListNode_Clear(sumOverDot[i]);
        NumberListNode_Clear(sumUnderDot[i]);
    }
    NumberListNode_Clear(val1);
    NumberListNode_Clear(val2);
    return ret;
}

/*
 * divide operation
 * coded by 한준호
 */
NumberListNode* divide(NumberListNode* val1, NumberListNode* val2) {
    NumberListNode* zero = makeNumberListNode();
    NumberNode* initializer = makeNumberNode();
    NumberList_push_back(zero->value, 0, initializer);
    initializer = makeNumberNode();
    NumberList_push_back(zero->value, DOT, initializer);
    zero->value->dot = initializer;

    if (compareAbsoluteValue(zero, val2) == 0) {
        printf("error : divide by 0\n");
        exit(1);
    }
    NumberListNode* smallVal = copyNumberListNode(zero);

    for (int i = 0; i < 200; i++) {
        NumberNode* temp = makeNumberNode();
        if (i == 199)
            NumberList_push_back(smallVal->value, 1, temp);
        else
            NumberList_push_back(smallVal->value, 0, temp);
    }
    while (val1->value->tail != val1->value->dot ||
           val2->value->tail != val2->value->dot) {
        product_by_10(val1);
        product_by_10(val2);
    }
    NumberListNode* left = copyNumberListNode(smallVal);
    NumberListNode* right = copyNumberListNode(val1);
    int cnt = 0;
    while (cnt < 10000) {
        NumberListNode* temp1 = copyNumberListNode(left);
        NumberListNode* temp2 = copyNumberListNode(right);
        NumberListNode* temp3 = copyNumberListNode(smallVal);
        if (compareAbsoluteValue(add(temp1, temp3), temp2) >= 0) {
            break;
        }
        NumberListNode* L = copyNumberListNode(left);
        NumberListNode* R = copyNumberListNode(right);

        NumberListNode* mid = divide_by_2(add(L, R));
        NumberListNode* M = copyNumberListNode(mid);
        NumberListNode* VAL2 = copyNumberListNode(val2);
        if (compareAbsoluteValue(multiply(M, VAL2), val1) > 0) {
            NumberListNode_Clear(right);
            right = mid;
        } else {
            NumberListNode_Clear(left);
            left = mid;
        }
        cnt++;
    }
    right->value->sig = val1->value->sig * val2->value->sig;
    NumberListNode_Clear(val1);
    NumberListNode_Clear(val2);
    NumberListNode_Clear(left);
    return right;
}

int compareAbsoluteValue(NumberListNode* val1, NumberListNode* val2) {
    NumberNode* i_current1 =
        val1->value->dot->prev;  // Pointer used to traverse the integer part of
                                 // linked list(val1,val2)
    NumberNode* i_current2 = val2->value->dot->prev;

    NumberNode* f_current1 =
        val1->value->dot->next;  // Pointer used to traverse the decimal part of
                                 // linked list(val1,val2)
    NumberNode* f_current2 = val2->value->dot->next;

    int ret = 0;
    while (i_current1 != NULL && i_current2 != NULL) {
        if (i_current1->number > i_current2->number) {
            ret = 1;
        } else if (i_current1->number < i_current2->number) {
            ret = -1;
        }
        i_current1 = i_current1->prev;
        i_current2 = i_current2->prev;
    }
    if (i_current1 == NULL && i_current2 != NULL) {
        while (i_current2 != NULL) {
            if (0 < i_current2->number) ret = -1;
            i_current2 = i_current2->prev;
        }
    }
    if (i_current1 != NULL && i_current2 == NULL) {
        while (i_current1 != NULL) {
            if (i_current1->number > 0) ret = 1;
            i_current1 = i_current1->prev;
        }
    }
    if (ret != 0) return ret;

    while (f_current1 != NULL && f_current2 != NULL && ret == 0) {
        if (f_current1->number > f_current2->number) {
            ret = 1;
        } else if (f_current1->number < f_current2->number) {
            ret = -1;
        }
        f_current1 = f_current1->next;
        f_current2 = f_current2->next;
    }
    if (ret != 0) return ret;

    if (f_current1 == NULL && f_current2 != NULL) {
        while (f_current2 != NULL) {
            if (0 < f_current2->number) ret = -1;
            f_current2 = f_current2->next;
        }
    }
    if (f_current1 != NULL && f_current2 == NULL) {
        while (f_current1 != NULL) {
            if (f_current1->number > 0) ret = 1;
            f_current1 = f_current1->next;
        }
    }
    return ret;
}

// for debug

void print_stack_top(stack* s) {
    if (s->sTop == NULL) {
        printf("top of stack = NULL\n");
    } else {
        printf("top of stack = ");
        print_NumberListNode(s->sTop);
    }
}

void print_queue(queue* q) {
    printf("-----------------elements of queue-----------------\n");
    NumberListNode* now = q->qHead;
    while (now != NULL) {
        print_NumberListNode(now);
        now = now->next;
    }
    printf("------------------end--------------------------\n");
}

void divide_by_10(NumberListNode* val) {
    NumberNode *pprev, *prev, *now, *next;
    now = val->value->dot;
    prev = val->value->dot->prev;
    if (prev == val->value->head) {
        NumberNode* temp = makeNumberNode();
        NumberList_push_front(val->value, 0, temp);
    }
    pprev = val->value->dot->prev->prev;
    next = val->value->dot->next;
    if (next == NULL) {
        prev->next = NULL;
        prev->prev = now;
        now->next = prev;
        now->prev = pprev;
        pprev->next = now;
        val->value->tail = prev;
    } else {
        next->prev = prev;
        prev->next = next;
        prev->prev = now;
        now->next = prev;
        now->prev = pprev;
        pprev->next = now;
    }
}
void product_by_10(NumberListNode* val) {
    NumberNode *prev, *now, *next, *nnext;
    now = val->value->dot;
    prev = val->value->dot->prev;
    if (now == val->value->tail) {
        NumberNode* temp = makeNumberNode();
        temp->number = 0;
        temp->prev = prev;
        temp->next = now;
        now->prev = temp;
        prev->next = temp;
    } else if (now->next == val->value->tail) {
        next = now->next;

        next->prev = prev;
        prev->next = next;
        next->next = now;
        now->prev = next;
        now->next = NULL;

        val->value->tail = now;
    } else {
        next = now->next;
        nnext = next->next;
        prev->next = next;
        next->prev = prev;
        next->next = now;
        now->prev = next;
        now->next = nnext;
        nnext->prev = now;
    }
}
NumberListNode* divide_by_2(NumberListNode* val) {
    NumberListNode* ret = makeNumberListNode();
    NumberNode* initializer = makeNumberNode();
    NumberList_push_back(ret->value, 5, initializer);
    initializer = makeNumberNode();
    NumberList_push_back(ret->value, DOT, initializer);
    ret->value->dot = initializer;
    // 현재 ret에 5. 저장됨

    divide_by_10(val);
    ret = multiply(ret, val);
    return ret;
}

void delete_zero(NumberListNode* val) {
    while (val->value->head->number == 0 &&
           val->value->head != val->value->dot->prev) {
        NumberNode* temp = val->value->head;
        val->value->head = val->value->head->next;
        val->value->head->prev = NULL;
        if (temp == val->value->dot->prev) {
            free(temp);
            break;
        }
        free(temp);
    }

    while (val->value->tail->number == 0 &&
           val->value->tail != val->value->dot) {
        NumberNode* temp = val->value->tail;
        val->value->tail = val->value->tail->prev;
        val->value->tail->next = NULL;
        if (temp == val->value->dot) {
            val->value->dot = NULL;
            free(temp);
            break;
        }
        free(temp);
    }
}