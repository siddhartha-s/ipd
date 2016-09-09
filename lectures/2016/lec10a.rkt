#lang dssl

;; Graph Search
;; ------------

; A Graph is:
;   (make-graph Nat (Nat[< nodes] -> [List-of Nat[< nodes]]))
; the `nodes` field tells us the number of nodes in the graph;
; the `succs` field gives us a function from each node to the list
; of its successors.
(define-struct graph (nodes succs))

(define GRAPH-0
  (make-graph 7
              (lambda (node)
                (case node
                  [(0) '(1 2 3)]
                  [(1) '(3 0 4)]
                  [(2) '(4 0)]
                  [(3) '(4 1 2)]
                  [(4) '(5 4)]
                  [(5) '()]
                  [(6) '(3 4)]))))

; A SearchTree is [Vector-of [Or Nat Bool]]
; where for each node n, the nth element is the parent node of node n;
; the value for the root is #true, and the value for unreachable nodes is
; #false.

; dfs : Graph Nat -> SearchTree
; Searches `a-graph` starting at `start-node`, returning the search
; tree.
;
; Strategy: generative recursion. Termination: `visit` can only recur
; a finite number of times, because each time it recurs it removes a
; node from the unvisited set, which can happen only a finite number of
; times.
(define (dfs a-graph start-node)
  ; : SearchTree
  (define result (make-vector (graph-nodes a-graph) #false))
  ; : Nat -> Void
  (define (visit node)
    (for ([succ ((graph-succs a-graph) node)])
      (when (false? (vector-ref result succ))
        (vector-set! result succ node)
        (visit succ))))

  (vector-set! result start-node #true)
  (visit start-node)
  result)

(check-expect (dfs GRAPH-0 0)
              (vector #t 0 3 1 3 4 #f))
(check-expect (dfs GRAPH-0 1)
              (vector 2 #t 3 1 3 4 #f))
(check-expect (dfs GRAPH-0 5)
              (vector #f #f #f #f #f #t #f))
(check-expect (dfs GRAPH-0 6)
              (vector 1 3 0 6 3 4 #t))

;; We can generalize graph search as follows. Given a graph, start with
;; an empty to-do list and empty search tree. Add the given starting
;; node to the to-do list. Then repeat so long as the to-do list is
;; non-empty: Remove a node n (any node) from the to-do list and examine
;; each of its successors. If successor s has not been seen according to
;; the search tree, then record n as s's predecessor in the search tree
;; and add s to the to-do list.

;; To implement this algorithm we need a data structure to serve as the
;; to-do list. We can partially specify the requirements as an ADT:

;; Container ADT:
;;  - empty : -> Container
;;  - empty? : Container -> Bool
;;  - add! : Container Element -> Void
;;  - remove! : Container -> Element

;; Different container implementations will remove nodes in different
;; orders, yielding different kinds of graph searches.

;; We can write our search in a generic way, where we don’t specify
;; ahead of time what kind of container to use for the to-do list. To do
;; this, we make a structure that holds functions for all the container
;; ADT operations, and then pass that structure to the search function.

; A [*CONTAINER-OF* X] is (for some Repr)
;  (make-container [-> Repr]
;                  [Repr -> Bool]
;                  [Repr X -> Void]
;                  [Repr -> X])
; where the four functions are interpreted as the operations of the
; Container ADT.
(define-struct container (empty empty? add! remove!))


; generic-search : [*CONTAINER-OF* Nat] Graph Nat -> SearchTree
; Performs a graph search from the given start node, using the given
; container implementation to order the search.

; Strategy: generative iteration. Termination: each iteration through
; the loop removes a node from `to-do`, and each node is added to `to-do`
; at most once, since it is also marked in `result` as having been visited,
; and thus won’t be visited again.
(define (generic-search *C* a-graph start-node)
  (define result (make-vector (graph-nodes a-graph) #false))
  (define to-do ((container-empty *C*)))
  (vector-set! result start-node #true)
  ((container-add! *C*) to-do start-node)
  (while (not ((container-empty? *C*) to-do))
    (define node ((container-remove! *C*) to-do))
    (for ([succ ((graph-succs a-graph) node)])
      (when (false? (vector-ref result succ))
        (vector-set! result succ node)
        ((container-add! *C*) to-do succ))))
  result)

;; Two different implementations of *CONTAINER-OF*:

(define-struct stack (list))
(define *STACK*
  (make-container
   (lambda () (make-stack '()))
   (lambda (s) (empty? (stack-list s)))
   (lambda (s x) (set-stack-list! s (cons x (stack-list s))))
   (lambda (s)
     (define result (first (stack-list s)))
     (set-stack-list! s (rest (stack-list s)))
     result)))
     
(define-struct bankers-queue (front back))
(define *BANKERS-QUEUE*
  (make-container
   (lambda () (make-bankers-queue '() '()))
   (lambda (q)
     (and (empty? (bankers-queue-front q))
          (empty? (bankers-queue-back q))))
   (lambda (q x)
     (set-bankers-queue-back! q (cons x (bankers-queue-back q))))
   (lambda (q)
     (cond
       [(cons? (bankers-queue-front q))
        (define result (first (bankers-queue-front q)))
        (set-bankers-queue-front! q (rest (bankers-queue-front q)))
        result]
       [else
        (define new-front (reverse (bankers-queue-back q)))
        (set-bankers-queue-front! q (rest new-front))
        (set-bankers-queue-back! q '())
        (first new-front)]))))

;; DFS:
(check-expect (generic-search *STACK* GRAPH-0 0)
              (vector #t 0 0 0 3 4 #f))
(check-expect (generic-search *STACK* GRAPH-0 5)
              (vector #f #f #f #f #f #t #f))
(check-expect (generic-search *STACK* GRAPH-0 6)
              (vector 2 3 3 6 6 4 #true))

;; BFS:
(check-expect (generic-search *BANKERS-QUEUE* GRAPH-0 0)
              (vector #t 0 0 0 1 4 #f))
(check-expect (generic-search *STACK* GRAPH-0 5)
              (vector #f #f #f #f #f #t #f))
(check-expect (generic-search *BANKERS-QUEUE* GRAPH-0 6)
              (vector 1 3 3 6 6 4 #t))
