;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname rbst) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;; A RandNumTree is one of:
;; - (make-node Number Natural RandNumTree RandNumTree)
;; - '()
(define-struct node (key size left right))

;; An example tree:
(define A-TREE
  (make-node 4 3
             (make-node 2 1 '() '())
             (make-node 6 1 '() '())))

;; contains? : RandNumTree Number -> Boolean
;; Returns whether the given Number is in the tree.
;;
;; Strategy: struct. decomp.
(define (contains? t k)
  (cond
    [(empty? t) #false]
    [else
     (cond
      [(< k (node-key t)) (contains? (node-left t) k)]
      [(> k (node-key t)) (contains? (node-right t) k)]
      [else #true])]))

(check-expect (contains? A-TREE 2) #t)
(check-expect (contains? A-TREE 4) #t)
(check-expect (contains? A-TREE 6) #t)
(check-expect (contains? A-TREE 1) #f)
(check-expect (contains? A-TREE 3) #f)

;; tree-size : RandNumTree -> Natural
;; Gets the size of a possibly empty tree.
;;
;; Strategy: struct. decomp.
(define (tree-size t)
  (cond
    [(node? t) (node-size t)]
    [else      0]))

(check-expect (tree-size '()) 0)
(check-expect (tree-size A-TREE) 3)

;; new-node : Natural RandNumTree RandNumTree-> RandNumTree
;; Creates a new singleton tree with the given value.
;;
;; Strategy: function composition
(define (new-node k l r)
  (make-node k (+ 1 (tree-size l) (tree-size r)) l r))

(check-expect (new-node 5 '() '()) (make-node 5 1 '() '()))
(check-expect (new-node 1 '() A-TREE) (make-node 1 4 '() A-TREE))


;; leaf-insert : RandNumTree Number -> RandNumTree
;; Inserts a key as a leaf, returning the modified tree.
;;
;; Strategy: struct. decomp.
(define (leaf-insert t k)
  (cond
    [(empty? t) (new-node k '() '())]
    ;; Strategy note: the remaining cases should be, strictly speaking,
    ;; a nested cond in the `else` branch of the outer cond.
    [(< k (node-key t))
     (new-node (node-key t)
               (leaf-insert (node-left t) k)
               (node-right t))]
    [(> k (node-key t))
     (new-node (node-key t)
               (node-left t)
               (leaf-insert (node-right t) k))]
    [else
     t]))

(check-expect (leaf-insert A-TREE 2) A-TREE)
(check-expect (leaf-insert A-TREE 4) A-TREE)
(check-expect (leaf-insert A-TREE 6) A-TREE)
(check-expect (leaf-insert A-TREE 5)
              (make-node 4 4
                         (make-node 2 1 '() '())
                         (make-node 6 2
                                    (make-node 5 1 '() '())
                                    '())))
