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
    [else (listof-sexp-template s-exp)]))

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
;; - Symbol
;; - (make-add Expr Expr)
;; - (make-sub Expr Expr)
;; - (make-mul Expr Expr)
;; - (make-let (Listof Binding) Expr)
(define-struct add (left right))
(define-struct sub (left right))
(define-struct mul (left right))
(define-struct let (binds expr))

;; A Binding is
;; - (make-binding Symbol Expr)
(define-struct binding (name expr))


#;
(define (expr-template e)
  (cond
    [(number? e) ...]
    [(symbol? e) ...]
    [(add? e) (... (expr-template (add-left e))
                   (expr-template (add-right e)))]
    [(sub? e) (... (expr-template (sub-left e))
                   (expr-template (sub-right e)))]
    [(mul? e) (... (expr-template (mul-left e))
                   (expr-template (mul-right e)))]))

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
(check-expect (parse '(+ (- 13 22) (+ 5 5)))
              (make-add (make-sub 13 22) (make-add 5 5)))
(check-expect (parse `(* 5 6)) (make-mul 5 6))
(check-expect (parse `(* (+ 5 6) (* 0 1)))
              (make-mul (make-add 5 6)
                        (make-mul 0 1)))
(check-expect (parse 'x) 'x)
(check-expect
 (parse '(let ([x 1]
               [y 2])
           (+ x y)))
 (make-let (list (make-binding 'x 1)
                 (make-binding 'y 2))
           (make-add 'x 'y)))
(check-expect
 (parse '(let ([x 1]
               [y (let ([z 1]) (+ z z))])
           (+ x y)))
 (make-let (list (make-binding 'x 1)
                 (make-binding 'y (make-let (list (make-binding 'z 1)) (make-add 'z 'z))))
           (make-add 'x 'y)))


(define (parse sexp)
  (match sexp
    [`(+ ,expr1 ,e2) (make-add (parse expr1) (parse e2))]
    [`(+ ,e1 ,e2 ,e3) (make-add (parse e1)
                                (make-add (parse e2)
                                          (parse e3)))]
    [`(- ,e1 ,e2) (make-sub (parse e1) (parse e2))]
    [`(* ,e1 ,e2) (make-mul (parse e1) (parse e2))]
    [`(let ,bindings ,expr)
     (make-let (parse-bindings bindings) (parse expr))]
    [e
     (cond
       [(number? e) e]
       [(symbol? e) e]
       [else (error "not an expr")])]))

;; parse-bindings : S-Exp -> (Listof Binding)

(check-expect (parse-bindings `([x 1] [y 2]))
              (list (make-binding 'x 1) (make-binding 'y 2)))

(define (parse-bindings sexp)
  (cond
    [(empty? sexp) sexp]
    [else
     (local [(define bind (first sexp))
             (define sym (first bind))
             (define expr (parse (second bind)))]
       (cons (make-binding sym expr) (parse-bindings (rest sexp))))]))

;; eval : Expr -> Number
;; evaluates the given expression as an arithmetic operation

(check-expect (eval (make-add 1 2)) 3)
(check-expect (eval (make-sub 3 4)) -1)
(check-expect (eval expr4) 4)
(check-expect (eval (parse '(+ 7 5 9))) 21)

(check-expect (eval (make-add (make-add 3 4)
                              (make-add 7 5)))
              19)
(check-expect (eval (make-mul (make-add 3 4) (make-sub 7 5)))
              14)

(check-error (eval (make-add 1 'x)) "x: this variable is not defined")

(check-expect
 (eval
  (parse '(let ([x 1]
                [y 2])
            (+ x y))))
 3)
(check-expect
 (eval
  (parse '(let ([x 1]
                [y (let ([z 1]) (+ z z))])
            (+ x y))))
 3)

;; An Env is (Listof Value-Binding)
;; A Value-Binding (make-binding Symbol Number)

;; empty-env : Env
(define empty-env empty)

;; lookup : Env Symbol -> Number
(define test-env (list (make-binding 'x 1) (make-binding 'y 2)))
(check-expect (lookup test-env 'x) 1)
(check-error (lookup test-env 'z) "z: this variable is not defined")

(define (lookup env name)
  (cond
    [(empty? env) (error (string-append (symbol->string name) ": this variable is not defined"))]
    [else
     (cond
       [(symbol=? name (binding-name (first env)))
        (binding-expr (first env))]
       [else (lookup (rest env) name)])]))

;; extend : Env Symbol Number -> Env
(check-expect (extend test-env 'z 3)
              (cons (make-binding 'z 3) test-env))

(define (extend env name val)
  (cons (make-binding name val) env))

;; extend* : env (Listof Symbol) (Listof Number) -> Env
;; Invariant : the lists have the same length
(define (extend* env names vals)
  (cond
    [(empty? names) env]
    [else
     (extend* (extend env (first names) (first vals)) (rest names) (rest vals))]))

(check-expect
 (eval
  (parse
   '(let ([x 1])
      (let ([x 2])
        x))))
 2)
(check-expect
 (eval
  (parse
   '(let ([x 1])
      (+
       (let ([x 2])
         x)
       x))))
 3)
  

;; Strategy: structural decomp
(define (eval e)
  (eval/env e empty-env))

(check-expect
 (eval
  (parse
   '(let ([x 1])
      (let ([y x])
        y))))
 1)

(define (eval/env e env)
  (cond
    [(number? e) e]
    [(symbol? e) (lookup env e)]
    [(add? e) (+ (eval/env (add-left e) env)
                 (eval/env (add-right e) env))]
    [(sub? e) (- (eval/env (sub-left e) env)
                 (eval/env (sub-right e) env))]
    [(mul? e) (* (eval/env (mul-left e) env)
                 (eval/env (mul-right e) env))]
    [(let? e)
     (local [(define values (eval-bindings (let-binds e) env))
             (define names (get-names (let-binds e)))
             (define new-env (extend* env names values))]
       (eval/env (let-expr e) new-env))]))

;; eval-bindings : (Listof Binding) Env -> (Listof Number)
(check-expect (eval-bindings (list (make-binding 'x (make-add 1 1))
                                   (make-binding 'y (make-mul 0 4)))
                             '())
              (list 2 0))
(check-expect (eval-bindings '() '()) '())
(define (eval-bindings bindings env)
  (map (λ (binding) (eval/env (binding-expr binding) env)) bindings))
(define (get-names bindings)
  (map binding-name bindings))












