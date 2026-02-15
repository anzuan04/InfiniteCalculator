/**
 * [1. Top-down 추상화]
 * 이 소스 파일은 무한 정밀도 실수의 생성, 관리, 그리고 사칙연산을 수행하는 핵심 구현부입니다.
 * 전체 로직은 데이터의 생명 주기에 따라 다음 단계로 구성됩니다.
 * * [Step 1: Initialization & Memory]: 리스트, 노드, 스택, 큐의 메모리 할당 및 초기화 (생성자 역할).
 * * [Step 2: Data Manipulation]: 연결 리스트의 앞/뒤에 노드를 추가하거나 복사하는 유틸리티 함수 (CRUD).
 * * [Step 3: Basic Arithmetic (Add/Sub)]: 자릿수별 순회를 통한 덧셈과 뺄셈 구현 (O(N)). Carry/Borrow 처리 포함.
 * * [Step 4: Advanced Arithmetic (Mul/Div)]: 
 * - 곱셈: 각 자릿수를 비트 단위(1, 2, 4, 8)로 분해하여 덧셈과 시프트 연산으로 변환 (최적화된 방식).
 * - 나눗셈: 이진 탐색(Binary Search)을 사용하여 몫을 근사하는 Divide & Conquer 알고리즘 적용.
 * * [Step 5: Utility & Cleanup]: 결과 출력, 디버깅, 0 제거, 메모리 해제 등 보조 기능.
 */

#include "infiniteNumberNode.h"

// [실행 흐름: Step 1] 초기화 및 구조체 설정
// [Detail]: 포인터 초기화를 통해 쓰레기 값으로 인한 세그멘테이션 폴트 방지.
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
    list->sig = 1; // [Why]: 부호의 기본값은 양수(1)로 설정하여 곱셈 등의 연산 시 1 * -1 = -1 등 부호 논리 유지.
}
void initializeNumberListNode(NumberListNode* node) {
    node->prev = NULL;
    node->next = NULL;
    node->value = NULL;
}

// [실행 흐름: Step 2] 리스트 후단(Tail)에 노드 추가
// [Why]: 일반적인 숫자 입력(예: 123)이나 소수점 아래 자릿수 확장에 사용됨.
void NumberList_push_back(NumberList* nl, int val, NumberNode* now) {
    now->number = val;
    if (nl->tail == NULL) { // [Detail]: 리스트가 비어있을 경우 Head와 Tail 모두 현재 노드를 가리킴.
        nl->head = now;
        nl->tail = now;
    } else {
        nl->tail->next = now; // [How]: 기존 Tail 뒤에 연결 $Tail \rightarrow Now$
        if (now == NULL) {
            printf("error : NULLNumber pushed in NumberList\n");
            exit(1);
        }
        now->prev = nl->tail; // [How]: 양방향 연결 $Tail \leftarrow Now$
        nl->tail = now;       // [Detail]: Tail 포인터 갱신
    }
}

// [실행 흐름: Step 2] 리스트 전단(Head)에 노드 추가
// [Why]: 덧셈의 올림(Carry)이나 뺄셈 결과 처리 시 높은 자릿수로 확장할 때 사용.
void NumberList_push_front(NumberList* nl, int val, NumberNode* now) {
    now->number = val;
    if (nl->head == NULL) {
        nl->head = now;
        nl->tail = now;
    } else {
        nl->head->prev = now; // [How]: 기존 Head 앞에 연결 $Now \leftarrow Head$
        if (now == NULL) {
            printf("error : NULLNumber pushed in NumberList\n");
            exit(1);
        }
        now->next = nl->head; // [How]: 양방향 연결 $Now \rightarrow Head$
        nl->head = now;       // [Detail]: Head 포인터 갱신
    }
}

// [실행 흐름: Step 1] 래퍼 노드 생성 (Factory Pattern 유사)
NumberListNode* makeNumberListNode() {
    NumberList* now = makeNumberList();
    NumberListNode* nowNode = (NumberListNode*)malloc(sizeof(NumberListNode));
    mallocAssert(nowNode);
    initializeNumberListNode(nowNode);
    nowNode->value = now;
    return nowNode;
}

// [실행 흐름: Step 2] 깊은 복사 (Deep Copy)
// [Why]: 연산 시 피연산자가 수정되는 것을 막고, 독립적인 결과 객체를 생성하기 위함.
// [Detail]: 시간 복잡도 O(N), N은 자릿수.
NumberListNode* copyNumberListNode(NumberListNode* val) {
    NumberListNode* ret = makeNumberListNode();
    NumberNode* now = val->value->head;
    while (now != NULL) {
        NumberNode* temp = makeNumberNode();
        NumberList_push_back(ret->value, now->number, temp);
        // [Detail]: 원본의 소수점(DOT) 위치를 감지하여 복사본에도 동일하게 마킹
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

// [실행 흐름: Step 2] 소수점 노드 추가
// [Why]: 정수부와 소수부를 구분하는 앵커(Anchor) 역할.
void NumberList_push_dot(NumberList* nl) {
    NumberNode* temp = makeNumberNode();
    NumberList_push_back(nl, DOT, temp);
    nl->dot = temp; // [Detail]: 추후 연산에서 빠른 접근을 위해 dot 포인터 저장.
    nl->tail = temp;
}

// [실행 흐름: Step 2] 큐(Queue) 삽입 연산
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

// [실행 흐름: Step 2] 큐(Queue) 추출 연산
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

// [실행 흐름: Step 2] 스택(Stack) 삽입 연산
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

// [실행 흐름: Step 2] 스택(Stack) 추출 연산
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

// [실행 흐름: Step 5] 메모리 할당 검증
void mallocAssert(void* p) {
    if (p == NULL) {
        printf("error : memory exceed!!\n");
        exit(1);
    }
}

// [실행 흐름: Step 5] 리스트 전체 메모리 해제
// [Detail]: 연결 리스트 특성상 노드 하나하나를 순회하며 free 해야 함 (O(N)).
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

// [실행 흐름: Step 2] 연산자 우선순위 처리 (Shunting-yard Helper)
void stack_push_op(stack* stk, queue* que, int opInput) {
    // [How]: 스택 상단의 연산자 우선순위가 들어오는 연산자보다 높거나 같으면 Pop하여 큐로 이동.
    // (ADD, SUB: 2, 3 -> 나누기 2 하면 1) / (MUL, DIV: 4, 5 -> 나누기 2 하면 2)
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

// [실행 흐름: Step 5] 디버깅용 출력
void print_NumberListNode(NumberListNode* nln) {
    if (nln == NULL) { printf("error : number is empty\n"); exit(1); }
    if (nln->value == NULL) { printf("error : NumberList is empty\n"); exit(1); }
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

/* [실행 흐름: Step 3] 덧셈 연산 (Add)
 * [Why]: 임의 정밀도 숫자의 가장 기본 연산. 자릿수 맞춤과 Carry 처리가 핵심.
 * [Detail]: 시간 복잡도 $O(N)$. */
NumberListNode* add(NumberListNode* val1, NumberListNode* val2) {
    NumberListNode* ret = makeNumberListNode();
    ret->value->sig = val1->value->sig; // [Note]: 부호 로직은 호출자(main)에서 처리됨.

    // 소수점(DOT)을 기준으로 정수부는 역방향(prev), 소수부는 정방향(next)으로 탐색 준비
    NumberNode* i_current1 = val1->value->dot->prev; 
    NumberNode* i_current2 = val2->value->dot->prev;
    NumberNode* f_current1 = val1->value->dot->next; 
    NumberNode* f_current2 = val2->value->dot->next;
    NumberNode* now;
    int carry = 0; 

    // 1. 소수부 덧셈 (왼쪽 -> 오른쪽으로 리스트 생성)
    while (f_current1 != NULL || f_current2 != NULL) {
        int float1 = (f_current1 != NULL) ? f_current1->number : 0;
        int float2 = (f_current2 != NULL) ? f_current2->number : 0;
        int sum = float1 + float2; // [Note]: 여기서는 carry를 바로 계산하지 않고 저장만 함.

        now = makeNumberNode();
        NumberList_push_back(ret->value, sum, now);

        if (f_current1 != NULL) f_current1 = f_current1->next;
        if (f_current2 != NULL) f_current2 = f_current2->next;
    }

    // 2. 소수부 Carry 후처리 (오른쪽 -> 왼쪽으로 역방향 순회)
    NumberNode* r_current = ret->value->tail; 
    while (r_current != NULL) {  
        int sum = r_current->number + carry;
        carry = sum / 10;           // [How]: $sum \div 10$ -> 다음 자리 올림수
        r_current->number = sum % 10; // [How]: $sum \pmod{10}$ -> 현재 자리 값
        r_current = r_current->prev;
    }

    // 3. 소수점 삽입
    now = makeNumberNode();
    NumberList_push_front(ret->value, DOT, now); 
    ret->value->dot = ret->value->head;

    // 4. 정수부 덧셈 (1의 자리부터 시작하여 위로 올라감)
    while (i_current1 != NULL || i_current2 != NULL || carry != 0) { 
        int digit1 = (i_current1 != NULL) ? i_current1->number : 0;
        int digit2 = (i_current2 != NULL) ? i_current2->number : 0;
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;

        now = makeNumberNode();
        NumberList_push_front(ret->value, sum % 10, now); // Head 쪽에 계속 추가

        if (i_current1 != NULL) i_current1 = i_current1->prev;
        if (i_current2 != NULL) i_current2 = i_current2->prev;
    }

    delete_zero(ret); // [Detail]: 불필요한 0 제거 (003.50 -> 3.5)
    if (ret->value->dot == NULL) NumberList_push_dot(ret->value);

    NumberListNode_Clear(val1); // [Detail]: 피연산자 메모리 해제
    NumberListNode_Clear(val2);
    return ret;
}

/* [실행 흐름: Step 3] 뺄셈 연산 (Subtract)
 * [Why]: $A - B$ 수행 시 $10 + A - B$ 형태의 빌림(Borrow) 처리 필요. */
NumberListNode* subtract(NumberListNode* val1, NumberListNode* val2) {
    NumberListNode* ret = makeNumberListNode();
    ret->value->sig = val1->value->sig;

    NumberNode* i_current1 = val1->value->dot->prev;
    NumberNode* i_current2 = val2->value->dot->prev;
    NumberNode* f_current1 = val1->value->dot->next;
    NumberNode* f_current2 = val2->value->dot->next;
    NumberNode* now;
    int borrow = 0; 

    // 1. 소수부 뺄셈 (일단 값 계산 후 저장)
    while (f_current1 != NULL || f_current2 != NULL) {
        int float1 = (f_current1 != NULL) ? f_current1->number : 0;
        int float2 = (f_current2 != NULL) ? f_current2->number : 0;
        int diff = 10 + float1 - float2; // [How]: 일단 10을 더해 음수 방지

        now = makeNumberNode();
        NumberList_push_back(ret->value, diff, now);

        if (f_current1 != NULL) f_current1 = f_current1->next;
        if (f_current2 != NULL) f_current2 = f_current2->next;
    }

    // 2. 소수부 Borrow 처리 (뒤에서 앞으로)
    NumberNode* r_current = ret->value->tail;
    while (r_current != NULL) {
        int diff = r_current->number - borrow;
        if (diff < 10) { // [Detail]: 원래 값보다 10을 더했음에도 10 미만이면 실제 Borrow가 필요했던 상황
            borrow = 1;
        } else {
            diff = diff % 10; // [Detail]: 10 이상이면 Borrow 필요 없음, 1의 자리만 취함
            borrow = 0;
        }
        r_current->number = diff;
        r_current = r_current->prev;
    }

    // 3. 소수점 처리
    now = makeNumberNode();
    NumberList_push_front(ret->value, DOT, now);
    ret->value->dot = ret->value->head;

    // 결과가 0.0일 경우 부호 조정
    if (ret->value->head->number == 0 && ret->value->dot == ret->value->tail) {
        ret->value->sig = 1;
    }

    // 4. 정수부 뺄셈
    while (i_current1 != NULL) {
        int digit1 = (i_current1 != NULL) ? i_current1->number : 0;
        int digit2 = (i_current2 != NULL) ? i_current2->number : 0;

        int diff = 10 + digit1 - digit2 - borrow; // [How]: 기본적으로 10을 빌려왔다고 가정
        if (diff < 10) {
            borrow = 1; // 실제 빌림 발생
        } else {
            borrow = 0; // 빌림 없음
        }

        now = makeNumberNode();
        NumberList_push_front(ret->value, diff % 10, now);

        if (i_current1 != NULL) i_current1 = i_current1->prev;
        if (i_current2 != NULL) i_current2 = i_current2->prev;
    }
    
    delete_zero(ret);
    if (ret->value->dot == NULL) NumberList_push_dot(ret->value);

    NumberListNode_Clear(val1);
    NumberListNode_Clear(val2);
    return ret;
}

/* [실행 흐름: Step 4] 곱셈 연산 (Multiply)
 * [Why]: 무한 자릿수 곱셈을 효율적으로 수행하기 위해 비트 분해 방식 사용.
 * [Detail]: 각 자릿수를 1, 2, 4, 8의 합으로 표현하여 미리 계산된 배수들을 더함 (Shift & Add 방식 응용). */
NumberListNode* multiply(NumberListNode* val1, NumberListNode* val2) {
    NumberListNode* ret = makeNumberListNode();
    ret->value->sig = val1->value->sig * val2->value->sig; // [How]: $sig = s1 \times s2$
    
    // 초기값 0.0 설정
    NumberNode* initializer = makeNumberNode();
    NumberList_push_back(ret->value, 0, initializer);
    initializer = makeNumberNode();
    NumberList_push_back(ret->value, DOT, initializer);
    ret->value->dot = initializer;

    // [Detail]: sumOverDot[i]에는 val1 * (2^i) 값이 저장됨 (i=0..3 -> x1, x2, x4, x8)
    NumberListNode *sumOverDot[4], *sumUnderDot[4];
    sumOverDot[0] = copyNumberListNode(val1);
    for (int i = 1; i < 4; i++) {
        NumberListNode *temp1, *temp2;
        temp1 = copyNumberListNode(sumOverDot[i - 1]);
        temp2 = copyNumberListNode(sumOverDot[i - 1]);
        sumOverDot[i] = add(temp1, temp2); // [How]: 전 단계 값을 더해서 2배 생성
    }

    // 1. 정수부 곱셈 처리
    NumberNode* now = val2->value->dot->prev;
    while (now != NULL) {
        for (int i = 0; i < 4; i++) {
            // [How]: 현재 자릿수(now->number)의 비트가 켜져 있으면 해당 배수(x1, x2, x4, x8)를 더함
            // 예: 숫자가 6(0110)이면 x2와 x4를 더함 -> 6배
            if ((now->number) & (1 << i)) {
                NumberListNode* temp = copyNumberListNode(sumOverDot[i]);
                ret = add(ret, temp);
            }
        }
        // [Detail]: 다음 자릿수로 넘어가기 전 기준값들을 10배씩 키움 (자릿수 시프트)
        for (int i = 0; i < 4; i++) {
            product_by_10(sumOverDot[i]);
        }
        now = now->prev;
    }

    // 2. 소수부 곱셈 처리 준비 (val1 * 0.1 등)
    sumUnderDot[0] = copyNumberListNode(val1);
    divide_by_10(sumUnderDot[0]); // [How]: 초기값 x0.1
    for (int i = 1; i < 4; i++) {
        NumberListNode *temp1, *temp2;
        temp1 = copyNumberListNode(sumUnderDot[i - 1]);
        temp2 = copyNumberListNode(sumUnderDot[i - 1]);
        sumUnderDot[i] = add(temp1, temp2);
    }

    // 3. 소수부 곱셈 처리
    now = val2->value->dot->next;
    while (now != NULL) {
        for (int i = 0; i < 4; i++) {
            if ((now->number) & (1 << i)) {
                NumberListNode* temp = copyNumberListNode(sumUnderDot[i]);
                ret = add(ret, temp);
            }
        }
        // [Detail]: 다음 소수 자릿수로 가기 위해 기준값들을 1/10배 줄임
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

/* [실행 흐름: Step 4] 나눗셈 연산 (Divide)
 * [Why]: 나눗셈은 직접 구현이 어려우므로 이진 탐색(Binary Search)으로 몫을 근사함.
 * [Detail]: $val1 / val2 = X$를 찾는 과정 -> $X * val2 \approx val1$이 되는 $X$를 탐색. */
NumberListNode* divide(NumberListNode* val1, NumberListNode* val2) {
    // 0.0 생성 (비교 및 초기화용)
    NumberListNode* zero = makeNumberListNode();
    NumberNode* initializer = makeNumberNode();
    NumberList_push_back(zero->value, 0, initializer);
    initializer = makeNumberNode();
    NumberList_push_back(zero->value, DOT, initializer);
    zero->value->dot = initializer;

    // Divide by 0 예외 처리
    if (compareAbsoluteValue(zero, val2) == 0) {
        printf("error : divide by 0\n");
        exit(1);
    }
    
    // [Detail]: smallVal은 탐색의 정밀도(종료 조건)를 결정 (0.00...01)
    NumberListNode* smallVal = copyNumberListNode(zero);
    for (int i = 0; i < 200; i++) { // 소수점 아래 200자리까지 정밀도 보장
        NumberNode* temp = makeNumberNode();
        if (i == 199) NumberList_push_back(smallVal->value, 1, temp);
        else NumberList_push_back(smallVal->value, 0, temp);
    }

    // [Step 4-1] 소수점 제거 (정수 나눗셈 형태로 변환하여 계산 편의성 증대)
    while (val1->value->tail != val1->value->dot ||
           val2->value->tail != val2->value->dot) {
        product_by_10(val1);
        product_by_10(val2);
    }

    // [Step 4-2] 이진 탐색 초기화
    NumberListNode* left = copyNumberListNode(smallVal); // 범위 하한
    NumberListNode* right = copyNumberListNode(val1);    // 범위 상한 (피제수 자체로 초기화)
    int cnt = 0;

    // [Step 4-3] 이진 탐색 루프 (최대 10000회)
    while (cnt < 10000) {
        NumberListNode* temp1 = copyNumberListNode(left);
        NumberListNode* temp2 = copyNumberListNode(right);
        NumberListNode* temp3 = copyNumberListNode(smallVal);
        
        // [Condition]: 좌우 범위 차이가 smallVal보다 작으면 수렴한 것으로 판단하고 종료
        if (compareAbsoluteValue(add(temp1, temp3), temp2) >= 0) {
            break;
        }

        NumberListNode* L = copyNumberListNode(left);
        NumberListNode* R = copyNumberListNode(right);
        NumberListNode* mid = divide_by_2(add(L, R)); // $Mid = (L+R)/2$

        NumberListNode* M = copyNumberListNode(mid);
        NumberListNode* VAL2 = copyNumberListNode(val2);
        
        // [Check]: $Mid \times Divisor$가 Dividend보다 크면, 몫은 더 작아야 함 -> Right를 줄임
        if (compareAbsoluteValue(multiply(M, VAL2), val1) > 0) {
            NumberListNode_Clear(right);
            right = mid;
        } else { // 몫은 더 커야 함 -> Left를 키움
            NumberListNode_Clear(left);
            left = mid;
        }
        cnt++;
    }
    right->value->sig = val1->value->sig * val2->value->sig; // 부호 결정
    
    NumberListNode_Clear(val1);
    NumberListNode_Clear(val2);
    NumberListNode_Clear(left);
    return right;
}

// [실행 흐름: Step 5] 절대값 비교 함수
int compareAbsoluteValue(NumberListNode* val1, NumberListNode* val2) {
    NumberNode* i_current1 = val1->value->dot->prev;
    NumberNode* i_current2 = val2->value->dot->prev;
    NumberNode* f_current1 = val1->value->dot->next;
    NumberNode* f_current2 = val2->value->dot->next;

    int ret = 0;
    // 1. 정수부 비교 (높은 자릿수부터)
    while (i_current1 != NULL && i_current2 != NULL) {
        if (i_current1->number > i_current2->number) {
            ret = 1;
        } else if (i_current1->number < i_current2->number) {
            ret = -1;
        }
        i_current1 = i_current1->prev;
        i_current2 = i_current2->prev;
    }
    // 자릿수 길이가 다른 경우 처리
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
    if (ret != 0) return ret; // 정수부에서 결판나면 리턴

    // 2. 소수부 비교 (높은 자릿수부터)
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

    // 소수부 길이 차이 처리
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

// [실행 흐름: Step 5] 유틸리티: 10으로 나누기 (Shift Right)
void divide_by_10(NumberListNode* val) {
    NumberNode *pprev, *prev, *now, *next;
    now = val->value->dot;
    prev = val->value->dot->prev;
    // [How]: 소수점(DOT) 노드를 한 칸 왼쪽(prev)으로 이동시키는 방식으로 구현
    if (prev == val->value->head) {
        NumberNode* temp = makeNumberNode();
        NumberList_push_front(val->value, 0, temp); // 자리가 없으면 0 추가
    }
    pprev = val->value->dot->prev->prev;
    next = val->value->dot->next;
    
    // 포인터 재연결 로직 (Doubly Linked List Swap)
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

// [실행 흐름: Step 5] 유틸리티: 10 곱하기 (Shift Left)
void product_by_10(NumberListNode* val) {
    NumberNode *prev, *now, *next, *nnext;
    now = val->value->dot;
    prev = val->value->dot->prev;
    // [How]: 소수점(DOT) 노드를 한 칸 오른쪽(next)으로 이동
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

// [실행 흐름: Step 5] 유틸리티: 2로 나누기
// [Detail]: 이진 탐색 시 Mid 값 계산에 사용됨. $X / 2 = X \times 0.5$로 구현.
NumberListNode* divide_by_2(NumberListNode* val) {
    NumberListNode* ret = makeNumberListNode();
    NumberNode* initializer = makeNumberNode();
    NumberList_push_back(ret->value, 5, initializer);
    initializer = makeNumberNode();
    NumberList_push_back(ret->value, DOT, initializer);
    ret->value->dot = initializer;
    // 현재 ret = 0.5 (소수점 뒤에 5가 있고 앞에 0이 생략된 상태)

    divide_by_10(val);       // [How]: $Val \times 0.1$
    ret = multiply(ret, val); // [How]: $(Val \times 0.1) \times 5 = Val \times 0.5 = Val / 2$
    return ret;
}

// [실행 흐름: Step 5] 불필요한 0 제거 (Trim Zeros)
void delete_zero(NumberListNode* val) {
    // 1. 앞쪽 0 제거 (예: 003.5 -> 3.5)
    while (val->value->head->number == 0 &&
           val->value->head != val->value->dot->prev) {
        NumberNode* temp = val->value->head;
        val->value->head = val->value->head->next;
        val->value->head->prev = NULL;
        if (temp == val->value->dot->prev) { // 소수점 바로 앞 0은 남김 (0.5 유지)
            free(temp);
            break;
        }
        free(temp);
    }

    // 2. 뒤쪽 0 제거 (예: 3.500 -> 3.5)
    while (val->value->tail->number == 0 &&
           val->value->tail != val->value->dot) {
        NumberNode* temp = val->value->tail;
        val->value->tail = val->value->tail->prev;
        val->value->tail->next = NULL;
        if (temp == val->value->dot) { // 소수점 자체를 지우진 않도록 주의
            val->value->dot = NULL;
            free(temp);
            break;
        }
        free(temp);
    }
}

// 디버깅용 함수들
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
