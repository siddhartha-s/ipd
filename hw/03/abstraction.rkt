;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname abstraction) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(require 2htdp/image)

;; Do not write any recursive functions for this homework.

;; Use map to write this function:
;; build-straight-line : num (listof num) -> (listof posn)
;; returns a list of posns where the X coordinate is n and 
;; the Y coordinate is a number
;; in lon
;; e.g., (build-straight-line 2 (list 1 2 3)) 
;;       (list (make-posn 2 1) (make-posn 2 2) (make-posn 2 3))
;; (define (build-straight-line n lon) ...)

;; Use filter to write this function:
;; pts-north : posn (listof posn) -> (listof posn)
;; returns the posns from lop that are north of p,
;; that is, whose y coordinate is greater than p's y coordinate
;; (define (pts-north p lop) ...)

;; Use foldr to write this function:
;; total-width : (listof image) -> num
;; returns the sum of the widths of all images in loi
;; (define (total-width loi) ...)

;; Use map filter and foldr to write the next four functions.

;; The next exercises use functions to represent curves in the plane. A
;; curve can be represented as a function that accepts an x coordinate
;; and returns a y coordinate. For example, the straight line through the
;; origin can be represented as:
(define (diagonal x) x)
;; and a parabola sitting on the origin can be represented as 
(define (parabola x) (* x x))

(define points (list (make-posn 1 0) (make-posn 1 1) (make-posn 2 2)))

;; points-on-line : (num -> num) (listof posn) -> (listof posn)
;; return the points in pts that are on the curve described by f
;; e.g., (points-on-line diagonal points)
;;       (list (make-posn 1 1) (make-posn 2 2))
;; and   (points-on-line parabola points)
;;       (list (make-posn 1 1))
;; (define (points-on-line f pts) ...)

;; positions: (num -> num) (listof num) -> (listof posn)
;; returns a list of positions on the curve `f' whose x-coordinates
;; are in lon
;; e.g., (positions parabola (list 1 2 3)) 
;;       (list (make-posn 1 1) (make-posn 2 4) (make-posn 3 9))
;; (define (positions f lon) ...)

;; flatten-posns : (listof posn) -> (listof num)
;; constructs the list consisting of all the X and Y coordinates of each
;; of the posns in lop, in order.
;; e.g., (flatten-posns points)
;;       (list 1 0 1 1 2 2)
;; (define (flatten-posns lop) ...)

;; possible-y-coords : (listof (num -> num)) num -> (listof num)
;; given a list of lines lof and an x-coordinate, returns the list
;; of what y-coordinate is associated with that x-coordinate in each curve
;; e.g., (possible-y-coords (list diagonal parabola) 7)
;;       (list 7 49)
;; (define (possible-y-coords lof x) ...)
