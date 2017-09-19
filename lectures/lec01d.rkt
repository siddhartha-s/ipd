;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname lec01d) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;; IPD Lecture 1d: Big bang exercise in class

(require 2htdp/image)
(require 2htdp/universe)

#|

The goal of this exercise is to use big-bang
to make a ball whose radius is controlled by
the position of the mouse pointer. You decide how.

Use the design recipe to complete the functions
below. (Which steps are done for you? Which steps are
left to be done?)

|#

;; A WorldState is:
;;   a non-negative number

(define SCENE-SIZE 300)

;; mouse-to-radius : WorldState Number Number MouseEvt -> WorldState
;; determines the radius of the circle based on the mouse's x and y coordinates
(define (mouse-to-radius old-radius x y mouse-evt)
  ;; this is incorrect
  (/ 1 0))

;; draw : WorldState -> Image
(define (draw radius)
  ;; this is also incorrect
  (empty-scene SCENE-SIZE SCENE-SIZE))

;; remove the following line (with the #; on it) to run, run, run.
#;
(big-bang
 1
 (on-draw draw)
 (on-mouse mouse-to-radius))
