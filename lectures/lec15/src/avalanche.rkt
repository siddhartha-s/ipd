#lang racket/gui
(require pict math/base)
(require profile)

(define call-out? #t)

(define hash-binaries
  (for/list ([file (in-directory "/Users/robby/Library/Caches/CLion2016.2/cmake/generated/")]
             #:when (equal? (path->string (last (explode-path file))) "avalanche"))
    file))
(define hash-binary
  (cond
    [(= 1 (length hash-binaries)) (car hash-binaries)]
    [else (error 'hash-binaries "got confused ~s" hash-binaries)]))

;; hash : bytes[8] -> (listof[64] (listof[64] (or/c 0 1)))
;; a thin wrapper over the binary `hash` from the in_out.cpp
(define (run-one-avalanche-round the-bytes)
  (cond
    [call-out? 
     (define sp (open-output-string))
     (parameterize ([current-input-port (open-input-bytes the-bytes)]
                    [current-output-port sp])
       (system* hash-binary))
     (read (open-input-string (get-output-string sp)))]
    [else
     (define hashed (equal-hash-code the-bytes))
     (define copied-bytes (bytes-copy the-bytes))
     (for/list ([byte (in-bytes the-bytes)]
                [i (in-naturals)]
                #:when #t
                [bit (in-range 8)])
       (define val (bytes-ref copied-bytes i))
       (bytes-set! copied-bytes i (bitwise-xor val (expt 2 bit)))
       (define difference (bitwise-xor hashed (equal-hash-code copied-bytes)))
       (bytes-set! copied-bytes i val)
       (for/list ([bit (in-range 64)])
         (if (zero? (bitwise-and (expt 2 bit) difference))
             0
             1)))]))

(define size 64)

(define status (build-vector 64 (λ (_) (make-vector size 0))))
(define (get-s n m) (vector-ref (vector-ref status n) m))
(define (set-s! n m v) (vector-set! (vector-ref status n) m v))
(define (inc-s! n m) (set-s! n m (+ 1 (get-s n m))))

(define trials 0)
(define (avalance-trial)
  (set! trials (+ trials 1))
  (define random-bytes (apply
                        bytes
                        (for/list ([i (in-range 8)])
                          (random 256))))
  (define results (run-one-avalanche-round random-bytes))
  (for ([result-line (in-list results)]
        [i (in-naturals)])
    (for ([result (in-list result-line)]
          [j (in-naturals)])
      (unless (zero? result)
        (inc-s! i j))))
  (set! cached-pict #f))

(define (build-pict)
  (cond
    [(zero? trials) (blank)]
    [else
     (apply
      vl-append
      (for/list ([i (in-range size)])
        (apply
         hc-append
         (for/list ([j (in-range size)])
           (define val (get-s i j))
           (define value (/ val trials))
           (define color
             (cond
               [(<= value 1/2)
                (choose-color-between (* value 2) white forestgreen)]
               [else
                (choose-color-between (* (- value 1/2) 2) forestgreen black)]))
           (colorize (filled-rectangle 10 10 #:draw-border? #f)
                     color)))))]))

(define (choose-color-between % low high)
  (make-object color%
    (choose-between % (send low red) (send high red))
    (choose-between % (send low green) (send high green))
    (choose-between % (send low blue) (send high blue))))

(define (choose-between % l h) (round (+ l (* % (- h l)))))

(define forestgreen (send the-color-database find-color "forestgreen"))
(define firebrick (send the-color-database find-color "firebrick"))
(define white (send the-color-database find-color "white"))
(define black (send the-color-database find-color "black"))

(define last-cw 1)
(define last-ch 1)
(define cached-pict #f)

(define frame (new frame% [width 500] [height 500] [label "Avalanche"]))
(define canvas (new canvas% [parent frame]
                    [paint-callback
                     (λ (c dc)
                       (define-values (cw ch) (send c get-client-size))
                       (unless (and cached-pict
                                    (equal? cw last-cw)
                                    (equal? ch last-ch))
                         (set! cached-pict (time (freeze (scale-to-fit (build-pict) cw ch)))))
                       (draw-pict cached-pict dc
                                  (- (/ cw 2) (/ (pict-width cached-pict) 2))
                                  (- (/ ch 2) (/ (pict-height cached-pict) 2))))]))
(define stop-button
  (new button% [parent frame]
       [label "stop"]
       [callback (λ (_1 _2)
                   (send timer stop)
                   (send stop-button enable #f))]))

(send frame show #t)

(define timer
  (new timer%
       [notify-callback
        (λ ()
          (cond
            [(send frame is-shown?)
             (avalance-trial)
             (send canvas refresh)]
            [else (send timer stop)]))]
       [interval 500]))