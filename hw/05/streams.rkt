#lang dssl

#|

To use the `dssl` language, use the "Language | Choose Language"
dialog to select "The Racket Language".

You also need to install the language. Go to the "File | Install Package..."
menu item in DrRacket. Type "dssl" in the box and click "Install".

|#

;; a Stream is:
;;   (make-stream number (-> Stream))
(define-struct stream (num rest))

;; take : Nat Stream -> (Listof Numbers)
;; to find the nth element of the stream.
;; structural template on Nat.
(define (take n stream)
  (cond
    [(zero? n) '()]
    [else (cons (stream-num stream)
                (take (- n 1) ((stream-rest stream))))]))

(define ones (make-stream 1 (λ () ones)))

#|

Define the stream of all perfect squares. The first few numbers
are: 0 1 4 9 16 25 36 49 64 81.

|#

(define squares ones) ;; wrong!

(check-expect (take 10 squares)
              (list 0 1 4 9 16 25 36 49 64 81))




#|

Define a stream that has all of the even integers.
This is a good way to organize that stream:

   0 2 -2 4 -4 6 -6 8 -8 10 -10 12 -12

(but it isn't the only way). 

|#


(define evens ones) ;; wrong!

(check-expect
 (take 10 evens)
 (list 0 2 -2 4 -4 6 -6 8 -8 10))

#|

The fibonacci numbers start with 0, then 1, and each
number aftewards is the sum of the previous two
numbers. Here are the first few numbers:

0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377

Using the stream data definition, define a stream
that consists of all of the fibonnaci numbers.

|#

(define fibs ones) ;; wrong!

(check-expect (take 15 fibs)
              (list 0 1 1 2 3 5 8 13 21 34 55 89 144 233 377))

