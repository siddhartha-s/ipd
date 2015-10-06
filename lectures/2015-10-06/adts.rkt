;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname adts-outline) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t quasiquote repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp")) #f)))

;; ADTs: abstract data types
;;  - defines a type in terms of information (not data!) and abstract
;;    operations

; A SetOfNumbers represents a set like you’d see in math class.
;
; Examples:
;  - empty: ∅
;  - singleton: {5}
;  - larger sets: {1, 3, 5, 7}
;
; It has these operations (and values):
;
; son-empty: SetOfNumbers
; The empty set ∅.
;
; son-singleton: Number -> SetOfNumbers
; A singleton set {n}.
;
; son-member?: Number SetOfNumbers -> Boolean
; Determines whether a number is an element of a set.
;
;   Examples:
;    - (son-member? 5 ⌈{4, 5, 6}⌉) => #true
;    - (son-member? 9 ⌈{4, 5, 6}⌉) => #false

#;
(check-expect (son-member? 5 (son-union (son-singleton 5)
                                        (son-singleton 6)))
              #true)

#;
(define (son-member? a-number a-son)
  (son-subset? (son-singleton a-number) a-son))

; son-union: SetOfNumbers SetOfNumbers -> SetOfNumbers
; Returns the union of two sets, A ∪ B.
;
;   Examples:
;     - ∅ ∪ {2, 3} = {2, 3}
;     - (son-union {1, 2} {2, 4}) => {1, 2, 4}
;
; son-intersect: SetOfNumbers SetOfNumbers -> SetOfNumbers
; Returns the intersection of two sets, A ∩ B.
;
;   Examples:
;     - ∅ ∩ {2, 3} = ∅
;     - (son-intersect {1, 2} {2, 4}) => {2}
;
; son-difference: SetOfNumbers SetOfNumbers -> SetOfNumbers
;
; son-count: SetOfNumbers -> Number
; Returns the cardinality of a set, |A|.
;
; son-subset?: SetOfNumbers SetOfNumbers -> Boolean
; Determines whether the first set is a subset of the second.
;

; A ListSetOfNumbers (implements SetOfNumbers) is one of:
; -- '()
; -- (cons Number ListSetOfNumbers)
; where all elements are unique   [representation invariant]
;
; Interpretation:
; -- ⟦'()⟧ = ∅
; -- ⟦(cons n lson)⟧ = n ∪ ⟦lson⟧    when n ∉ ⟦lson⟧
;[-- ⟦anything else⟧ is invalid! (don't write this part)]

; [Counter-example: '(1 2 4 4) is not a ListSetOfNumbers.]

; [How we use a representation invariant:
;   - If every ListSetOfNumbers we create follows the invariant,
;   - and if every function that takes ListSetOfNumbers(es) preserves
;     the invariant,
;   - then every function that takes ListSetOfNumbers can rely on the
;     invariant.]

; Multiple (data) representations of the same (information) set:
;
;   ⟦(list 3 9 -2)⟧ = {-2, 3, 9} = ⟦(list -2 9 3)⟧

; son-empty: SetOfNumbers
; The empty set ∅.
(define son-empty '())

; lson-singleton: Number -> SetOfNumbers
; A singleton set {n}.
;
; Examples:
;  - (lson-singleton 4) => {4}
;  - (lson-singleton 8) => {8}
;
; Strategy: domain knowledge (ListSetOfNumbers representation)
(define (lson-singleton n)
  (cons n '()))

(check-expect (lson-singleton 4) '(4))
(check-expect (lson-singleton 8) '(8))

; lson-member?: Number SetOfNumbers -> Boolean
; Determines whether a number is an element of a set.
;
;   Examples:
;    - (lson-member? 5 ⌈{4, 5, 6}⌉) => #true
;    - (lson-member? 9 ⌈{4, 5, 6}⌉) => #false
;
; Strategy: Structural decomposition
(define (lson-member? n lson)
  (cond
    [(empty? lson) #false]
    [(cons? lson)
     (or (= n (first lson))
         (lson-member? n (rest lson)))]))

(check-expect (lson-member? 5 '(4 5 6)) #true)
(check-expect (lson-member? 4 '(4 5 6)) #false)

; lson-union: ListSetOfNumbers ListSetOfNumbers -> ListSetOfNumbers
; Returns the union of two sets, A ∪ B.
;
;   Examples:
;     - ∅ ∪ {2, 3} = {2, 3}
;     - (lson-union {1, 2} {2, 4}) => {1, 2, 4}
;
; Strategy: structural decomposition (lson1)
(define (lson-union lson1 lson2)
  (cond
    [(empty? lson1) lson2]
    [(cons? lson1)
     (insert-if-not-member
       (first lson1)
       (lson-union (rest lson1) lson2))]))

;(check-expect (lson-union '(2 3)) '(2 3))
;(check-expect (lson-union '(1 2) '(4 2)) '(1 2 4))

; Number ListSetOfNumbers -> ListSetOfNumbers
; Inserts an element to a set if it's not already a member.
;
; Strategy: domain knowledge (ListSetOfNumber repr.)
(define (insert-if-not-member n lson)
  (if (lson-member? n lson)
    lson
    (cons n lson)))

; lson-count: ListSetOfNumbers -> Number
; Returns the cardinality of a set, |A|.
;
; Examples:
;  - (lson-count ∅) => 0
;  - (lson-count {2, 3, 4, 5}) => 4
;
; Strategy: function composition
(define (lson-count lson)
  (length lson))

(check-expect (lson-count '()) 0)
(check-expect (lson-count '(2 3 4 5)) 4)

(check-expect (lson-count (lson-union '(1 2) '(2 4)))
              3)

;; A StackOfNumbers represents a LIFO stack of numbers.
;;
;; We'll write a StackOfNumbers in a “ket” with the top of the stack
;; on the right:
;; 
;;  - the empty stack: |>
;;  - a stack with one element: |5>
;;  - a stack with 5 on top and 3 on the bottom: |3 4 5>
;
; Operations:
;
; stack-empty: StackOfNumbers
; The empty stack |>
;
; stack-empty?: StackOfNumbers -> Boolean
; Is the given stack empty?
;
; stack-push: Number StackOfNumbers -> StackOfNumbers
; Pushes a number onto the stack.
;
;   Examples:
;    - (stack-push 4 |>) => |4>
;    - (stack-push 4 |3 7 9>) => |3 7 9 4>
; 
; stack-top: StackOfNumbers -> Number
; Returns the number on top of the stack, or error if empty.
;
;   Examples:
;    - (stack-top |3 7 9 4>) => 4
;    - (stack-top |3>) => 3
;    - (stack-top |>) ERROR
; 
; stack-pop: StackOfNumbers -> StackOfNumbers
; Returns remaining stack after popping the top, or error if empty.
;
;   Examples:
;    - (stack-pop |3 7 9 4>) => |3 7 9>
;    - (stack-pop |3>) => |>
;    - (stack-pop |>) ERROR

; A ListStackOfNumbers is one of:
; -- '()
; -- (cons Number ListStackOfNumbers)
;
; Not so good interpretation:
;  - ⟦'()⟧ = |>
;  - ⟦(cons n lson)⟧ = |n m1 m2 ... mk>
;      where ⟦lson⟧ = |m1 m2 ... mk>
;
; Good interpretation:
;  - ⟦'()⟧ = |>
;  - ⟦(cons n lson)⟧ = |m1 m2 ... mk n>
;      where ⟦lson⟧ = |m1 m2 ... mk>

; Implementations using the good interpretation:
(define (stack-push n stk)
  (cons n stk))

(define (stack-pop stk)
  (rest stk))

(define (stack-top stk)
  (first stk))


; A QoN represents a FIFO queue of numbers
;
; We will write a QoN (in the world of information) between right-pointing guillemets:
;  - »» is the empty queue
;  - »3» is a queue with one element in it
;  - »5 4 3» is a queue with 5 added first and 3 added most recently
;
; Operations:
;   qon-empty: QoN
;   qon-empty?: QoN -> Boolean
;   qon-enqueue: Number QoN -> QoN
;   qon-front: QoN -> Number
;   qon-dequeue: QoN -> QoN

; A LQoN-A (implements QoN) is one of:
; -- '()
; -- (cons Number LQoN)
;
; Interpretation:
; -- ⟦'()⟧ = »»
; -- ⟦(cons n lqon)⟧ = »m1 m2 ... mk n»
;      where ⟦lqon⟧ = »m1 m2 ... mk»

; A LQoN-B (implements QoN) is one of:
; -- '()
; -- (cons Number LQoN)
;
; Interpretation:
; -- ⟦'()⟧ = »»
; -- ⟦(cons n lqon)⟧ = »n m1 m2 ... mk»
;      where ⟦lqon⟧ = »m1 m2 ... mk»

; (enqueue n lqon)
;  - A: (snoc lqon n)           — O(N) where N is (length lqon)
;  - B: (cons n lqon)           — O(1)

; (dequeue lqon)
;  - A: (rest lqon)             — O(1)
;  - B: (all-but-last lqon)     — O(N) where N is (length lqon)

(define (snoc lst1 n)
  (cond
    [(empty? lst1) (list n)]
    [(cons? lst1) (cons (first lst1)
                        (snoc (rest lst1) n))]))

(define (all-but-last lst)
  (cond
    [(empty? (rest lst)) '()]
    [(cons? (rest lst)) (cons (first lst)
                              (all-but-last (rest lst)))]))


