/**
 * [1. Top-down 추상화]
 * 이 헤더 파일은 '무한 소수 계산기'의 뼈대가 되는 자료구조와 함수의 인터페이스를 정의합니다.
 * 전체 설계는 다음 단계로 구성됩니다.
 * * [Step 1: Constants & Macros]: 연산자 및 특수 기호를 정수형 데이터와 구분하기 위한 고유 식별값 정의.
 * * [Step 2: Core Data Structures]: 무한 자릿수를 표현하기 위한 이중 연결 리스트(Node)와 이를 관리하는 컨테이너(List) 설계.
 * * [Step 3: Algorithm Support Structures]: 수식 파싱(Shunting-yard)과 연산 순서 제어를 위한 스택(Stack) 및 큐(Queue) 정의.
 * * [Step 4: Interface Definitions]: 메모리 관리, 자료구조 조작, 사칙연산 로직을 수행하는 함수들의 프로토타입 선언.
 */

#ifndef INFINITE_NUMBER_NODE_H
#define INFINITE_NUMBER_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// [실행 흐름: Step 1] 상수 및 매크로 정의
// [Why]: char 타입의 연산자를 int형 연결 리스트에 저장할 때, 숫자(0~9)와 충돌하지 않도록 고유한 값을 부여해야 함.
// [How]: 일반적인 숫자 범위에서 절대 나올 수 없는 큰 양수나 매우 작은 음수를 할당.
#define ERROR_SIZE 100000
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define OPEN_BRACKET 1000000
#define DOT -1000000000 // [Detail]: 소수점(.)은 자릿수 계산의 기준점이 되므로, 음수 중 가장 작은 값으로 설정하여 식별 용이성 확보.

// [실행 흐름: Step 2] 핵심 자료구조 정의 - 숫자 노드
// [Why]: 정해진 크기(int, double)를 넘어서는 무한한 자릿수를 저장하기 위해, 각 자릿수를 개별 노드로 분리해야 함.
typedef struct __NumberNode {
    int number;                 // [Detail]: 해당 자릿수의 실제 값 (0~9) 또는 특수 기호(DOT 등) 저장.
    struct __NumberNode* next;  // [How]: 소수점 아래(낮은 자릿수) 방향으로 연결 $N_{i} \rightarrow N_{i-1}$.
    struct __NumberNode* prev;  // [How]: 정수부(높은 자릿수) 방향으로 연결 $N_{i} \leftarrow N_{i+1}$. [Detail]: 올림수(Carry) 처리 시 역방향 탐색 필수.
} NumberNode;

// [실행 흐름: Step 2] 핵심 자료구조 정의 - 숫자 리스트 관리자
// [Why]: 낱개의 노드들만으로는 숫자의 시작과 끝, 소수점 위치를 O(1)에 파악하기 어려움. 메타 데이터를 관리할 구조체 필요.
typedef struct __NumberList {
    struct __NumberNode* head; // [Detail]: 숫자의 가장 높은 자릿수(MSB). 출력 및 크기 비교의 시작점.
    struct __NumberNode* tail; // [Detail]: 숫자의 가장 낮은 자릿수(LSB). 덧셈/뺄셈 연산의 시작점.
    struct __NumberNode* dot;  // [Why]: 정수부와 소수부를 가르는 기준점. [Detail]: 자릿수 맞춤(Padding) 연산 시 기준 포인터로 활용.
    int sig;                   // [Detail]: 부호 (1: 양수, -1: 음수). [How]: 곱셈/나눗셈 결과 부호는 $sig1 \times sig2$로 계산.
    int op;                    // [Detail]: 이 리스트가 숫자가 아닌 연산자(ADD, MUL 등)를 담고 있을 경우 해당 매크로 값 저장.
} NumberList;

// [실행 흐름: Step 3] 알고리즘 지원 구조 - 리스트 래퍼(Wrapper)
// [Why]: 스택이나 큐에 'NumberList' 자체를 넣으려 할 때, 순서를 관리할 추가적인 포인터(prev, next)가 필요함.
// [Detail]: 기존 __NumberList 구조체를 수정하지 않고, 이를 감싸는 노드를 만들어 스택/큐의 요소로 활용 (어댑터 패턴 유사).
typedef struct __NumberListNode {
    NumberList* value;             // [Detail]: 실제 데이터(무한 소수)를 가리키는 포인터.
    struct __NumberListNode* prev; // [Detail]: 스택/큐 내의 이전 요소 연결.
    struct __NumberListNode* next; // [Detail]: 스택/큐 내의 다음 요소 연결.
} NumberListNode;

// [실행 흐름: Step 3] 알고리즘 지원 구조 - 큐 (Queue)
// [Why]: 후위 표기식(Postfix Notation)으로 변환된 수식을 순서대로 저장하고 실행하기 위함.
typedef struct __calQueue {
    NumberListNode* qHead; // [Detail]: 큐의 Front (Dequeue 위치). 실행할 연산/숫자를 꺼내는 지점.
    NumberListNode* qTail; // [Detail]: 큐의 Rear (Enqueue 위치). 변환된 토큰을 넣는 지점.
} queue;

// [실행 흐름: Step 3] 알고리즘 지원 구조 - 스택 (Stack)
// [Why]: 중위 표기식을 후위 표기식으로 변환할 때 연산자 우선순위를 처리하거나, 후위 표기식을 계산할 때 피연산자를 임시 저장.
typedef struct __calStack {
    NumberListNode* sTop; // [Detail]: 스택의 Top (Push/Pop 위치). LIFO 구조.
} stack;

// [실행 흐름: Step 4] 인터페이스 정의 - 리스트 조작
void NumberList_push_back(NumberList* nl, int val, NumberNode* now);  // [Detail]: 리스트 뒤(Tail)에 노드 추가 (일반적인 숫자 생성)
void NumberList_push_front(NumberList* nl, int val, NumberNode* now); // [Detail]: 리스트 앞(Head)에 노드 추가 (Carry 발생 시 사용)
NumberListNode* makeNumberListNode();                                 // [Detail]: 래퍼 노드 메모리 할당 및 초기화
NumberListNode* copyNumberListNode(NumberListNode* val);              // [Why]: 원본 데이터 보존을 위한 깊은 복사(Deep Copy)
NumberList* makeNumberList();                                         // [Detail]: 리스트 관리자 구조체 생성
NumberNode* makeNumberNode();                                         // [Detail]: 숫자 노드 생성

// [실행 흐름: Step 4] 인터페이스 정의 - 자료구조(Stack/Queue) 조작
void queue_push(queue* Q, NumberListNode* nowNode);
NumberListNode* queue_pop(queue* Q);
void stack_push(stack* S, NumberListNode* now);
NumberListNode* stack_pop(stack* S);
void stack_push_op(stack* S, queue* Q, int opInput); // [Detail]: 연산자 우선순위 비교 로직을 포함한 스택 푸시

// [실행 흐름: Step 4] 인터페이스 정의 - 메모리 관리
void NumberList_Clear(NumberList* nl);          // [Why]: 사용이 끝난 무한 소수(리스트 전체) 메모리 해제
void NumberListNode_Clear(NumberListNode* nln); // [Detail]: 래퍼 노드와 내부 리스트까지 모두 해제
void mallocAssert(void* p);                     // [Why]: 메모리 할당 실패(NULL) 시 프로그램 비정상 종료 방지 및 에러 메시지 출력

// [실행 흐름: Step 4] 인터페이스 정의 - 유틸리티 및 초기화
void print_NumberListNode(NumberListNode* nln); // [Detail]: 디버깅 및 결과 출력을 위한 순회 함수
void initializeQueue(queue* q);
void initializeStack(stack* s);
void initializeNumberNode(NumberNode* node);
void initializeNumberList(NumberList* list);
void initializeNumberListNode(NumberListNode* node);

// [실행 흐름: Step 4] 인터페이스 정의 - 산술 연산 보조 함수
int compareAbsoluteValue(NumberListNode* val1, NumberListNode* val2); // [Why]: 뺄셈/나눗셈 시 절대값 크기 비교 필요
NumberListNode* divide_by_2(NumberListNode* val);                     // [Detail]: 나눗셈 이진 탐색(Binary Search)에 사용
void divide_by_10(NumberListNode* val);                               // [Detail]: 자릿수 이동 (Shift Right)
void product_by_10(NumberListNode* val);                              // [Detail]: 자릿수 이동 (Shift Left)
void NumberList_push_dot(NumberList* nl);                             // [Detail]: 소수점 노드 명시적 추가
void delete_zero(NumberListNode* val);                                // [Why]: 연산 후 발생하는 불필요한 앞뒤의 0 제거 (003.50 -> 3.5)

// [실행 흐름: Step 4] 인터페이스 정의 - 핵심 사칙연산
NumberListNode* add(NumberListNode* val1, NumberListNode* val2);      // [Detail]: 덧셈 (O(N))
NumberListNode* subtract(NumberListNode* val1, NumberListNode* val2); // [Detail]: 뺄셈 (O(N))
NumberListNode* multiply(NumberListNode* val1, NumberListNode* val2); // [Detail]: 곱셈 (덧셈의 반복, O(N^2) 유사)
NumberListNode* divide(NumberListNode* val1, NumberListNode* val2);   // [Detail]: 나눗셈 (이진 탐색 + 곱셈/비교 반복)

// for debug
void print_stack_top(stack* s);
void print_queue(queue* q);

#endif
