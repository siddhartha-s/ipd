;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname lec08b) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
#|
natural numbers
--------------

Up to now, we've just treated numbers as atomic data and
used +, -, etc on them. But, what if we wanted to build a
list whose length was based on a given number? Without some
builtin thing, you couldn't do that.

So, I'm going to show you a weird data definition now:

  a Natural is either:
   - 0
   - (add1 Natural)

This is a different way of looking at numbers -- we think
of them as having structure internal structure.

So, how would we write a template for that:
|#

#;
(define (fun-for-nat n)
  (cond
    [(zero? n) ...]
    [else ... (fun-for-nat (sub1 n)) ...]))


;; So, lets use this to write a function:

;; build-strs : Natural String -> [Listof String]
;; to construct a list of the symbols s 
;; with length `n'.
(check-expect (build-strs 0 "chair") '())
(check-expect (build-strs 3 "doll")
              (list "doll" "doll" "doll"))
(check-expect (length (build-strs 123 "hi")) 123)

(define (build-strs n s)
  (cond
    [(zero? n) empty]
    [else (cons s (build-strs (sub1 n) s))]))

;; Okay, lets go further: say I wanted to vary the elements in
;; the list. So, I want something like this:

;; build-a-list : Natural [Natural -> X] -> [Listof X]
;; build a list of length `n', where each element
;; is given by calling `f' on its index.

;; tests
(check-expect (build-a-list 0 (λ (x) "hi")) '())

(check-expect (build-a-list 3 (λ (x) "doll"))
              (list '"doll" "doll" "doll"))

(check-expect (build-a-list 4 (λ (x) (* x x)))
              (list 0 1 4 9))

(define (build-a-list n f)
  (cond
    [(zero? n) empty]
    [else (cons (f n)
                (build-a-list (sub1 n) f))]))

;; hand evaluation using sq:

;; sq : Number -> Number
(define (sq x) (* x x))

  (build-a-list 3 sq)
= (cons 9 (build-a-list 2 sq))
= (cons 9 (cons 4 (build-a-list 1 sq)))
= (cons 9 (cons 4 (cons 1 (build-a-list 0 sq))))
= (cons 9 (cons 4 (cons 1 empty)))

;; Uh oh. It's backwards! And it doesn't end with 0, but instead 1.

;; Well, what can we do about that?  We could reverse the list
;; afterwards. Or, we could call `f` with different numbers. Here's the
;; latter solution:

;; build-a-list : Natural [Natural -> X] -> [Listof X]
(define (build-a-list-2 f n)
  (build-a-list/orig-length f n n))

(define (build-a-list/orig-length f n start-n)
  (cond
    [(zero? n) empty]
    [else (cons (f (- start-n n))
                (build-a-list/orig-length f (sub1 n) start-n))]))

;; This function is actually built in; it is called build-list.
