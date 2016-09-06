#lang dssl

;; A RandNumTree is one of:
;; - (node Number Natural RandNumTree RandNumTree)
;; - '()
(define-struct node (key size left right))

;; RandNumTree Number -> Boolean
;; Returns whether the given Number is in the tree.
(define (member? t k)
  (cond
    [(empty? t) #false]
    [(< k (node-key t)) (member? (node-left t) k)]
    [(> k (node-key t)) (member? (node-right t) k)]
    [else #true]))

;; RandNumTree -> Natural
;; Gets the size of a possibly empty tree.
(define (tree-size t)
  (if (node? t) (node-size t) 0))

;; Natural -> RandNumTree
;; Creates a new singleton tree with the given value.
(define (new-node k)
  (node k 1 '() '()))

;; RandNumTree -> Void
;; Sets the size at the root based on the sizes of the subtrees
(define (fix-size! t)
  (set-node-size! t (+ 1
                       (tree-size (node-left t))
                       (tree-size (node-right t)))))

;; RandNumTree Number -> RandNumTree
;; Inserts a key as a leaf, returning the modified tree.
(define (leaf-insert! t k)
  (cond
    [(empty? t) (new-node k)]
    [(< k (node-key t))
     (set-node-left! t (leaf-insert! (node-left t) k))
     (fix-size! t)
     t]
    [(> k (node-key t))
     (set-node-right! t (leaf-insert! (node-right t) k))
     (fix-size! t)
     t]
    [else
     t]))

;; RandNumTree -> Void
;; Performs a right rotation
(define (rotate-right! d)
  (define b (node-left d))
  (set-node-left! d (node-right b))
  (set-node-right! b d)
  (fix-size! d)
  (fix-size! b)
  b)

;; RandNumTree -> Void
;; Performs a left rotation
(define (rotate-left! b)
  (define d (node-right b))
  (set-node-right! b (node-left d))
  (set-node-left! d b)
  (fix-size! b)
  (fix-size! d)
  d)

;; RandNumTree Number -> RandNumTree
;; Inserts an element at the root, returning the modified tree.
(define (root-insert! t k)
  (cond
    [(empty? t) (new-node k)]
    [(< k (node-key t))
     (set-node-left! t (root-insert! (node-left t) k))
     (rotate-right! t)]
    [(> k (node-key t))
     (set-node-right! t (root-insert! (node-right t) k))
     (rotate-left! t)]
    [else
     t]))

;; RandNumTree Number -> RandNumTree
;; Inserts an element, maintaining randomness and returning the modified tree.
(define (insert! t k)
  (cond
    [(empty? t) (new-node k)]
    [(zero? (random (add1 (tree-size t))))
     (root-insert! t k)]
    [(< k (node-key t))
     (set-node-left! t (insert! (node-left t) k))
     (fix-size! t)
     t]
    [(> k (node-key t))
     (set-node-right! t (insert! (node-right t) k))
     (fix-size! t)
     t]
    [else t]))



;; RandNumTree RandNumTree -> RandNumTree
;; Joins two trees, assuming all the keys of the first are less than
;; all the keys of the second.
(define (join! t1 t2)
  (cond
    [(empty? t1) t2]
    [(empty? t2) t1]
    [(< (random (+ (tree-size t1) (tree-size t2))) (tree-size t1))
     (set-node-right! t1 (join! (node-right t1) t2))
     (fix-size! t1)
     t1]
    [else
     (set-node-left! t2 (join! t1 (node-left t2)))
     (fix-size! t2)
     t2]))

;; RandNumTree Number -> RandNumTree
;; Deletes an element from a tree, returning the modified tree.
(define (delete! t k)
  (cond
    [(empty? t) t]
    [(< k (node-key t))
     (set-node-left! t (delete! (node-left t) k))
     (fix-size! t)
     t]
    [(> k (node-key t))
     (set-node-right! t (delete! (node-right t) k))
     (fix-size! t)
     t]
    [else
     (join! (node-left t) (node-right t))]))

;; Natural [Natural -> Number] -> RandNumTree
;; Builds a random tree of `n` elements, using `f` to produce each element
;; (like `build-list`)
(define (build-tree n f)
  (define result '())
  (for ([i n]) (set! result (insert! result (f i))))
  result)

;; RandNumTree -> Natural
;; Returns the height (max depth) of a tree.
(define (height t)
  (cond
    [(node? t) (add1 (max (height (node-left t))
                          (height (node-right t))))]
    [else 0]))

;; Natural -> Natural
;; Builds a tree with `n` elements and returns its height.
(define (sample-height n)
  (height (build-tree n identity)))
