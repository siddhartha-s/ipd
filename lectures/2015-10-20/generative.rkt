;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname generative) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
#|
  Problems:

    - bisection method

    - merge sort

    - Graph problems/algorithms:
        - BFS
        - DFS
        - SSSP (Dykstra's algorithm?)
        - Some MSP algorithm?
|#

#|
In numerical analysis, the bisection method finds a zero of a function by
performing binary search on some span of it in which a zero is guaranteed
to exist. If we have a continuous function f that is negative at some point x0
and positive at some point x1, then there is a zero somewhere in between.

Without loss of generality, let x0 < x1, f(x0) < 0, and f(x1) > 0. Then the
let the halfway point p = (x0 + x1)/2. Then one of:

  - p = 0, so p is the zero
  - p < 0, so there's a zero in interval (p, x1)
  - p > 0, so there's a zero in interval (x0, p)

That is, given an interval with a zero, we can transform it into a smaller
problem--an interval half the size. Thus we can make the interval arbitrarily
small via generative recursion.
|#

;; [Number -> Number] Number Number PositiveNumber -> Number
;; Approximates the zero of a function by finding a result x such that
;; |f(x)| ≤ delta.
;;
;; PRECONDITION: f(x0) < 0 and f(x1) > 0.
;;
;; Examples:
;;
;;  - if f(x) = x, interval (-1, 1), any delta, result should be 0
;;  - if f(x) = x, interval (-1, 3), any delta, result should be 0
;;  - if f(x) = x, interval (-1, 2), delta = 0.2, result should be 0.125
;;  - if f(x) = x, interval (-1, 2), delta = 0.1, result should be -0.0625
;;  - if f(x) = 2x, interval (-1, 2), delta = 0.2, result should be -0.0625
;;
;; Strategy: generative recursion
;; Termination argument: Assuming f is not pathological (?), as p approaches
;; the actual zero, f(p) approaches 0. Each subproblem moves one bound or the
;; other closer to the actual zero, which means that the bounds close in on it,
;; which means that p closes in on it, which means that f(p) approaches 0,
;; which means it will be less than delta at some point.
(define (find-zero/bisection f x0 x1 delta)
  (local
   [(define p (/ (+ x0 x1) 2))
    (define fp (f p))]
   (cond
     [(<= (- delta) fp delta) p]
     [(< fp 0)
      (find-zero/bisection f p x1 delta)]
     [else ;; (< 0 fp)
      (find-zero/bisection f x0 p delta)])))

(check-expect (find-zero/bisection (lambda (x) x) -1 1 0.01) 0)
(check-expect (find-zero/bisection (lambda (x) x) -1 3 0.01) 0)
(check-expect (find-zero/bisection (lambda (x) x) -1 2 0.2) 0.125)
(check-expect (find-zero/bisection (lambda (x) x) -1 2 0.1) -0.0625)
(check-expect (find-zero/bisection (lambda (x) (* 2 x)) -1 2 0.2) -0.0625)

;; calculate (sqrt 7)
;; x = (sqrt 7)
;; x^2 = 7
;; x^2 - 7 = 0
(check-within (find-zero/bisection (lambda (x) (- (sqr x) 7)) #i0 #i7 #i0.0001)
              (sqrt 7) 0.0001)

