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

(define (insert-nums nums)
  (cond
    [(empty? nums) empty-heap]
    [else (insert (insert-nums (rest nums)) (first nums))]))

(define (remove-mins h)
  (cond
    [(empty-heap? h) '()]
    [else (cons (find-min h)
                (remove-mins (remove-min h)))]))

(check-expect (remove-mins (insert-nums (list 0))) (list 0))
(check-expect (remove-mins (insert-nums (list 0 1))) (list 0 1))
(check-expect (remove-mins (insert-nums (list 1 0))) (list 0 1))
(check-expect (remove-mins (insert-nums (list 0 0))) (list 0 0))

(check-expect (remove-mins (meld (insert-nums (list 0 1 2))
                                 (insert-nums (list 2 1 0))))
              (list 0 0 1 1 2 2))

(define zero-thru-sixteen (list 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16))
(check-expect (remove-mins (insert-nums zero-thru-sixteen))
              zero-thru-sixteen)
(check-expect (remove-mins (insert-nums (reverse zero-thru-sixteen)))
              zero-thru-sixteen)

;; a binomial-heap is:
;;  (listof (or #f binomial-tree))
;; invariant: each element of the list
;; has the next largest rank from the
;; previous element in the list, unless
;; that element is #f, in which case it
;; stands for an empty binomial-tree with
;; the missing order.

;; a binomial-tree is:
;;  (make-node number (listof binomial-tree))
;; invariant:
;;  -- heap invariant
;;  -- if 'children' is non-empty, then the first
;;     element of 'children' has the same shape & size as
;;     the tree you get by removing that first child
;;     and making a new tree from the rest.
;;
(define-struct node (value children))



;; find-min : binomial-heap -> number or #f
(define (find-min b)
  (cond
    [(empty? b) #f]
    [else
     (cond
       [(boolean? (first b)) (find-min (rest b))]
       [else
        (pick-one (node-value (first b))
                  (find-min (rest b)))])]))

;; pick-one : number (number or false) -> number
(define (pick-one n1 n2/f)
  (cond
    [(number? n2/f) (min n1 n2/f)]
    [else n1]))

(define empty-heap '())
(define (empty-heap? b) (empty? b))

;; insert : binomial-heap number -> binomial-heap
(define (insert b n)
  (add #false (list (make-node n '())) b))

(define (remove-min b)
  (local [(define small-root (find-min b))
          (define heap-without-small-root
            (remove-tree-with-root b small-root))
          (define node-with-small-root
            (find-tree-with-root b small-root))]
    (add #false
         (remove-trailing-falses heap-without-small-root)
         (reverse (node-children node-with-small-root)))))

(define (remove-tree-with-root b v)
  (cond
    [(empty? b) (error 'ack)]
    [else
     (cond
       [(boolean? (first b)) (cons #f (remove-tree-with-root (rest b) v))]
       [(= (node-value (first b)) v)
        (cons #f (rest b))]
       [else
        (cons (first b) (remove-tree-with-root (rest b) v))])]))

(define (remove-trailing-falses b)
  (cond
    [(empty? b) '()]
    [else
     (local [(define no-trailing-falses (remove-trailing-falses (rest b)))]
       (cond
         [(and (boolean? (first b)) (empty? no-trailing-falses))
          '()]
         [else (cons (first b) no-trailing-falses)]))]))

(define (find-tree-with-root b v)
  (cond
    [(empty? b) (error 'ack)]
    [else
     (cond
       [(boolean? (first b)) (find-tree-with-root (rest b) v)]
       [(= (node-value (first b)) v)
        (first b)]
       [else
        (find-tree-with-root (rest b) v)])]))

(define (meld h1 h2) (add #false h1 h2))

;; add : (or #f binomial-tree) binomial-heap binomial-heap -> binomial-heap
(define (add c-in h1 h2)
  (cond
    [(and (empty? h1) (empty? h2) (boolean? c-in))
     '()]
    [(and (empty? h1) (cons? h2) (boolean? c-in))
     h2]
    [(and (cons? h1) (empty? h2) (boolean? c-in))
     h1]
    [(and (empty? h1) (empty? h2) (node? c-in))
     (list c-in)]
    [(and (empty? h1) (cons? h2) (node? c-in))
     (add-one c-in h2)]
    [(and (cons? h1) (empty? h2) (node? c-in))
     (add-one c-in h1)]
    [(and (cons? h1) (cons? h2) (boolean? c-in))
     (cons #false (add (join (first h1) (first h2)) (rest h1) (rest h2)))]
    [(and (cons? h1) (cons? h2) (node? c-in))
     (cons c-in
           (add (join (first h1) (first h2))
                (rest h1) (rest h2)))]))

(define (add-one t h)
  (cond
    [(empty? h) (list t)]
    [else
     (cond
       [(boolean? (first h))
        (cons t (rest h))]
       [else
        (cons #false (add-one (join (first h) t)
                              (rest h)))])]))

;; join : (or #false binomial-tree) (or #false binomial-tree) -> (or #false binomial-tree)
(define (join t1 t2)
  (cond
    [(boolean? t1) t2]
    [(boolean? t2) t1]
    [else
     (cond
       [(< (node-value t1) (node-value t2))
        (make-node (node-value t1)
                   (cons t2 (node-children t1)))]
       [else
        (make-node (node-value t2)
                   (cons t1 (node-children t2)))])]))
