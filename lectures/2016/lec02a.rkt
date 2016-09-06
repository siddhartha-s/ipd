;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname lec02a) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;; ENUMERATIONS

#|
New design recipe:
1. *** Data definition
2. Signature, purpose, header
3. Examples
4. *** Strategy
5. Coding
6. Tests
|#

;; Kinds of Data Definitions

;; [synonym]
; A Temperature is a Number
; interp. temperature in Kelvin

;; [enumeration]
; A Mode is one of:
; - "solid"
; - "outline"

;; [synonym]
; A Name is a String

;; [interval]
; A GPA1 is a Number in [0, 4]

;; [enumeration]
; A GPA2 is one of:
; - 0
; - 1
; - 2
; - 3
; - 4

;; [interval]
; A Quux is an Integer in [0, 1000]

;; [synonym]
; A WorldState is a Number

;; [enumeration]
; A Color is one of:
; - "red"
; - "green"
; - "pink"
; - "chartreuse"
; ...

;; [enumeration]
; A TrafficLight is one of:
; - "red"
; - "amber"
; - "green"

; process-traffic-light : TrafficLight ... -> ...
; Template for TrafficLight.
#;
(define (process-traffic-light light ...)
  (cond
    [(string=? light "red")    ...]
    [(string=? light "amber")  ...]
    [(string=? light "green")  ...]))

; what-to-do : TrafficLight -> String
; Figures out what to do at an intersection.

(check-expect (what-to-do "red")    "stop")
(check-expect (what-to-do "amber")  "maybe stop")
(check-expect (what-to-do "green")  "go")

; Strategy: structural decomposition
(define (what-to-do light)
  (cond
    [(string=? light "red")    "stop"]
    [(string=? light "amber")  "maybe stop"]
    [(string=? light "green")  "go"]))


; color-of-light-in-french : TrafficLight -> String
; To find the color of a traffic light

(check-expect (color-of-light-in-french "red")    "rouge")
(check-expect (color-of-light-in-french "green")  "vert")
(check-expect (color-of-light-in-french "amber")  "jaune")

; Strategy: struct. decomp.
(define (color-of-light-in-french light)
   (cond
    [(string=? light "red")    "rouge"]
    [(string=? light "amber")  "jaune"]
    [(string=? light "green")  "vert"]))

;; [interval itemization]
; A BMINumber is one of:
; - less than 20
; - in [20, 25)
; - in [25, 32)
; - 32 or more

; process-BMI-number : BMINumber ... -> ...
; Template for BMI Number
#;
(define (process-BMI-number n ...)
  (cond
    [(< n 20) ...]
    [(< n 25) ...]
    [(< n 32) ...]
    [else     ...]))
