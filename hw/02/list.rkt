;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname list) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
#|

Design the following functions:


;; squares : [Listof Number] -> [Listof Number]
;; compute the perfect squares of numbers in `l`
(define (squares l) ...)

;; contains-telephone : [listof String] -> Boolean
;; returns #true if 'l' contains the string "telephone", #false otherwise
(define (contains-telephone l) ...)


;; shortest : [NE-list-of String] -> String
;; returns the shortest string in 'l' -- read this for help:
;; http://www.ccs.neu.edu/home/matthias/HtDP2e/part_two.html#%28part._sec~3alists~3ane%29
;; also look up the 'string-length' function in the documentation (type f1 to open the docs)
(define (shortest l) ...)

;; mean : [Listof Number] -> Number
;; computes the average of the elements of 'l'
;; return 0 if 'l' list is empty
(define (mean l) ...)


|#
