#include "infiniteNumberNode.h"

int main() {
    char ch;
    stack *stk = (stack *)malloc(sizeof(stack));
    queue *que = (queue *)malloc(sizeof(queue));
    initializeQueue(que);
    initializeStack(stk);
    char before = 'x';
    int warning = 0;
    NumberListNode *nowNumberListNode = makeNumberListNode();
    while ((ch = (char)getchar()) != EOF) {
        if (ch == ' ' || ch == '\t' || ch == '\n') {
            continue;
        }
        if (warning == 1) {
            if (ch != '.') {
                printf("error : leading zero!!\n");
                exit(1);
            } else {
                warning = 0;
            }
        }
        if (ch >= '0' && ch <= '9') {
            if (before == ')') {
                stack_push_op(stk, que, MUL);
            } else if ((before == 'x' || before == '+' || before == '-' ||
                        before == '*' || before == '/' || before == '(' ||
                        before == ')') &&
                       ch == '0') {
                warning = 1;
            }
            NumberNode *temp = makeNumberNode();
            NumberList_push_back(nowNumberListNode->value, (int)(ch - '0'),
                                 temp);
        } else if (ch == '.') {
            if (nowNumberListNode->value->dot != NULL) {
                printf("error : Invalid operator!!1%c\n", ch);
                exit(1);
            }
            NumberList_push_dot(nowNumberListNode->value);
        } else if (ch == '*' || ch == '/' || ch == '+' || ch == '-') {
            //////////////////////////
            if (before == '(' || before == 'x') {
                NumberNode *temp = makeNumberNode();
                NumberList_push_back(nowNumberListNode->value, 0, temp);
            }
            //////////////////////////
            if (nowNumberListNode->value->head != NULL &&
                nowNumberListNode->value->dot == NULL) {
                NumberList_push_dot(nowNumberListNode->value);
            }
            if (nowNumberListNode->value->head != NULL) {
                queue_push(que, nowNumberListNode);
                nowNumberListNode = makeNumberListNode();
            }
            int op = 0;
            switch (ch) {
                case '*':
                    op = MUL;
                    break;
                case '/':
                    op = DIV;
                    break;
                case '+':
                    op = ADD;
                    break;
                case '-':
                    op = SUB;
                    break;
            }
            stack_push_op(stk, que, op);
        } else if (ch == '(') {
            if (before >= '0' && before <= '9') {
                if (nowNumberListNode->value->head != NULL &&
                    nowNumberListNode->value->dot == NULL) {
                    NumberList_push_dot(nowNumberListNode->value);
                }
                if (nowNumberListNode->value->head != NULL) {
                    queue_push(que, nowNumberListNode);
                    nowNumberListNode = makeNumberListNode();
                }

                stack_push_op(stk, que, MUL);
            } else if (before == ')') {
                stack_push_op(stk, que, MUL);
            }
            stack_push_op(stk, que, OPEN_BRACKET);
        } else if (ch == ')') {
            if (nowNumberListNode->value->head != NULL &&
                nowNumberListNode->value->dot == NULL) {
                NumberList_push_dot(nowNumberListNode->value);
            }
            if (nowNumberListNode->value->head != NULL) {
                queue_push(que, nowNumberListNode);
                nowNumberListNode = makeNumberListNode();
            }

            NumberListNode *temp;
            while ((temp = stack_pop(stk))->value->op != OPEN_BRACKET) {
                queue_push(que, temp);
                if (stk->sTop == NULL) {
                    printf("error : Invalid Operation2!!\n");
                    exit(1);
                }
            }
            NumberListNode_Clear(temp);
        } else {
            printf("error : Invalid operator3!!%c\n", ch);
            exit(1);
        }
        before = ch;
        // debug
        // print_queue(que);
        //
    }

    if (nowNumberListNode->value->head != NULL &&
        nowNumberListNode->value->dot == NULL) {
        NumberList_push_dot(nowNumberListNode->value);
    }
    if (nowNumberListNode->value->head != NULL) {
        queue_push(que, nowNumberListNode);
        nowNumberListNode = makeNumberListNode();
    }
    while (stk->sTop != NULL) queue_push(que, stack_pop(stk));

    // debug
    // print_queue(que);
    //
    stack *operand = (stack *)malloc(sizeof(stack));
    mallocAssert(operand);
    initializeStack(operand);
    NumberListNode *pprev = NULL, *prev = NULL;
    while (que->qHead != NULL) {
        NumberListNode *now = queue_pop(que);
        // printf("now popped on queue : ");
        // print_NumberListNode(now);
        if (now->value->op == 0) {
            stack_push(operand, now);
        } else {
            // pprev op prev
            prev = stack_pop(operand);
            pprev = stack_pop(operand);
            if (prev == NULL || pprev == NULL) {
                printf("error : Invalid Operation5!!\n");
                exit(1);
            }
            int op = now->value->op;
            switch (op) {
                case ADD:
                    if (pprev->value->sig == 1 && prev->value->sig == 1) {
                        stack_push(operand, add(pprev, prev));
                    } else if (pprev->value->sig == -1 &&
                               prev->value->sig == 1) {
                        if (compareAbsoluteValue(pprev, prev) <= 0) {
                            pprev->value->sig *= -1;
                            stack_push(operand, subtract(prev, pprev));
                        } else {
                            stack_push(operand, subtract(pprev, prev));
                        }
                    } else if (pprev->value->sig == 1 &&
                               prev->value->sig == -1) {
                        if (compareAbsoluteValue(pprev, prev) <= 0) {
                            stack_push(operand, subtract(prev, pprev));
                        } else {
                            prev->value->sig *= -1;
                            stack_push(operand, subtract(pprev, prev));
                        }
                    } else {
                        stack_push(operand, add(pprev, prev));
                    }
                    break;
                case SUB:
                    if (pprev->value->sig == 1 && prev->value->sig == 1) {
                        if (compareAbsoluteValue(pprev, prev) <= 0) {
                            prev->value->sig *= -1;
                            stack_push(operand, subtract(prev, pprev));
                        } else {
                            stack_push(operand, subtract(pprev, prev));
                        }
                    } else if (pprev->value->sig == -1 &&
                               prev->value->sig == 1) {
                        prev->value->sig *= -1;
                        stack_push(operand, add(pprev, prev));
                    } else if (pprev->value->sig == 1 &&
                               prev->value->sig == -1) {
                        prev->value->sig *= -1;
                        stack_push(operand, add(pprev, prev));
                    } else {
                        if (compareAbsoluteValue(pprev, prev) <= 0) {
                            prev->value->sig *= -1;
                            pprev->value->sig *= -1;
                            stack_push(operand, subtract(prev, pprev));
                        } else {
                            prev->value->sig *= -1;
                            stack_push(operand, subtract(pprev, prev));
                        }
                    }
                    break;
                case MUL:
                    stack_push(operand, multiply(pprev, prev));
                    break;
                case DIV:
                    stack_push(operand, divide(pprev, prev));
                    break;
            }
            pprev = NULL;
        }
        // print_stack_top(operand);
    }
    if (pprev != NULL) {
        printf("error : Invalid Operation6!!\n");
        exit(1);
    }
    print_NumberListNode(stack_pop(operand));
    return 0;
}