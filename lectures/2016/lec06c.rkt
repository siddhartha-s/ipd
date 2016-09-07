;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname lec06c-rndbst) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;; A RandNumTree is one of:
;; - (make-node Number Natural RandNumTree RandNumTree)
;; - '()
(define-struct node (key size left right))

;; An example tree:
(define A-TREE
  (make-node 4 3
             (make-node 2 1 '() '())
             (make-node 6 1 '() '())))

;; RandNumTree Number -> Boolean
;; Returns whether the given Number is in the tree.
(define (contains? t k)
  (cond
    [(empty? t) #false]
    [(< k (node-key t)) (contains? (node-left t) k)]
    [(> k (node-key t)) (contains? (node-right t) k)]
    [else #true]))

(check-expect (contains? A-TREE 2) #t)
(check-expect (contains? A-TREE 4) #t)
(check-expect (contains? A-TREE 6) #t)
(check-expect (contains? A-TREE 1) #f)
(check-expect (contains? A-TREE 3) #f)

;; RandNumTree -> Natural
;; Gets the size of a possibly empty tree.
(define (tree-size t)
  (if (node? t) (node-size t) 0))

(check-expect (tree-size '()) 0)
(check-expect (tree-size A-TREE) 3)

;; Natural -> RandNumTree
;; Creates a new singleton tree with the given value.
(define (new-node k l r)
  (make-node k (+ 1 (tree-size l) (tree-size r)) l r))

(check-expect (new-node 5 '() '()) (make-node 5 1 '() '()))
(check-expect (new-node 1 '() A-TREE) (make-node 1 4 '() A-TREE))

;; RandNumTree -> Void
;; Sets the size at the root based on the sizes of the subtrees
(define (fix-size t)
  (make-node (node-key t)
             (+ 1
                (tree-size (node-left t))
                (tree-size (node-right t)))
             (node-left t)
             (node-right t)))

(check-expect (fix-size A-TREE) A-TREE)
(check-expect (fix-size (make-node 4 0
                                   (make-node 2 1 '() '())
                                   (make-node 6 1 '() '())))
              A-TREE)

;; RandNumTree Number -> RandNumTree
;; Inserts a key as a leaf, returning the modified tree.
(define (leaf-insert t k)
  (cond
    [(empty? t) (new-node k '() '())]
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

;; RandNumTree -> Void
;; Performs a right rotation
(define (rotate-right d)
  (new-node (node-key (node-left d))
            (node-left (node-left d))
            (new-node (node-key d)
                      (node-right (node-left d))
                      (node-right d))))

(check-expect (rotate-right A-TREE)
              (make-node 2 3 '()
                         (make-node 4 2 '()
                                    (make-node 6 1 '() '()))))

;; RandNumTree -> Void
;; Performs a left rotation
(define (rotate-left b)
  (new-node (node-key (node-right b))
            (new-node (node-key b)
                      (node-left b)
                      (node-left (node-right b)))
            (node-right (node-right b))))

(check-expect (rotate-left A-TREE)
              (make-node 6 3
                         (make-node 4 2
                                    (make-node 2 1 '() '())
                                    '())
                         '()))

;; RandNumTree Number -> RandNumTree
;; Inserts an element at the root, returning the modified tree.
(define (root-insert t k)
  (cond
    [(empty? t) (new-node k '() '())]
    [(< k (node-key t))
     (rotate-right (new-node
                    (node-key t)
                    (root-insert (node-left t) k)
                    (node-right t)))]
    [(> k (node-key t))
     (rotate-left (new-node
                   (node-key t)
                   (node-left t)
                   (root-insert (node-right t) k)))]
    [else
     t]))

(check-expect (root-insert A-TREE 5)
              (make-node 5 4
                         (make-node 4 2
                                    (make-node 2 1 '() '())
                                    '())
                         (make-node 6 1 '() '())))
(check-expect (root-insert A-TREE 4) A-TREE)
(check-expect (root-insert A-TREE 2)
              (make-node 2 3
                         '()
                         (make-node 4 2
                                    '()
                                    (make-node 6 1 '() '()))))

;; RandNumTree Number -> RandNumTree
;; Inserts an element, maintaining randomness and returning the modified tree.
(define (insert t k)
  (cond
    [(empty? t) (new-node k '() '())]
    [(zero? (random (add1 (tree-size t))))
     (root-insert t k)]
    [(< k (node-key t))
     (new-node (node-key t)
               (insert (node-left t) k)
               (node-right t))]
    [(> k (node-key t))
     (new-node (node-key t)
               (node-left t)
               (insert (node-right t) k))]
    [else t]))

(check-expect (tree-size (insert '() 5)) 1)
(check-expect (tree-size (insert A-TREE 3)) 4)
(check-expect (tree-size (insert A-TREE 4)) 3)
(check-expect (tree-size (insert A-TREE 2)) 3)
(check-expect (tree-size (insert A-TREE 6)) 3)

;; It's hard to get coverage when the function uses randomness,
;; so we repeat the tests two more times.

(check-expect (tree-size (insert A-TREE 3)) 4)
(check-expect (tree-size (insert A-TREE 4)) 3)
(check-expect (tree-size (insert A-TREE 2)) 3)
(check-expect (tree-size (insert A-TREE 6)) 3)

(check-expect (tree-size (insert A-TREE 3)) 4)
(check-expect (tree-size (insert A-TREE 4)) 3)
(check-expect (tree-size (insert A-TREE 2)) 3)
(check-expect (tree-size (insert A-TREE 6)) 3)

;; Natural [Natural -> Number] -> RandNumTree
;; Builds a random tree of `n` elements, using `f` to produce each element
;; (like `build-list`)
(define (build-tree n f)
  (cond
    [(zero? n) '()]
    [else      (insert (build-tree (sub1 n) f) (f (sub1 n)))]))

(check-expect (build-tree 0 identity) '())
(check-expect (build-tree 1 identity) (new-node 0 '() '()))

;; RandNumTree -> Natural
;; Returns the height (max depth) of a tree.
(define (height t)
  (cond
    [(node? t) (add1 (max (height (node-left t))
                          (height (node-right t))))]
    [else 0]))

(check-expect (height A-TREE) 2)

;; Natural -> Natural
;; Builds a tree with `n` elements and returns its height.
(define (sample-height n)
  (height (build-tree n identity)))

(check-expect (< (sample-height 100) 20) #t)

;; RandNumTree RandNumTree -> RandNumTree
;; Joins two trees, assuming all the keys of the first are less than
;; all the keys of the second.
(define (join t1 t2)
  (cond
    [(empty? t1) t2]
    [(empty? t2) t1]
    [(< (random (+ (tree-size t1) (tree-size t2))) (tree-size t1))
     (new-node (node-key t1)
               (node-left t1)
               (join (node-right t1) t2))]
    [else
     (new-node (node-key t2)
               (join t1 (node-left t2))
               (node-right t2))]))

;; RandNumTree Number -> RandNumTree
;; Deletes an element from a tree, returning the modified tree.
(define (delete t k)
  (cond
    [(empty? t) t]
    [(< k (node-key t))
     (new-node (node-key t)
               (delete (node-left t) k)
               (node-right t))]
    [(> k (node-key t))
     (new-node (node-key t)
               (node-left t)
               (delete (node-right t) k))]
    [else
     (join (node-left t) (node-right t))]))

(check-expect (delete A-TREE 2)
              (new-node 4 '() (new-node 6 '() '())))
(check-expect (delete A-TREE 6)
              (new-node 4 (new-node 2 '() '()) '()))
(check-expect (delete A-TREE 3) A-TREE)

;; Doing this four times should get us coverage 15/16 of the time.
(check-member-of (delete A-TREE 4)
                 (new-node 2 '() (new-node 6 '() '()))
                 (new-node 6 (new-node 2 '() '()) '()))
(check-member-of (delete A-TREE 4)
                 (new-node 2 '() (new-node 6 '() '()))
                 (new-node 6 (new-node 2 '() '()) '()))
(check-member-of (delete A-TREE 4)
                 (new-node 2 '() (new-node 6 '() '()))
                 (new-node 6 (new-node 2 '() '()) '()))
(check-member-of (delete A-TREE 4)
                 (new-node 2 '() (new-node 6 '() '()))
                 (new-node 6 (new-node 2 '() '()) '()))
