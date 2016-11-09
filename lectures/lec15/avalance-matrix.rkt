#lang racket

(define bits 4)

;; coordinate (i,j) in the matrix should be the number of
;; times the bit in position j flips in the output of `f`
;; when I flip the bit in position in i in the input to `f`
;; (for all possible inputs)
(define (avalanche-matrix f)
  (for/list ([i (in-range bits)])
    (for/list ([j (in-range bits)])
      (for/sum ([n (in-range (expt 2 bits))]
                #:when (< n (flip-nth-bit n i)))
        (define original (f n))
        (define flipped (f (flip-nth-bit n i)))
        (if (zero? (bitwise-and (bitwise-xor original flipped)
                                (expt 2 j)))
            0
            1)))))

(define (flip-nth-bit n i)
  (bitwise-xor n (expt 2 i)))

(define/contract (rotate x)
  (-> (and/c exact-nonnegative-integer? (<=/c 15))
      (and/c exact-nonnegative-integer? (<=/c 15)))
  (define bits (number->string x 2))
  (define bits/len (string-append (build-string (- 4 (string-length bits))
                                                (λ (_) #\0))
                                  bits))
  (string->number
   (string-append (substring bits/len 1 (string-length bits/len))
                  (substring bits/len 0 1))
   2))

(define (rot-and-xor x) (bitwise-xor x (rotate x)))
(avalanche-matrix add1)
(avalanche-matrix rot-and-xor)

(define permutation #(15 10 0 2 9 11 4 8 7 14 3 13 12 5 6 1))
(define (apply-permutation x) (vector-ref permutation x))
(avalanche-matrix apply-permutation)

(define (find-good-permutations)
  (define perfect-avalanche-matrix
    (for/list ([i (in-range bits)])
      (for/list ([i (in-range bits)])
        bits)))
  (let loop ([n 0]
             [found 0])
    (when (and (not (zero? n)) (zero? (modulo n 1000))) (printf "~a\n" (/ found n 1.)))
    (define permutation (list->vector (shuffle (for/list ([i (in-range (expt 2 bits))]) i))))
    (define matrix (avalanche-matrix
                    (λ (n) (vector-ref permutation n))))
    (cond
      [(equal? matrix perfect-avalanche-matrix)
       (loop (+ n 1) (+ found 1))]
      [else (loop (+ n 1) found)])))

(define (permutation->mapping permutation)
  (for ([out (in-vector permutation)]
        [in (in-naturals)])
    (printf "~a -> ~a\n"
            (~r #:base 2 #:min-width 4 #:pad-string "0" in)
            (~r #:base 2 #:min-width 4 #:pad-string "0" out))))

(module+ test
  (require rackunit)
  (check-equal? (rotate 0) 0)
  (check-equal? (rotate 15) 15)
  (check-equal? (rotate 1) 2)
  (check-equal? (rotate 3) 6)
  (check-equal? (rotate 8) 1))