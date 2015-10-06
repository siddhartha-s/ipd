#lang racket

(define-syntax (bench stx)
  (syntax-case stx ()
    [(bench n body ...)
     #'(time
        (for ([_ n])
          (begin body ...)))]))
