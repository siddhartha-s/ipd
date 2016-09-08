#lang dssl

;; Problem: find the shortest path between two points in a weighted graph.
;;
;; It turns out that the reasonable ways to compute this need to compute more
;; information than we actually need, in particular, we typically will need to
;; compute the shortest path from the starting node to *every* node. This is
;; called the Single-Source Shortest Path problem.

;; To work on this problem, first we need a representation of weighted graphs.

; A WGraph is (make-wgraph Nat [Nat Nat -> Weight])
;   where
; Weight is one of:
;  - RealNumber
;  - 'inf
(define-struct wgraph (nodes weights))
; Interpretation: wgraph-nodes gives the number of nodes in the graph, and
; the function wgraph-weights returns the weight of the edge between any
; pair of nodes. If there is no edge, we represent this with the weight +inf.0.

; weight+ : Weight Weight -> Weight
; Adds two weights, respecting infinity.
(define (weight+ a b)
  (if (and (number? a) (number? b))
      (+ a b)
      'inf))

(check-expect (weight+ 3 4) 7)
(check-expect (weight+ 'inf 4) 'inf)
(check-expect (weight+ 3 'inf) 'inf)
(check-expect (weight+ 'inf 'inf) 'inf)

; weight< : Weight Weight -> Bool
; Compares two weights, respecting infinity.
(define (weight< a b)
  (cond
    [(symbol? a) #false]
    [(symbol? b) #true]
    [else (< a b)]))

(check-expect (weight< 3 4) #true)
(check-expect (weight< 4 3) #false)
(check-expect (weight< 'inf 'inf) #false)
(check-expect (weight< 3 'inf) #true)
(check-expect (weight< 'inf 4) #false)

;; Graph from Wikipedia Dijkstra’s algo page
(define A-GRAPH
  (make-wgraph
   7
   (lambda (u v)
     (match (list u v)
       ['(1 2) 7]
       ['(2 1) 7]
       ['(1 3) 9]
       ['(3 1) 9]
       ['(1 6) 14]
       ['(6 1) 14]
       ['(2 3) 10]
       ['(3 2) 10]
       ['(2 4) 15]
       ['(4 2) 15]
       ['(3 4) 11]
       ['(4 3) 11]
       ['(3 6) 2]
       ['(6 3) 2]
       ['(4 5) 6]
       ['(5 4) 6]
       ['(5 6) 9]
       ['(6 5) 9]
       [else 'inf]))))

;; SSSP has an interesting structure. Consider this: Suppose the last node
;; on the shortest path to v is u. Then the rest of the shortest path to v
;; is the same as the shortest path to u. So to track shortest paths, we
;; just need to know the path predecessor nodes—the parents in a search tree.


; An SSSPResult is (make-sssp [Vector-of [Or Nat Bool]] [Vector-of Weight])
(define-struct sssp (preds weights))
; Interpretation: sssp-preds gives the predecessor on the shortest path to
; each node, and sssp-weights gives the weight of the path. For unreachable
; nodes sssp-preds will be false, and for the start node true.


;; The shortest path to any node v must pass through one of its graph
;; predecessor nodes u, and its distance will be the sum of shortest path
;; length to u and the edge weight from u to v. So to find the shortest
;; path, provided we know the shortest paths to all possible u nodes, we
;; just have to inspect each possibility and minimize. One way to think of
;; this is that we gain knowledge as we search the graph, and in particular
;; when we arrive at a node v from a node u, we know that its path is no
;; worse than the path to u plus the edge to v. So we can check if we’ve
;; found a better path, and update our knowledge if we haven’t.
;;
;; This update is called relaxation.

; relax : SSSPResult Nat Nat Weight -> Void
; Updates the distance to `v` given the best distance to `u` found so far
; and the weight `weight` of the edge between `u` and `v`.
(define (relax result u v weight)
  (define old-weight (vector-ref (sssp-weights result) v))
  (define new-weight (weight+ weight (vector-ref (sssp-weights result) u)))
  (when (weight< new-weight old-weight)
    (vector-set! (sssp-weights result) v new-weight)
    (vector-set! (sssp-preds result) v u)))

;; One way we can compute SSSP is to relax all the edges enough times that
;; all information propagates. How many times at most do we have to relax
;; the edges of a graph?

(define (bellman-ford a-graph start-node)
  (define result (make-sssp (make-vector (wgraph-nodes a-graph) #false)
                            (make-vector (wgraph-nodes a-graph) 'inf)))
  (vector-set! (sssp-preds result) start-node #true)
  (vector-set! (sssp-weights result) start-node 0)
  (for ([_ (in-range (wgraph-nodes a-graph))])
    (for ([u (in-range (wgraph-nodes a-graph))])
      (for ([v (in-range (wgraph-nodes a-graph))])
        (relax result u v ((wgraph-weights a-graph) u v)))))
  result)

(check-expect (bellman-ford A-GRAPH 1)
              (make-sssp (vector #f #t 1 1 3 6 3)
                         (vector 'inf 0 7 9 20 20 11)))
(check-expect (bellman-ford A-GRAPH 3)
              (make-sssp (vector #f 3 3 #t 3 6 3)
                         (vector 'inf 9 10 0 11 11 2)))

;; However, if we are clever (and if there are no negative edges, which will
;; mess this up), we can do it faster. In particular, we can relax each edge
;; only once, provided we do it in the right order. What’s the right order?

;; (We want to see the vertices in order from closest to the start to farthest.
;; That way, every time we look at an edge, we know that the source of the edge
;; already has its shortest path found.)

(define (dijkstra a-graph start-node)
  (define visited (make-vector (wgraph-nodes a-graph) #false))
  (define result (make-sssp (make-vector (wgraph-nodes a-graph) #false)
                            (make-vector (wgraph-nodes a-graph) 'inf)))
  (vector-set! (sssp-preds result) start-node #true)
  (vector-set! (sssp-weights result) start-node 0)

  (define (find-smallest-unvisited)
    (define best #false)
    (for ([v (in-range (wgraph-nodes a-graph))])
      (when (and (not (vector-ref visited v))
                 (or (false? best)
                     (weight< (vector-ref (sssp-weights result) v)
                              (vector-ref (sssp-weights result) best))))
        (set! best v)))
    best)

  (let loop ()
    (define u (find-smallest-unvisited))
    (when (and (not (false? u))
               (not (symbol? (vector-ref (sssp-weights result) u))))
      (for ([v (in-range (wgraph-nodes a-graph))])
        (relax result u v ((wgraph-weights a-graph) u v)))
      (vector-set! visited u #true)
      (loop)))

  result)

(check-expect (dijkstra A-GRAPH 1)
              (make-sssp (vector #f #t 1 1 3 6 3)
                         (vector 'inf 0 7 9 20 20 11)))
(check-expect (dijkstra A-GRAPH 3)
              (make-sssp (vector #f 3 3 #t 3 6 3)
                         (vector 'inf 9 10 0 11 11 2)))

;; What’s the time complexity of our implementation of Dijkstra’s algorithm?
;; How might we improve it?