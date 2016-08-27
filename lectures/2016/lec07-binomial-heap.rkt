;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname lec07-binomial-heap) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;; ADT:
;;  empty-heap : binomial-heap
;;  is-empty? : binomial-heap -> boolean
;;  insert : binomial-heap number -> binomial-heap
;;  find-min : binomial-heap -> number
;;     pre: not is-empty?
;;  remove-min : binomial-heap -> binomial-heap
;;     pre: not is-empty?
;;  meld : binomial-heap binomial-heap -> binomial-heap

(check-expect (find-min (insert empty-heap 0)) 0)
(check-expect (find-min (insert (insert empty-heap 0) 1)) 0)
(check-expect (find-min (insert (insert empty-heap 1) 0)) 0)

(define (insert-nums nums)
  (cond
    [(empty? nums) empty-heap]
    [else (insert (insert-nums (rest nums)) (first nums))]))

(define (remove-mins h)
  (cond
    [(empty-heap? h) '()]
    [else (cons (find-min h)
                (remove-mins (remove-min h)))]))

(check-expect (remove-mins (insert-nums (list 0 0 0))) (list 0 0 0))

(check-expect (remove-mins (insert-nums (list 0 1 2))) (list 0 1 2))
(check-expect (remove-mins (insert-nums (list 1 0 2))) (list 0 1 2))
(check-expect (remove-mins (insert-nums (list 0 2 1))) (list 0 1 2))
(check-expect (remove-mins (insert-nums (list 2 0 1))) (list 0 1 2))
(check-expect (remove-mins (insert-nums (list 1 2 0))) (list 0 1 2))
(check-expect (remove-mins (insert-nums (list 2 1 0))) (list 0 1 2))

(check-expect (remove-mins (insert-nums (list 0 1 2 3))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 1 0 2 3))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 0 2 1 3))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 2 0 1 3))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 1 2 0 3))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 2 1 0 3))) (list 0 1 2 3))

(check-expect (remove-mins (insert-nums (list 0 1 3 2))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 1 0 3 2))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 0 2 3 1))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 2 0 3 1))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 1 2 3 0))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 2 1 3 0))) (list 0 1 2 3))

(check-expect (remove-mins (insert-nums (list 0 3 1 2))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 1 3 0 2))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 0 3 2 1))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 2 3 0 1))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 1 3 2 0))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 2 3 1 0))) (list 0 1 2 3))

(check-expect (remove-mins (insert-nums (list 3 0 1 2))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 3 1 0 2))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 3 0 2 1))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 3 2 0 1))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 3 1 2 0))) (list 0 1 2 3))
(check-expect (remove-mins (insert-nums (list 3 2 1 0))) (list 0 1 2 3))

(define (make-nums n)
  (cond
    [(zero? n) '()]
    [else (cons (- n 1) (make-nums (- n 1)))]))

(define lots-of-numbers (reverse (make-nums 10000)))
(check-expect (remove-mins (insert-nums lots-of-numbers)) lots-of-numbers)
(check-expect (remove-mins (insert-nums (reverse lots-of-numbers)))
              lots-of-numbers)

(check-expect (remove-mins (meld (insert-nums (list 0 1 2 3))
                                 (insert-nums (list 4 5 6 7))))
              (list 0 1 2 3 4 5 6 7))
(check-expect (remove-mins (meld (insert-nums (list 4 5 6 7))
                                 (insert-nums (list 0 1 2 3))))
              (list 0 1 2 3 4 5 6 7))

;; a binomial-tree is:
;;  (make-node number (listof binomial-tree))
;; invariant: 
;;  if 'children' is non-empty, then the first
;;  element of 'children' has the same shape & size as
;;  the tree you get by removing that first child
;;  and making a new tree from the rest.

(define-struct node (value rank children))

;; a binomial-heap is:
;;  (listof binomial-tree)

;; find-min : binomial-heap -> number or #f
(define (find-min b)
  (cond
    [(empty? b) #f]
    [else
     (pick-one (node-value (first b))
               (find-min (rest b)))]))

;; pick-one : number (number or false) -> number
(define (pick-one n1 n2/f)
  (cond
    [(number? n2/f) (min n1 n2/f)]
    [else n1]))

(define empty-heap '())
(define (empty-heap? b) (empty? b))

;; insert : binomial-heap number -> binomial-heap
(define (insert b n)
  (add #false (list (make-node n 0 '())) b))

(define (remove-min b)
  (local [(define small-root (find-min b))
          (define heap-without-small-root
            (remove-tree-with-root b small-root))
          (define node-with-small-root
            (find-tree-with-root b small-root))]
    (add #false
         heap-without-small-root
         (reverse (node-children node-with-small-root)))))

(define (remove-tree-with-root b v)
  (cond
    [(empty? b) (error 'ack)]
    [else
     (cond
       [(= (node-value (first b)) v)
        (rest b)]
       [else
        (cons (first b) (remove-tree-with-root (rest b) v))])]))

(define (find-tree-with-root b v)
  (cond
    [(empty? b) (error 'ack)]
    [else
     (cond
       [(= (node-value (first b)) v)
        (first b)]
       [else
        (find-tree-with-root (rest b) v)])]))

(define (meld b1 b2)
  (add #f b1 b2))

;; add : (or #f binomial-tree) binomial-heap binomial-heap -> binomial-heap
(define (add c-in b1 b2)
  (local [(define carry-rank (tree-rank c-in))
          (define b1-rank (heap-rank b1))
          (define b2-rank (heap-rank b2))]
    (cond
      [(boolean? carry-rank)
       (cond
         [(boolean? b1-rank) b2]
         [(boolean? b2-rank) b1]
         [(= b1-rank b2-rank)
          (add (join (first b1) (first b2)) (rest b1) (rest b2))]
         [(< b1-rank b2-rank)
          (cons (first b1) (add #f (rest b1) b2))]
         [(> b1-rank b2-rank)
          (cons (first b2) (add #f b1 (rest b2)))])]
      [else
       (cond
         [(and (boolean? b1-rank) (boolean? b2-rank))
          (list c-in)]
         [(boolean? b2-rank)
          (cond
            [(< carry-rank b1-rank)
             (cons c-in (add #f b1 b2))]
            [(= carry-rank b1-rank)
             (add (join c-in (first b1)) (rest b1) b2)])]
         [(boolean? b1-rank)
          (cond
            [(< carry-rank b2-rank)
             (cons c-in (add #f b1 b2))]
            [(= carry-rank b2-rank)
             (add (join c-in (first b2)) b1 (rest b2))])]
         [else
          (cond
            [(and (< carry-rank b1-rank)
                  (< carry-rank b2-rank))
             (cons c-in (add #f b1 b2))]
            [(and (< carry-rank b1-rank)
                  (= carry-rank b2-rank))
             (add (join c-in (first b2))
                  b1
                  (rest b2))]
            [(and (< carry-rank b2-rank)
                  (= carry-rank b1-rank))
             (add (join c-in (first b1))
                  (rest b1)
                  b2)]
            [(and (= carry-rank b1-rank)
                  (= carry-rank b2-rank))
             (cons c-in
                   (add (join (first b1) (first b2))
                        (rest b1)
                        (rest b2)))])])])))

;; join : binomial-tree binomial-tree -> binomial-tree
;; ASSERTION (= (node-rank b1) (node-rank b2))
(define (join b1 b2)
  (cond
    [(< (node-value b1) (node-value b2))
     (make-node (node-value b1)
                (+ (node-rank b1) 1)
                (cons b2 (node-children b1)))]
    [else
     (make-node (node-value b2)
                (+ (node-rank b2) 1)
                (cons b1 (node-children b2)))]))


;; heap-rank : binomial-heap -> nat or #f
(define (heap-rank b)
  (cond
    [(empty? b) #f]
    [else (node-rank (first b))]))

;; tree-rank : binomial-tree or #f -> nat or #f
(define (tree-rank b)
  (cond
    [(boolean? b) #f]
    [else (node-rank b)]))
