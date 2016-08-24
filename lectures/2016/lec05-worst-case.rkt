#lang racket
(require pict pict/tree-layout slideshow)

(define (mk n)
  (define-values (layout size)
    (mk-layout n))
  (vc-append (binary-tidier layout)
             (text (format "~a node~a"
                           size
                           (if (= size 1) "" "s")))))

(define (mk-layout h)
  (define n 0)
  (values (let loop ([h h])
            (set! n (+ n 1))
            (cond
              [(= h 0)
               (tree-layout #:pict (circle-it 0) #f #f)]
              [(= h 1)
               (set! n (+ n 1))
               (tree-layout #:pict (circle-it 1)
                            (tree-layout #:pict (circle-it 0) #f #f) #f)]
              [else
               (tree-layout
                #:pict (circle-it h)
                (loop (- h 1))
                (loop (- h 2)))]))
          n))


(define (circle-it h)
  (define pad 10)
  (define p (text (~a h)))
  (define sp (text "99"))
  (cc-superimpose
   (filled-ellipse (+ (pict-width sp) pad)
                   (+ (pict-height sp) pad)
                   #:color "white")
   (ellipse (+ (pict-width sp) pad)
            (+ (pict-height sp) pad)
            #:border-color "black"
            #:border-width 2)
   p))


(for ([x (in-range 10)])
  (slide
   (scale-to-fit (mk x)
                 client-w
                 client-h)))
