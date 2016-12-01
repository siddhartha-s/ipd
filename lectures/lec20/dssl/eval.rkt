#lang dssl

;; An S-Expression (S-Exp) is one of
;; - Number
;; - Symbol
;; - (Listof S-Exp)

#;
(define (s-exp-template s-exp)
  (cond
    [(number? s-exp) ...]
    [(symbol? s-exp) ...]
    [(empty? s-exp) ...]
    [else (... (s-exp-template (first s-exp))
               (rest s-exp))]))

;; Examples
(define a-number 5)
(define a-symbol 'x)
(define a-sexp '(+ 1 2))
(define compound-sexp '(+ (- 3 2) (+ 1 2)))
(define compound-qq `(+ (- 3 2) ,a-sexp))

(check-expect '5 5)
(check-expect compound-qq compound-sexp)
(check-expect `(+ ,(+ 3 4) 5) '(+ 7 5))

;; An Expr is one of
;; - Number
;; - (make-add Expr Expr)
;; - (make-sub Expr Expr)
(define-struct add (left right))
(define-struct sub (left right))

#;
(define (expr-template e)
  (cond
    [(number? e) ...]
    [(add? e) (... (expr-template (add-left e))
                   (expr-template (add-right e)))]
    [(sub? e) (... (expr-template (sub-left e))
                   (expr-template (sub-right e)))]))

;; Examples
(define expr1 5)
(define expr2 (make-add 1 2))
(define expr3 (make-sub 3 2))
(define expr4 (make-add expr3 expr2))

;; parse : S-Exp -> Expr
;; parses the s-expression into an equivalent Expr

(check-expect (parse 5) 5)
(check-expect (parse '(+ 1 2)) (make-add 1 2))
(check-expect (parse '(- 3 4)) (make-sub 3 4))
(check-expect (parse compound-sexp) expr4)

(define (parse sexp)
  (match sexp
    [`(+ ,e1 ,e2) (make-add (parse e1) (parse e2))]
    [`(- ,e1 ,e2) (make-sub (parse e1) (parse e2))]
    [e
     (cond
       [(number? e) e]
       [else (error "not an expr")])]))

;; eval : Expr -> Number
;; evaluates the given expression as an arithmetic operation

(check-expect (eval (make-add 1 2)) 3)
(check-expect (eval (make-sub 3 4)) -1)
(check-expect (eval expr4) 4)

(define (eval exp)
  (cond
    [(number? exp) exp]
    [(add? exp) (+ (eval (add-left exp))
                 (eval (add-right exp)))]
    [(sub? exp) (- (eval (sub-left exp))
                 (eval (sub-right exp)))]))
  
   




