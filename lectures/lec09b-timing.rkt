#lang racket/base
(require racket/runtime-path)

#|

This is the same code

|#

;; a Stream is
;;   (make-stream Number (-> Stream))
(define-struct stream (num [rest #:mutable]))

;; take : Nat Stream -> (Listof Numbers)
;; to find the nth element of the stream.
;; structural template on Nat.
(define (take n stream)
  (cond
    [(zero? n) '()]
    [else (cons (stream-num stream)
                (take (- n 1) (get-rest stream)))]))

(define (get-rest stream) ((stream-rest stream)))

;; filter-stream : (Number -> Boolean) Stream -> Stream
(define (filter-stream p stream)
  (cond
    [(p (stream-num stream))
     (make-stream (stream-num stream)
                  (lambda ()
                    (filter-stream p (get-rest stream))))]
    [else (filter-stream p (get-rest stream))]))


;; remove-div : number stream -> stream
(define (remove-div n stream)
  (filter-stream (lambda (x) (not (zero? (modulo x n))))
                 stream))

;; sieve : Stream -> Stream
(define (sieve stream)
  (make-stream (stream-num stream)
               (lambda ()
                 (remove-div (stream-num stream)
                             (sieve (get-rest stream))))))

;; map-stream : (Number -> Number) Stream -> Stream
(define (map-stream f stream)
  (make-stream (f (stream-num stream))
               (lambda () (map-stream f (get-rest stream)))))


(define nats-from-2 (make-stream 2 (lambda () (map-stream add1 nats-from-2))))

(define-runtime-path lec09b-timing-data.rktd "lec09b-timing-data.rktd")
(cond
  [(file-exists? lec09b-timing-data.rktd)
   (define data (call-with-input-file lec09b-timing-data.rktd read))
   (define plot (dynamic-require 'plot 'plot))
   (define points (dynamic-require 'plot 'points))
   (define (subtract-em data)
     (for/list ([i (in-list data)]
                [j (in-list (cdr data))])
       (list (list-ref i 0)
             (- (list-ref j 1) (list-ref i 1)))))
   (define δ-data (subtract-em data))
   (define δδ-data (subtract-em δ-data))
   (values (plot (points data)
                 #:x-label "number taken"
                 #:y-label "milliseconds")
           (plot (points δ-data)
                 #:x-label "number taken"
                 #:y-label "δ milliseconds")
           (plot (points δδ-data)
                 #:x-label "number taken"
                 #:y-label "δδ milliseconds"))]
  [else
   (define initial 1000)
   (define number-of-points 100)
   (define step 100)
   (printf "computing data:\n")
   (define table
     (for/list ([i (in-range number-of-points)])
       (collect-garbage) (collect-garbage)
       (define-values (ans cpu real gc)
         (time-apply take (list initial nats-from-2)))
       (printf "~s -> ~s\n" initial cpu)
       (begin0 (list initial cpu)
               (set! initial (+ initial step)))))
   (call-with-output-file lec09b-timing-data.rktd
     (λ (port) (write table port)))])
