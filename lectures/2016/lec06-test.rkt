#lang racket/base
(require racket/list)

(require "lec06-avl.rkt")

(define (run)
  (for ([l (in-permutations '(1 2 3 4 5 6 7 8))])
    (let/ec k
      (for/fold ([t "leaf"]
                 [inserted '()])
                ([e (in-list l)])
        (define next (insert t e))
        (check-them next (cons e inserted) k)
        (values next (cons e inserted))))))

(define (check-them t es k)
  (unless (AVL? t)
    (eprintf "non-AVL after inserting ~s\n"
             (reverse es))
    (k (void)))
  (unless (equal? (sort es <)
                  (in-order t))
    (eprintf "wrong-values after inserting ~s\n"
             (reverse es))
    (k (void))))

(module+ main (time (run)))
