;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname lec06-avl) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(require "provide.rkt") (provide insert AVL? in-order)

;; an AVL-tree is either:
;;  (make-node number AVL-tree AVL-tree number)
;;  "leaf"
;; INVARIANT :
;;   (<= -1 (- (height left) (height right)) 1)
(define-struct node (value left right height))

(define (height tree)
  (cond
    [(equal? tree "leaf") 0]
    [else (node-height tree)]))

(define (build-node value left right)
  (make-node value left right (+ 1 (max (height left) (height right)))))

(define size1 (build-node 2 "leaf" "leaf"))
(define size2l (build-node 3 size1 "leaf"))
(define size2r (build-node 1 "leaf" size1))
(define size3
  (build-node 4
              (build-node 2 "leaf" "leaf")
              (build-node 6 "leaf" "leaf")))

(check-expect (height "leaf") 0)
(check-expect (height size1) 1)
(check-expect (height size2l) 2)
(check-expect (height size2r) 2)
(check-expect (height size3) 2)


;; AVL? : AVL-tree -> number
;; checks to be sure the given tree has the AVL invariant
(check-expect (AVL? "leaf") #true)
(check-expect (AVL? size1) #true)
(check-expect (AVL? size2l) #true)
(check-expect (AVL? size2r) #true)
(check-expect (AVL? size3) #true)
(check-expect (AVL? (build-node
                     0
                     "leaf"
                     (build-node
                      1
                      "leaf"
                      (build-node
                       2
                       "leaf"
                       "leaf"))))
              #false)

(define (AVL? tree)
  (cond
    [(equal? tree "leaf") #true]
    [else
     (and (AVL? (node-left tree))
          (AVL? (node-right tree))
          (= (node-height tree) (compute-height tree))
          (values-all? < (node-value tree) (node-left tree))
          (values-all? > (node-value tree) (node-right tree))
          (<= -1
              (- (height (node-left tree))
                 (height (node-right tree)))
              1))]))

;; values-all? : (number number -> boolean) number AVL-tree -> boolean
;; to determine if all of the values in 'tree' are 'cmp' than 'val'
(check-expect (values-all? < 0 "leaf") #true)
(check-expect (values-all? > 0 (build-node 2
                                           (build-node 1 "leaf" "leaf")
                                           (build-node 3 "leaf" "leaf")))
              #true)
(check-expect (values-all? > 1 (build-node 2
                                           (build-node 1 "leaf" "leaf")
                                           (build-node 3 "leaf" "leaf")))
              #false)
(define (values-all? cmp val tree)
  (cond
    [(equal? tree "leaf") #true]
    [else (and (cmp (node-value tree) val)
               (values-all? cmp val (node-left tree))
               (values-all? cmp val (node-right tree)))]))

(define (compute-height tree)
  (cond
    [(equal? tree "leaf") 0]
    [else (+ (max (compute-height (node-left tree))
                  (compute-height (node-right tree)))
             1)]))


;; in-order : AVL-tree -> (listof number)
(check-expect (in-order "leaf") '())
(check-expect (in-order (build-node 2
                                    (build-node 1 "leaf" "leaf")
                                    (build-node 3 "leaf" "leaf")))
              (list 1 2 3))
(define (in-order tree)
  (cond
    [(equal? tree "leaf") '()]
    [else (append (in-order (node-left tree))
                  (list (node-value tree))
                  (in-order (node-right tree)))]))

;; an AVL-tree/grow is:
;;  (make-AG AVL-tree boolean)
;;   INVARIANT: the AVL-tree is always a 'node'
(define-struct AG (tree grew?))

;; insert : AVL-tree number -> AVL-tree
(define (insert tree value)
  (AG-tree (insert/grow tree value)))

;; insert/grow : AVL-tree number -> AVL-tree/grow
(define (insert/grow tree value)
  (cond
    [(equal? tree "leaf")
     (make-AG (build-node value "leaf" "leaf") #true)]
    [else
     (cond
       [(< value (node-value tree))
        (rebalance-left (node-value tree)
                        (insert/grow (node-left tree) value)
                        (node-right tree)
                        (balance tree))]
       [(= value (node-value tree)) (make-AG tree #false)]
       [(> value (node-value tree))
        (rebalance-right (node-value tree)
                         (node-left tree)
                         (insert/grow (node-right tree) value)
                         (balance tree))])]))

;; balance : AVL-tree -> "even" or "left" or "right"
;; assumption: node is not "leaf"
(check-expect (balance (build-node 2 "leaf" "leaf")) "even")
(check-expect (balance (build-node 2 (build-node 1 "leaf" "leaf") "leaf")) "left")
(check-expect (balance (build-node 2 "leaf" (build-node 3 "leaf" "leaf"))) "right")
(define (balance node)
  (cond
    [(= (height (node-left node)) (height (node-right node)))
     "even"]
    [(= (- (height (node-left node)) 1) (height (node-right node)))
     "left"]
    [(= (+ (height (node-left node)) 1) (height (node-right node)))
     "right"]))

(define (rebalance-left value AG-left right the-balance)
  (cond
    [(AG-grew? AG-left)
     (cond
       [(equal? the-balance "left")
        (cond
          [(equal? "right" (balance (AG-tree AG-left)))
           (local [(define x value)
                   (define y (node-value (AG-tree AG-left)))
                   (define z (node-value (node-right (AG-tree AG-left))))
                   (define A (node-left (AG-tree AG-left)))
                   (define B (node-left (node-right (AG-tree AG-left))))
                   (define C (node-right (node-right (AG-tree AG-left))))
                   (define D right)]
             (make-AG
              (build-node
               z
               (build-node y A B)
               (build-node x C D))
              #false))]
          [else
           (local [(define x value)
                   (define y (node-value (AG-tree AG-left)))
                   (define A (node-left (AG-tree AG-left)))
                   (define B (node-right (AG-tree AG-left)))
                   (define C right)]
             (make-AG
              (build-node y A (build-node x B C))
              #true))])]
       [(equal? the-balance "right")
        (make-AG (build-node value (AG-tree AG-left) right)
                 #false)]
       [(equal? the-balance "even")
        (make-AG (build-node value (AG-tree AG-left) right)
                 #true)])]
    [else
     (make-AG (build-node value (AG-tree AG-left) right)
              #false)]))

(define (rebalance-right value left AG-right the-balance)
  (cond
    [(AG-grew? AG-right)
     (cond
       [(equal? the-balance "left")
        (make-AG (build-node value left (AG-tree AG-right))
                 #false)]
       [(equal? the-balance "right")
        (cond
          [(equal? "left" (balance (AG-tree AG-right)))
           (local [(define x value)
                   (define y (node-value (AG-tree AG-right)))
                   (define z (node-value (node-left (AG-tree AG-right))))
                   (define A left)
                   (define B (node-left (node-left (AG-tree AG-right))))
                   (define C (node-right (node-left (AG-tree AG-right))))
                   (define D (node-right (AG-tree AG-right)))]
             (make-AG
              (build-node z
                          (build-node x A B)
                          (build-node y C D))
              #false))]
          [else
           (local [(define x value)
                   (define y (node-value (AG-tree AG-right)))
                   (define A left)
                   (define B (node-left (AG-tree AG-right)))
                   (define C (node-right (AG-tree AG-right)))]
             (make-AG
              (build-node y
                          (build-node x A B)
                          C)
              #true))])]
       [(equal? the-balance "even")
        (make-AG (build-node value left (AG-tree AG-right))
                 #true)])]
    [else
     (make-AG (build-node value left (AG-tree AG-right))
              #false)]))
