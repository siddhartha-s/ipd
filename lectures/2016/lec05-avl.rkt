;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname lec05-avl) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;; an AVL-tree is either:
;;  (make-db number AVL-tree AVL-tree number)
;;  "leaf"
;; INVARIANT :
;;   (<= -1 (- (height left) (height right)) 1)
(define-struct db (value left right height))

;; height : AVL-tree -> number
(check-expect (height "leaf") 0)
(check-expect (height (make-db 3 "leaf" "leaf" 1)) 1)
(define (height tree)
  (cond
    [(equal? tree "leaf") 0]
    [else (db-height tree)]))

(define (build-db value left right)
  (make-db value
           left
           right
           (+ (max (height left)
                   (height right))
              1)))

(define size1 (build-db 2 "leaf" "leaf"))
(define size2l (build-db 3 size1 "leaf"))
(define size2r (build-db 1 "leaf" size1))
(define size3
  (build-db 4
            (build-db 2 "leaf" "leaf")
            (build-db 6 "leaf" "leaf")))

;; AVL? : AVL-tree -> number
;; checks to be sure the given tree has the AVL invariant,
;; the BST invariant, and that the height fields are correct
(check-expect (AVL? "leaf") #true)
(check-expect (AVL? size1) #true)
(check-expect (AVL? size2l) #true)
(check-expect (AVL? size2r) #true)
(check-expect (AVL? size3) #true)
(check-expect (AVL? (build-db
                     0
                     "leaf"
                     (build-db
                      1
                      "leaf"
                      (build-db
                       2
                       "leaf"
                       "leaf"))))
              #false)

(define (AVL? tree)
  (cond
    [(equal? tree "leaf") #true]
    [else
     (and (AVL? (db-left tree))
          (AVL? (db-right tree))
          (= (db-height tree) (compute-height tree))
          (values-all? < (db-value tree) (db-left tree))
          (values-all? > (db-value tree) (db-right tree))
          (<= -1
              (- (height (db-left tree))
                 (height (db-right tree)))
              1))]))

;; values-all? : (number number -> boolean) number AVL-tree -> boolean
;; to determine if all of the values in 'tree' are 'cmp' than 'val'
(check-expect (values-all? < 0 "leaf") #true)
(check-expect (values-all? > 0 (build-db 2
                                         (build-db 1 "leaf" "leaf")
                                         (build-db 3 "leaf" "leaf")))
              #true)
(check-expect (values-all? > 1 (build-db 2
                                         (build-db 1 "leaf" "leaf")
                                         (build-db 3 "leaf" "leaf")))
              #false)
(define (values-all? cmp val tree)
  (cond
    [(equal? tree "leaf") #true]
    [else (and (cmp (db-value tree) val)
               (values-all? cmp val (db-left tree))
               (values-all? cmp val (db-right tree)))]))

;; compute-height : AVL-tree -> number
;; ignores the `height` field of the AVL tree and
;; computes the heights by counting
(check-expect (compute-height "leaf") 0)
(check-expect (compute-height (make-db 1 "leaf" "leaf" 100)) 1)
(check-expect (compute-height (make-db 1 (make-db 1 "leaf" "leaf" 100) "leaf" 100)) 2)
(define (compute-height tree)
  (cond
    [(equal? tree "leaf") 0]
    [else (+ (max (compute-height (db-left tree))
                  (compute-height (db-right tree)))
             1)]))


;; in-order : AVL-tree -> (listof number)
;; returns the numbers in the AVL tree as they would appear in an inorder traversal
(check-expect (in-order "leaf") '())
(check-expect (in-order (build-db 2
                                  (build-db 1 "leaf" "leaf")
                                  (build-db 3 "leaf" "leaf")))
              (list 1 2 3))
(define (in-order tree)
  (cond
    [(equal? tree "leaf") '()]
    [else (append (in-order (db-left tree))
                  (list (db-value tree))
                  (in-order (db-right tree)))]))

;; insert : AVL-tree number -> AVL-tree
;; inserts 'value' into 'tree', returning a new AVL tree.
(define (insert tree value)
  (cond
    [(equal? tree "leaf")
     (build-db value "leaf" "leaf")]
    [else
     (cond
       [(< value (db-value tree))
        (rebalance-left (db-value tree)
                        (db-left tree)
                        (insert (db-left tree) value)
                        (db-right tree)
                        (balance tree))]
       [(= value (db-value tree)) tree]
       [(> value (db-value tree))
        (rebalance-right (db-value tree)
                         (db-left tree)
                         (db-right tree)
                         (insert (db-right tree) value)
                         (balance tree))])]))

;; balance : AVL-tree -> "even" or "left" or "right"
;; assumption: db is not "leaf"
(check-expect (balance (build-db 2 "leaf" "leaf")) "even")
(check-expect (balance (build-db 2 (build-db 1 "leaf" "leaf") "leaf")) "left")
(check-expect (balance (build-db 2 "leaf" (build-db 3 "leaf" "leaf"))) "right")
(define (balance db)
  (local [(define delta (- (height (db-left db))
                           (height (db-right db))))]
    (cond
      [(= delta 0) "even"]
      [(= delta 1) "left"]
      [(= delta -1) "right"])))

(define (rebalance-left value original-left new-left right the-balance)
  (cond
    [(= (height original-left) (height new-left))
     (build-db value new-left right)]
    [else
     (cond
       [(equal? the-balance "left")
        (cond
          [(equal? "right" (balance new-left))
           (local [(define x value)
                   (define y (db-value new-left))
                   (define z (db-value (db-right new-left)))
                   (define A (db-left new-left))
                   (define B (db-left (db-right new-left)))
                   (define C (db-right (db-right new-left)))
                   (define D right)]
             (build-db
              z
              (build-db y A B)
              (build-db x C D)))]
          [else
           (local [(define x value)
                   (define y (db-value new-left))
                   (define A (db-left new-left))
                   (define B (db-right new-left))
                   (define C right)]
             (build-db y A (build-db x B C)))])]
       [(equal? the-balance "right")
        (build-db value new-left right)]
       [(equal? the-balance "even")
        (build-db value new-left right)])]))

(define (rebalance-right value left original-right new-right the-balance)
  (cond
    [(= (height original-right) (height new-right))
     (build-db value left new-right)]
    [else
     (cond
       [(equal? the-balance "left")
        (build-db value left new-right)]
       [(equal? the-balance "right")
        (cond
          [(equal? "left" (balance new-right))
           (local [(define x value)
                   (define y (db-value new-right))
                   (define z (db-value (db-left new-right)))
                   (define A left)
                   (define B (db-left (db-left new-right)))
                   (define C (db-right (db-left new-right)))
                   (define D (db-right new-right))]
             (build-db z
                       (build-db x A B)
                       (build-db y C D)))]
          [else
           (local [(define x value)
                   (define y (db-value new-right))
                   (define A left)
                   (define B (db-left new-right))
                   (define C (db-right new-right))]
             (build-db y
                       (build-db x A B)
                       C))])]
       [(equal? the-balance "even")
        (build-db value left new-right)])]))

;; insert-many : listof-number -> AVL-tree
(check-expect (insert-many '()) "leaf")
(check-expect (insert-many (list 1 2 3))
              (build-db 2
                        (build-db 1 "leaf" "leaf")
                        (build-db 3 "leaf" "leaf")))
(define (insert-many lon)
  (cond
    [(empty? lon) "leaf"]
    [else (insert (insert-many (rest lon))
                  (first lon))]))

(define (insert-worked? elements)
  (local [(define tree (insert-many elements))]
    (and (AVL? tree)
         (equal? (in-order tree)
                 (remove-duplicates (sort elements <))))))

;; remove-duplicates : listof-number -> listof-number
(check-expect (remove-duplicates '()) '())
(check-expect (remove-duplicates (list 1)) (list 1))
(check-expect (remove-duplicates (list 1 2)) (list 1 2))
(check-expect (remove-duplicates (list 1 1)) (list 1))
(check-expect (remove-duplicates (list 1 2 1)) (list 1 2))
(define (remove-duplicates lon)
  (cond
    [(empty? lon) lon]
    [else
     (cons (first lon)
           (remove-all (first lon)
                       (remove-duplicates (rest lon))))]))

(check-satisfied '() insert-worked?)
(check-satisfied (list 1) insert-worked?)
(check-satisfied (list 1 2) insert-worked?)
(check-satisfied (list 2 1) insert-worked?)

;; permutations of three elements
(check-satisfied (list 1 2 3) insert-worked?)
(check-satisfied (list 2 1 3) insert-worked?)
(check-satisfied (list 1 3 2) insert-worked?)
(check-satisfied (list 3 1 2) insert-worked?)
(check-satisfied (list 2 3 1) insert-worked?)
(check-satisfied (list 3 2 1) insert-worked?)

;; permutations of four elements
(check-satisfied (list 4 1 2 3) insert-worked?)
(check-satisfied (list 1 4 2 3) insert-worked?)
(check-satisfied (list 1 2 4 3) insert-worked?)
(check-satisfied (list 1 2 3 4) insert-worked?)
(check-satisfied (list 4 2 1 3) insert-worked?)
(check-satisfied (list 2 4 1 3) insert-worked?)
(check-satisfied (list 2 1 4 3) insert-worked?)
(check-satisfied (list 2 1 3 4) insert-worked?)
(check-satisfied (list 4 1 3 2) insert-worked?)
(check-satisfied (list 1 4 3 2) insert-worked?)
(check-satisfied (list 1 3 4 2) insert-worked?)
(check-satisfied (list 1 3 2 4) insert-worked?)
(check-satisfied (list 4 3 1 2) insert-worked?)
(check-satisfied (list 3 4 1 2) insert-worked?)
(check-satisfied (list 3 1 4 2) insert-worked?)
(check-satisfied (list 3 1 2 4) insert-worked?)
(check-satisfied (list 4 2 3 1) insert-worked?)
(check-satisfied (list 2 4 3 1) insert-worked?)
(check-satisfied (list 2 3 4 1) insert-worked?)
(check-satisfied (list 2 3 1 4) insert-worked?)
(check-satisfied (list 4 3 2 1) insert-worked?)
(check-satisfied (list 3 4 2 1) insert-worked?)
(check-satisfied (list 3 2 4 1) insert-worked?)
(check-satisfied (list 3 2 1 4) insert-worked?)

;; duplicate elements in different configurations
(check-satisfied (list 1 1) insert-worked?)
(check-satisfied (list 1 1 2 3) insert-worked?)
(check-satisfied (list 3 1 2 3) insert-worked?)
