/**
 * [1. Top-down 추상화]
 * 이 파일은 사용자가 입력한 중위 표기식(Infix)을 받아 무한 소수 연산을 수행하고 결과를 출력하는 메인 로직입니다.
 * 전체 실행 흐름은 다음과 같습니다.
 * * [Step 1: Initialization]: 스택, 큐, 임시 저장소 등 연산에 필요한 컨테이너를 초기화합니다.
 * * [Step 2: Parsing & Translation]: 사용자 입력을 한 문자씩 읽어(Streaming), 숫자와 연산자를 구분하고 Shunting-yard 알고리즘을 통해 후위 표기식(Postfix)으로 변환합니다.
 * * [Step 3: Finalize Parsing]: 입력 종료 후, 버퍼에 남은 숫자와 스택에 남은 연산자를 큐로 모두 이동시킵니다.
 * * [Step 4: Evaluation]: 완성된 후위 표기식 큐를 순회하며, 연산자(+, -, *, /)를 만날 때마다 피연산자 스택에서 값을 꺼내 계산합니다.
 * * [Step 5: Result Output]: 최종 연산 결과를 출력하고 프로그램을 종료합니다.
 */

#include "infiniteNumberNode.h"

int main() {
    char ch;
    // [실행 흐름: Step 1] 자료구조 초기화
    // [Why]: 연산자 우선순위 처리(Stack)와 후위 표기식 저장(Queue)을 위한 메모리 공간 확보.
    stack *stk = (stack *)malloc(sizeof(stack));
    queue *que = (queue *)malloc(sizeof(queue));
    initializeQueue(que);
    initializeStack(stk);

    char before = 'x'; // [Detail]: 이전 문자를 기록하여 단항 연산자나 문법 오류(연속된 연산자 등)를 감지하는 데 사용.
    int warning = 0;   // [Detail]: '0'으로 시작하는 숫자(Leading Zero) 등 잠재적 오류 상태 플래그.
    
    // [Detail]: 현재 파싱 중인 숫자를 임시 저장하는 버퍼 리스트.
    NumberListNode *nowNumberListNode = makeNumberListNode(); 

    // [실행 흐름: Step 2] 입력 파싱 루프 (Infix -> Postfix 변환)
    // [How]: getchar()로 스트림에서 문자 하나씩 읽어 처리 (O(N)).
    while ((ch = (char)getchar()) != EOF) {
        if (ch == ' ' || ch == '\t' || ch == '\n') {
            continue; // 공백 무시
        }

        // Leading Zero 에러 체크 (예: 05)
        if (warning == 1) {
            if (ch != '.') {
                printf("error : leading zero!!\n");
                exit(1);
            } else {
                warning = 0;
            }
        }

        // Case 1: 숫자 입력 (0~9)
        if (ch >= '0' && ch <= '9') {
            // [Detail]: 닫는 괄호 ')' 바로 뒤에 숫자가 오면 곱셈 생략으로 간주 (예: (3+4)5 -> (3+4)*5)
            if (before == ')') {
                stack_push_op(stk, que, MUL);
            } 
            // [Detail]: 연산자 뒤에 0이 오면 Leading Zero 가능성 체크
            else if ((before == 'x' || before == '+' || before == '-' ||
                        before == '*' || before == '/' || before == '(' ||
                        before == ')') &&
                       ch == '0') {
                warning = 1;
            }
            // [How]: 현재 숫자의 자릿수를 리스트 뒤에 추가
            NumberNode *temp = makeNumberNode();
            NumberList_push_back(nowNumberListNode->value, (int)(ch - '0'), temp);
        } 
        // Case 2: 소수점 입력 (.)
        else if (ch == '.') {
            if (nowNumberListNode->value->dot != NULL) { // [Why]: 소수점이 두 번 나오면 에러 (예: 3.5.2)
                printf("error : Invalid operator!!1%c\n", ch);
                exit(1);
            }
            NumberList_push_dot(nowNumberListNode->value);
        } 
        // Case 3: 사칙연산자 입력 (+, -, *, /)
        else if (ch == '*' || ch == '/' || ch == '+' || ch == '-') {
            //////////////////////////
            // [Detail]: 단항 연산자 처리 (예: -5). 이전 문자가 괄호나 시작점이면 0을 추가하여 0-5로 변환.
            if (before == '(' || before == 'x') {
                NumberNode *temp = makeNumberNode();
                NumberList_push_back(nowNumberListNode->value, 0, temp);
            }
            //////////////////////////
            
            // 숫자가 완성되었으므로 소수점 처리 및 큐 삽입
            if (nowNumberListNode->value->head != NULL &&
                nowNumberListNode->value->dot == NULL) {
                NumberList_push_dot(nowNumberListNode->value); // 정수일 경우 소수점 자동 추가
            }
            if (nowNumberListNode->value->head != NULL) {
                queue_push(que, nowNumberListNode); // [How]: 파싱된 숫자를 큐로 이동
                nowNumberListNode = makeNumberListNode(); // 새 숫자 버퍼 생성
            }

            int op = 0;
            switch (ch) {
                case '*': op = MUL; break;
                case '/': op = DIV; break;
                case '+': op = ADD; break;
                case '-': op = SUB; break;
            }
            // [How]: Shunting-yard 알고리즘에 따라 연산자 스택 처리
            stack_push_op(stk, que, op);
        } 
        // Case 4: 여는 괄호 '('
        else if (ch == '(') {
            // [Detail]: 숫자나 닫는 괄호 뒤에 '('가 오면 곱셈 생략 (예: 3(4+5) -> 3*(4+5))
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
        } 
        // Case 5: 닫는 괄호 ')'
        else if (ch == ')') {
            // 괄호 닫기 전 현재 숫자 마무리 처리
            if (nowNumberListNode->value->head != NULL &&
                nowNumberListNode->value->dot == NULL) {
                NumberList_push_dot(nowNumberListNode->value);
            }
            if (nowNumberListNode->value->head != NULL) {
                queue_push(que, nowNumberListNode);
                nowNumberListNode = makeNumberListNode();
            }

            // [How]: 여는 괄호를 만날 때까지 스택의 연산자들을 큐로 Pop
            NumberListNode *temp;
            while ((temp = stack_pop(stk))->value->op != OPEN_BRACKET) {
                queue_push(que, temp);
                if (stk->sTop == NULL) { // 스택이 비었는데 여는 괄호가 없으면 괄호 불일치 에러
                    printf("error : Invalid Operation2!!\n");
                    exit(1);
                }
            }
            NumberListNode_Clear(temp); // 여는 괄호 노드는 버림
        } else {
            printf("error : Invalid operator3!!%c\n", ch);
            exit(1);
        }
        before = ch; // 이전 문자 갱신
    }

    // [실행 흐름: Step 3] 파싱 종료 후 잔여 데이터 처리
    // 1. 마지막에 입력중이던 숫자 처리
    if (nowNumberListNode->value->head != NULL &&
        nowNumberListNode->value->dot == NULL) {
        NumberList_push_dot(nowNumberListNode->value);
    }
    if (nowNumberListNode->value->head != NULL) {
        queue_push(que, nowNumberListNode);
        nowNumberListNode = makeNumberListNode();
    }
    // 2. 스택에 남은 연산자들 모두 큐로 이동
    while (stk->sTop != NULL) queue_push(que, stack_pop(stk));

    // [실행 흐름: Step 4] 후위 표기식 계산 (Evaluation)
    // [Why]: 후위 표기식은 순차적으로 스캔하며 피연산자는 스택에 넣고, 연산자를 만나면 계산하는 방식으로 괄호 없이 처리 가능.
    stack *operand = (stack *)malloc(sizeof(stack)); // 피연산자 저장용 스택
    mallocAssert(operand);
    initializeStack(operand);
    
    NumberListNode *pprev = NULL, *prev = NULL; // 피연산자 두 개를 담을 포인터 (prev: 우변, pprev: 좌변)

    while (que->qHead != NULL) {
        NumberListNode *now = queue_pop(que); // 큐에서 토큰 하나 꺼냄
        
        if (now->value->op == 0) {
            // [Case]: 피연산자(숫자)인 경우 스택에 Push
            stack_push(operand, now);
        } else {
            // [Case]: 연산자인 경우 스택에서 두 숫자를 Pop하여 계산
            prev = stack_pop(operand);  // 두 번째 피연산자 (B)
            pprev = stack_pop(operand); // 첫 번째 피연산자 (A)
            
            if (prev == NULL || pprev == NULL) {
                printf("error : Invalid Operation5!!\n");
                exit(1);
            }
            
            int op = now->value->op;
            // [Detail]: 부호(sig)와 연산 종류에 따른 분기 처리.
            // 덧셈 함수(add)는 양수끼리의 덧셈만 구현되어 있으므로, 음수 연산은 뺄셈(subtract) 등으로 변환하여 호출.
            switch (op) {
                case ADD:
                    if (pprev->value->sig == 1 && prev->value->sig == 1) {
                        stack_push(operand, add(pprev, prev)); // (+A) + (+B)
                    } else if (pprev->value->sig == -1 && prev->value->sig == 1) {
                        // (-A) + (+B) -> |A| <= |B| ? (B - A) : -(A - B)
                        if (compareAbsoluteValue(pprev, prev) <= 0) {
                            pprev->value->sig *= -1; // A 부호 반전
                            stack_push(operand, subtract(prev, pprev));
                        } else {
                            stack_push(operand, subtract(pprev, prev));
                        }
                    } else if (pprev->value->sig == 1 && prev->value->sig == -1) {
                        // (+A) + (-B) -> |A| <= |B| ? -(B - A) : (A - B)
                        if (compareAbsoluteValue(pprev, prev) <= 0) {
                            stack_push(operand, subtract(prev, pprev));
                        } else {
                            prev->value->sig *= -1; // B 부호 반전
                            stack_push(operand, subtract(pprev, prev));
                        }
                    } else { // (-A) + (-B) -> -(A + B)
                        stack_push(operand, add(pprev, prev)); // 결과 부호는 add 내부에서 A의 부호(-1)를 따라감
                    }
                    break;

                case SUB:
                    if (pprev->value->sig == 1 && prev->value->sig == 1) {
                        // (+A) - (+B)
                        if (compareAbsoluteValue(pprev, prev) <= 0) {
                            prev->value->sig *= -1; // 결과가 음수
                            stack_push(operand, subtract(prev, pprev)); // -(B - A)
                        } else {
                            stack_push(operand, subtract(pprev, prev)); // A - B
                        }
                    } else if (pprev->value->sig == -1 && prev->value->sig == 1) {
                        // (-A) - (+B) -> -(A + B)
                        prev->value->sig *= -1;
                        stack_push(operand, add(pprev, prev));
                    } else if (pprev->value->sig == 1 && prev->value->sig == -1) {
                        // (+A) - (-B) -> A + B
                        prev->value->sig *= -1;
                        stack_push(operand, add(pprev, prev));
                    } else {
                        // (-A) - (-B) -> -A + B -> B - A
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
            pprev = NULL; // 사용 완료 포인터 초기화 (Safety)
        }
    }

    // [실행 흐름: Step 5] 최종 결과 출력
    if (pprev != NULL) { // 연산 후 잔여 데이터가 있으면 논리 오류
        printf("error : Invalid Operation6!!\n");
        exit(1);
    }
    // 스택에 남은 최후의 노드가 최종 결과값
    print_NumberListNode(stack_pop(operand));
    return 0;
}
