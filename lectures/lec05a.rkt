;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname lec05a) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
#|
mutually referential data definitions
-------------------------------------

Lets consider family trees:

+-----------+  +--------------+
|Carl (1926)|  |Bettina (1926)|
|Eyes: Green|  |Eyes: Green   |
+-----------+  +--------------+
    |               |
    +-------+-------+
            |
     +----------------+--------------+--------------+
     |                |              |              |
+------------+  +-----------+  +-----------+  +-----------+
|Adam (1950) |  |Dave (1955)|  |Eva  (1965)|  |Fred (1966)|
|Eyes: Yellow|  |Eyes: Black|  |Eyes: Blue |  |Eyes: Pink |
+------------+  +-----------+  +-----------+  +-----------+
                                     |              |
                                     +------+-------+
                                            |
                                    +---------------+
                                    | Gustav (1988) |
                                    | Eyes: Brown   |
                                    +---------------+

Thinking about these as parents who have children that themselves have
children, etc.

What is the data definition we get for these trees?
|#

; a family-tree is:
;   (make-parent list-of-family-tree string number string)
(define-struct parent (children name date eyes))

; a list-of-family-tree is either:
;  - empty
;  - (cons family-tree list-of-family-tree)

#|
Note that these are mutually recursive data definitions!

What do you think will happen to the functions? ...?

Lets write out the example above as a descendant family tree.
Starting from the bottom:
|#

(define gustav (make-parent '() "gustav" 1988 "brown"))
(define adam (make-parent '() "adam" 1950 "yellow"))
(define dave (make-parent '() "dave" 1965 "black"))
(define eva (make-parent (list gustav) "eva" 1965 "blue"))
(define fred (make-parent (list gustav) 'fred 1966 "pink"))
(define carl (make-parent (list adam dave eva fred)
                          "carl"
                          1926
			  "green"))
(define bettina (make-parent (list adam dave eva fred)
               	             "bettina"
                	     1926
			     "green"))

;; Here is the template for these functions:

#;
(define (ft-template ft)
  (lft-template (parent-children ft))
  (parent-name ft)
  (parent-date ft)
  (parent-eyes ft))

#;
(define (lft-template lft)
  (cond
   [(empty? lft) ..]
   [else 
    ... (ft-template (first lft))
    ... (lft-template (rest lft))]))


;; write a function that determines if there is a blue-eyed descendant:

;; b-e-d? : family-tree -> boolean
;; to determine if there is a blue-eyed descendent in a family tree

;; examples (as tests):
(check-expect (b-e-d? gustav) #false)
(check-expect (b-e-d? carl) #true)

(define (b-e-d? ft)
  (or (equal? "blue" (parent-eyes ft))
      (b-e-d-l? (parent-children ft))))

;; b-e-d-l? : family-tree-list -> boolean
;; to determine if there is a blue-eyed descendent in a list of family trees

;; examples (as tests)
(check-expect (b-e-d-l? '()) #false)
(check-expect (b-e-d-l? (list eva)) #true)

(define (b-e-d-l? lft)
  (cond
   [(empty? lft) #false]
   [else 
    (or (b-e-d? (first lft))
        (b-e-d-l? (rest lft)))]))


#|
Lets try some hand-evaluation:
|#

(b-e-d? carl)
"="
(b-e-d-l? (list adam dave eva fred))
"="
(or (b-e-d? adam) (b-e-d-l? (list dave eva fred)))
"="
(or (b-e-d-l? '()) (b-e-d-l? (list dave eva fred)))
"="
(b-e-d-l? (list dave eva fred))
"= ..."
#true

#|
============================================================

How about a function that computes the average age of
the people in a family tree?

Try to do this on your own for practice. HINT: you can
re-use the `average` function from your homework

|#
