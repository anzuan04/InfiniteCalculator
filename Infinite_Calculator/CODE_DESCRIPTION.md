# CODE_DESCRIPTION

The summarized description about InfiniteNumberNode.c

---

## Project Structure

| 파일명 | 역할 | 상세 설명 |
| :--- | :--- | :--- |
| **`infiniteNumberNode.h`** | 구조체(`struct`), 상수(`define`), 함수 프로토타입 정의 |
| **`infiniteNumberNode.c`** | 메모리 관리, 자료구조 조작, 사칙연산(`+`, `-`, `*`, `/`) 알고리즘 구현 |
| **`main.c`** |  사용자 입력 파싱, 중위→후위 표기식 변환, 연산 스케줄링 및 결과 출력 |

---

## System Architecture

프로그램은 **입력(Input) → 파싱(Parsing) → 연산(Calculation) → 출력(Output)**의 파이프라인으로 동작합니다.

```text
[User Input Stream]   (e.g., "12.5 + 3")
       │
       ▼
[1. Parsing Engine] (in main.c)
       │ • 공백 제거 및 유효성 검사 (Leading Zero 등)
       │ • Shunting-yard 알고리즘: 중위 표기법 -> 후위 표기법 변환
       │ • Stack(연산자)과 Queue(피연산자) 활용
       ▼
[2. Execution Queue] (Postfix)
       │ (State: ["12.5", "3", "+"])
       ▼
[3. Calculation Core] (in infiniteNumberNode.c)
       │ • Queue에서 토큰 추출
       │ • "피연산자" -> Stack Push
       │ • "연산자" -> Stack Pop 2회 -> 연산 수행(Add/Mul...) -> Result Push
       ▼
[Output Result]

---

## InfiniteNumberNode.c

### DataStructure

(Prev/MSB) <------------------------------------> (Next/LSB)

    [Head]                                            [Tail]
      │                                                 │
      ▼                                                 ▼
    ┌───┐    ┌───┐    ┌───────┐    ┌───┐    ┌───┐
    │ 3 │◀──▶│ 5 │◀──▶│ DOT(.)│◀──▶│ 1 │◀──▶│ 2 │
    └───┘    └───┘    └───────┘    └───┘    └───┘
                          ▲
                          │
                 [Dot Pointer] (기준점)

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

    2. **Addition Process**

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

        2. *move fc1 and fc2 to next then repeat a.*

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
        
        3. *again a.~b.*

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

        4. *process the carry in ret*

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
        
        5. *add DOT to ret*

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

        2. *move ic1 and ic2 to prev then repeat a.*

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

    3. **Return ret**

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

        > ex : 39.12 - 38.995

    2. **Subtraction Process**

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

        2. *move fc1 and fc2 to next then repeat a.*

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
        
        3. *again a.~b.*

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
                | . | 1 | 2 | 5 |           **NLN ret**
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
                | 0 | . | 1 | 2 | 5 |       **NLN ret**
                +---+---+---+---+---+

                int borrow = 0
            
            > int borrow = !((10 + ic1 - ic2 - borrow) % 10)

        2. *move ic1 and ic2 to prev then repeat a.*

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
                | 0 | 0 | . | 1 | 2 | 5 |   **NLN ret**
                +---+---+---+---+---+---+

                int borrow = 0
        
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
                | 0 | . | 1 | 2 | 5 |       **NLN ret**
                +---+---+---+---+---+

    3. **Return ret**

            +---+---+---+---+---+
            | 0 | . | 1 | 2 | 5 |       **NLN ret**
            +---+---+---+---+---+

* **NumberListNode\* multiply(NumberListNode\* val1, NumberListNode\* val2);**

    1. **Definition To Setup**
            
            +---+---+---+---+---+
            | 3 | 5 | . | 1 | 2 |       **NLN val1**
            +---+---+---+---+---+

            +---+---+---+---+---+---+
            | 3 | 7 | . | 9 | 9 | 5 |   **NLN val2**
            +---+---+---+---+---+---+

            +------+---+---+---+
            | val1 |   |   |   |        **NLN sumOverDot[4]**
            +------+---+---+---+

            +------+---+---+---+
            | val1 |   |   |   |        ** NLN sumUnderDot[4]**
            +------+---+---+---+

            +---+---+
            | 0 | . |                   **NLN ret**
            +---+---+

            ret->value->sig = val1->value->sig * val2->value->sig;

        > ex : 35.12 * 37.995

    2. **Multiplication Process**

    * ***int Multiplication process***

        1. *make list for transforming mul to add*

                +------+--------+--------+--------+
                | val1 | 2*val1 | 4*val1 | 8*val1 |   **NLN sumOverDot[4]**
                +------+--------+--------+--------+
      
        2. *add sumOverDot to ret repeatedly until ret equals the same value as the product*

                     now                     **NN pointer**
                      |
                      v
                +---+---+---+---+---+---+
                | 3 | 7 | . | 9 | 9 | 5 |    **NLN val2**
                +---+---+---+---+---+---+
  
                +---+---+   +------+   +--------+   +--------+
                | 0 | . | + | val1 | + | 2*val1 | + | 4*val1 |   **NLN ret**
                +---+---+   +------+   +--------+   +--------+

                                     |
                                     v

                                 +--------+
                                 | 7*val1 |  **NLN ret**
                                 +--------+

            > 7\*val1 = val1 + 2\*val1 + 4\*val1

        3. *multiply sumOverDot by 10*

                +---------+---------+---------+---------+
                | 10*val1 | 20*val1 | 40*val1 | 80*val1 |   **NLN sumOverDot[4]**
                +---------+---------+---------+---------+
        
        4. *move now to prev and repeat b.*

                 now                       **NN pointer**
                  |
                  v
                +---+---+---+---+---+---+
                | 3 | 7 | . | 9 | 9 | 5 |  **NLN val2**
                +---+---+---+---+---+---+
  
                +--------+   +---------+   +---------+
                | 7*val1 | + | 10*val1 | + | 20*val1 |   **NLN ret**
                +--------+   +---------+   +---------+

                                 |
                                 v
                            +---------+
                            | 37*val1 |    **NLN ret**
                            +---------+

            > 3\*val1 = val1 + 2\*val1
    
    * ***float Multiplication process***

        1. *make list for transforming mul to add*

                +----------+----------+----------+----------+
                | 0.1*val1 | 0.2*val1 | 0.4*val1 | 0.8*val1 |   **NLN sumUnderDot[4]**
                +----------+----------+----------+----------+
      
        2. *add sumUnderDot to ret repeatedly until ret equals the same value as the product*

                             now             **NN pointer**
                              |
                              v
                +---+---+---+---+---+---+
                | 3 | 7 | . | 9 | 9 | 5 |    **NLN val2**
                +---+---+---+---+---+---+
  
                +---------+   +----------+   +----------+
                | 37*val1 | + | 0.1*val1 | + | 0.8*val1 |  **NLN ret**
                +---------+   +----------+   +----------+

                                     |
                                     v

                               +-----------+
                               | 37.9*val1 |  **NLN ret**
                               +-----------+

            > 9\*val1 = val1 + 8\*val1

        3. *divide by 10 to sumUnderDot*

                +-----------+-----------+-----------+-----------+
                | 0.01*val1 | 0.02*val1 | 0.04*val1 | 0.08*val1 |   **NLN sumUnderDot[4]**
                +-----------+-----------+-----------+-----------+
        
        4. *move now to prev and repeat b.*

                                 now            **NN pointer**
                                  |
                                  v
                +---+---+---+---+---+---+
                | 3 | 7 | . | 9 | 9 | 5 |       **NLN val2**
                +---+---+---+---+---+---+
  
                +-----------+   +-----------+   +-----------+
                | 37.9*val1 | + | 0.01*val1 | + | 0.08*val1 |  **NLN ret**
                +-----------+   +-----------+   +-----------+

                                       |
                                       v
  
                                +------------+
                                | 37.99*val1 |  **NLN ret**
                                +------------+

            > 9\*val1 = val1 + 8\*val1

        5. *repeat b.~d.*

                                     now         **NN pointer**
                                      |
                                      v
                +---+---+---+---+---+---+
                | 3 | 7 | . | 9 | 9 | 5 |        **NLN val2**
                +---+---+---+---+---+---+
  
                +------------+   +------------+   +------------+
                | 37.99*val1 | + | 0.001*val1 | + | 0.004*val1 |  **NLN ret**
                +------------+   +------------+   +------------+

                                       |
                                       v
  
                                +-------------+
                                | 37.995*val1 |  **NLN ret**
                                +-------------+

            > 5\*val1 = val1 + 4\*val1
    
    3. **Return ret**

            +-------------+
            | 37.995*val1 |  **NLN ret**
            +-------------+

* **NumberListNode\* divide(NumberListNode\* val1, NumberListNode\* val2);**

    1. **Definition To Setup**
                
                 left       **NLN left**
                  |
                  v
            +---+---+---+
            | 9 | 0 | . |   **NLN val1**
            +---+---+---+

            +---+---+---+
            | 1 | 5 | . |   **NLN val2**
            +---+---+---+

                right       **NLN right**
                  |
                  v
            +---+---+---+
            | 0 | . | 1 | ^ 200  **NLN smallVal**
            +---+---+---+

            ret->value->sig = val1->value->sig * val2->value->sig;

            int cnt = 0

        > ex : 9 / 1.5

        > multiply 9 and 1.5 by 10 in order to remove the decimal part

    2. **Divide And Conquer**

        * ***divide process***

                     left       **NLN left**
                      |
                      v
                +---+---+---+
                | 9 | 0 | . |   **NLN val1**
                +---+---+---+

                      +

                    right       **NLN right**
                      |
                      v
                +---+---+---+
                | 0 | . | 1 | ^ 200  **NLN smallVal**
                +---+---+---+

                      =

                   +-----+
                   | ret | / 2  **NLN mid**
                   +-----+

            > mid = (left+right)/2

            > 0.1^200…mid…90

        * ***Conquer process***

                +---------------------------------------------------+
                | compareAbsoluteValue(multiply(M, VAL2), val1) > 0 |
                +---------------------------------------------------+
                                          |
                               +----------+----------+
                              Yes                   No
                               |                     |
                        +-------------+       +------------+
                        | right = mid |       | left = mid |
                        +-------------+       +------------+

                int cnt++

            > if mid\*15 is greater than 90, right is mid. so next mid is smaller than now mid

            > else, left is mid. so next mid is greater than now mid

    3. **Repeat ii. until (cnt >= 10000 or left+smallVal >= right)**

            +---------------------+
            | 6.44342576…*0.1^200 |  **NLN right**
            +---------------------+
    
    4. **Return right**

            +---------------------+
            | 6.44342576…*0.1^200 |  **NLN right**
            +---------------------+

---

### DataStructure Method

#### NumberList

* **void NumberList_push_back(NumberList\* nl, int val, NumberNode\* now);**

    1. **Store val To now**

            +---+
            | 8 |   **NN head**
            +---+
              
              ^     **NN prev**
              |
              v     **NN next**
            
            +---+
            | 7 |   **NN tail**
            +---+

            +---+
            | 9 |   **NN now**
            +---+
        
        > ex : push back 9 to 87

        > if NumberList is NULL, head and tail is now
              
    2. **Set prev And next**

            +---+
            | 8 |   **NN head**
            +---+
              
              ^     **NN prev**
              |
              v     **NN next**
            
            +---+
            | 7 |   **NN tail**
            +---+

              ^     **NN prev**
              |
              v     **NN next**

            +---+
            | 9 |   **NN now**
            +---+
    
    3. **Set tail**
        
            +---+
            | 8 |   **NN head**
            +---+
              
              ^     **NN prev**
              |
              v     **NN next**
            
            +---+
            | 7 |
            +---+

              ^     **NN prev**
              |
              v     **NN next**

            +---+
            | 9 |   **NN tail**
            +---+
    
* **void NumberList_push_front(NumberList\* nl, int val, NumberNode\* now);**

    1. **Store val To now**

            +---+
            | 9 |   **NN now**
            +---+

            +---+
            | 8 |   **NN head**
            +---+
              
              ^     **NN prev**
              |
              v     **NN next**
            
            +---+
            | 7 |   **NN tail**
            +---+
        
        > ex : push front 9 to 87

        > if NumberList is NULL, head and tail is now        
              
    2. **Set prev And next**

            +---+
            | 9 |   **NN now**
            +---+

              ^     **NN prev**
              |
              v     **NN next**

            +---+
            | 8 |   **NN head**
            +---+
              
              ^     **NN prev**
              |
              v     **NN next**
            
            +---+
            | 7 |   **NN tail**
            +---+
    
    3. **Set head**
        
            +---+
            | 9 |   **NN head**
            +---+

              ^     **NN prev**
              |
              v     **NN next**

            +---+
            | 8 |
            +---+
              
              ^     **NN prev**
              |
              v     **NN next**
            
            +---+
            | 7 |   **NN tail**
            +---+

#### Queue

* **void queue_push(queue\* Q, NumberListNode\* nowNode);**

    1. **Set prev And next Between tail and nowNode**

            +-----+
            | 829 |  **NLN head**
            +-----+
              
               ^     **NLN prev**
               |
               v     **NLN next**
            
            +-----+
            | 795 |  **NLN tail**
            +-----+

               ^     **NLN prev**
               |
               v     **NLN next**

             +---+
             | + |   **NLN nowNode**
             +---+

        > ex : push + to queue

        > if queue is NULL, qhead and qtail is nowNode  
    
    2. **Set tail**
        
            +-----+
            | 829 |  **NLN head**
            +-----+
              
               ^     **NLN prev**
               |
               v     **NLN next**
            
            +-----+
            | 795 |  
            +-----+

               ^     **NLN prev**
               |
               v     **NLN next**

             +---+
             | + |   **NLN tail**
             +---+

* **NumberListNode\* queue_pop(queue\* Q);**

    1. **Set ret To Point head**

            +-----+
            | 829 |  **NLN head** <- NLN* ret
            +-----+
              
               ^     **NLN prev**
               |
               v     **NLN next**
            
            +-----+
            | 795 |  
            +-----+

               ^     **NLN prev**
               |
               v     **NLN next**

             +---+
             | + |   **NLN tail**
             +---+

    2. **Set head To next**

            +-----+
            | 829 |  <- NLN* ret
            +-----+
              
               ^     **NLN prev**
               |
               v     **NLN next**
            
            +-----+
            | 795 |  **NLN head**
            +-----+

               ^     **NLN prev**
               |
               v     **NLN next**

             +---+
             | + |   **NLN tail**
             +---+

    3. **Remove prev And next Between ret And head**

            +-----+
            | 829 |  <- NLN* ret
            +-----+
              
            +-----+
            | 795 |  **NLN head**
            +-----+

               ^     **NLN prev**
               |
               v     **NLN next**

             +---+
             | + |   **NLN tail**
             +---+
    
    4. **Return ret**

            +-----+
            | 829 |  <- NLN* ret
            +-----+

#### Stack

* **void stack_push(stack\* S, NumberListNode\* now);**

    1. **Set prev And next Between sTop and nowNode**
            
            +---+
            | + |   
            +---+
                           
              ^
              |
              v
            
            +---+
            | * |   **NLN sTop**
            +---+
              
              ^
              |
              v
            
            +---+
            | ( |   **NLN nowNode**
            +---+

    2. **Set sTop**
            
            +---+
            | + |   
            +---+
                           
              ^
              |
              v
            
            +---+
            | * |   
            +---+
              
              ^
              |
              v
            
            +---+
            | ( |   **NLN sTop**
            +---+

* **NumberListNode\* stack_pop(stack\* S);**

    1. **Set ret To Point sTop**
            
            +---+
            | + |   
            +---+
                           
              ^
              |
              v
            
            +---+
            | * |   
            +---+
              
              ^
              |
              v
            
            +---+
            | ( |   **NLN sTop** <- NLN* ret
            +---+

    2. **Set sTop to prev**
            
            +---+
            | + |   
            +---+
                           
              ^
              |
              v
            
            +---+
            | * |   **NLN sTop** 
            +---+
              
              ^
              |
              v
            
            +---+
            | ( |   <- NLN* ret
            +---+

    3. **Remove prev And next Between sTop and ret**
            
            +---+
            | + |   
            +---+
                           
              ^
              |
              v
            
            +---+
            | * |   **NLN sTop** 
            +---+
              
            +---+
            | ( |   <- NLN* ret
            +---+

    3. **Return ret**

            +---+
            | ( |   <- NLN* ret
            +---+
            
* **void stack_push_op(stack\* S, queue\* Q, int opInput);**

    1. **stack pop then push queue until sTop is open bracket or operator priority of opInput is greater than sTop**
            
            STACK                      QUEUE

            +---+                      +----+
            | + |                      | 12 |
            +---+                      +----+
              |                          |
            +---+                      +----+
            | * |   **NLN sTop**       | 22 |
            +---+                      +----+
                                         |
            +---+                      +----+
            | / |   **int opInput**    | 34 |
            +---+                      +----+
              
                            |                        
                            |
                            v

            STACK                      QUEUE

            +---+                      +----+
            | + |   **NLN sTop**       | 12 |
            +---+                      +----+
                                         |
            +---+                      +----+
            | / |   **int opInput**    | 22 |
            +---+                      +----+
                                         |
                                       +----+
                                       | 34 |
                                       +----+
                                         |
                                       +---+
                                       | * |
                                       +---+
                                       
        > ex : 12+22*34/22

    2. **stack push opInput**

            STACK      

            +---+                      
            | + |   **NLN sTop**       
            +---+
              |
            +---+                      
            | / |   **int opInput**    
            +---+  

---


    
