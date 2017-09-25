;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname lec02b) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(require 2htdp/image)
(require 2htdp/universe)

#|
In these notes, single-semicolon (;) comments are comments that should be
written in code that you write as well. They are a necessary part of the
design of the program. Comments written with two or more semicolons, or
in the form of this comment, are meta-comments explaining what the notes
are about, and would not be included in an actual program that you would
write.
|#

#|
1. Data def'n
2. S/P/H
3. Examples
4. Strategy
5. Coding
6. Tests
|#

#|
Choices of strategy:

  Structural decomposition:

    Using a template to inspect a class of data.

  Interval decomposition:

    Discriminating a numeric value based on intervals.

  Decision tree:

    Choosing between alternatives based on a sequence of questions.
    (Note that functional/structural decomposition are specific kinds
    of decision trees.

  Function composition:

    Combining other functions to produce some result.

  Domain knowledge:

    Translating non-programming knowledge into code.

Note that interval and structural decomposition are specific kinds
of decision trees. And note that domain knowledge may take the
form of function composition or a decision tree. However, we should
write down the strategy that actually informs our design, which is
usually the most specific strategy that applies.
|#

#|
Making a template for an enumeration or itemization:

  I. Create a cond clause for each alternative in the D.D.
  II. Add a question to each clause to distinguish it.

During coding, multiple clauses with the same answer may be collapsed.
|#

;;
;; Part 1: A circle game
;;

; Constants
(define SIZE        400)    ; height and width of scene
(define SHRINK-RATE 3)      ; radius decrease at each time step
(define FONT-SIZE 20)       ; size of text
(define TEXT-COLOR "black")      ; the color for text
(define SAFE-COLOR "green")      ; large circle color
(define MARGINAL-COLOR "yellow") ; medium circle color
(define DANGER-COLOR "red")      ; small circle color

; Derived constants
(define CCX         (/ SIZE 2))      ; x coord
(define CCY         (/ SIZE 2))      ; y coord
(define R0          (* 3/8 SIZE))    ; initial radius
(define MARGINAL    (* 2/8 SIZE))    ; marginal radius
(define DANGER      (* 1/8 SIZE))    ; danger radius
(define GROW-RATE   (* 4 SHRINK-RATE)) ; radius increase at each click
(define TEXT-INSET (/ FONT-SIZE 2))  ; how far from upper-left to put score

(define LOSE-MSG    "You lose!")     ; message displayed upon loss
(define EMPTY       (empty-scene SIZE SIZE))

; A Radius is a Number

; circ-step : Radius -> Radius
; Decreases the radius of the circle

(check-expect (circ-step (+ SHRINK-RATE 50))  50)
(check-expect (circ-step 1)                   0)
(check-expect (circ-step 0)                   0)

; Strategy: function composition
(define (circ-step r)
  (max 0 (- r SHRINK-RATE)))

; circ-mouse : Radius Number Number MouseEvt -> Radius
; Grows the circle if clicked inside
;
; Examples:
;  - Given old=50, click at (0, 0), result is 62
;  - Given old=50, click at (60, 0), result is 50
;
; Strategy: struct. decomp.
(define (circ-mouse old x y me)
  (cond
    [(string=? me "button-up")  (circ-click old x y)]
    [else                       old]))

(check-expect (circ-mouse 50 CCX CCY        "button-up") (+ 50 GROW-RATE))
(check-expect (circ-mouse 50 (+ 50 CCX) CCY "button-up") 50)
(check-expect (circ-mouse 50 CCX CCY        "drag")      50)


; circ-click : Radius Number Number -> Radius
; Grows the circle if the given position is inside it
;
; Examples:
;  - If old is 50 and click is in the center, result is 62 (GROW-RATE is 12)
;  - If old is 50 and click is 51 from the center, result is 50
;
; Strategy: decision tree
(define (circ-click old x y)
  (cond
    [(in-circ? x y old CCX CCY)  (+ old GROW-RATE)]
    [else                        old]))

(check-expect (circ-click 50 CCX CCY)        (+ 50 GROW-RATE))
(check-expect (circ-click 50 (+ 50 CCX) CCY) 50)

; in-circ? : Number Number Number Number Number -> Boolean
; Is the given point (x,y) within a circle of radius r
; centered at (cx,cy)?
;
; Examples:
;  - If center is (0, 0), click at (3, 4), and r = 3, false
;  - If center is (0, 0), click at (3, 4), and r = 6, true
;
; Strategy: domain knowledge (geometry)
(define (in-circ? x y r cx cy)
  (> r (dist (- cx x) (- cy y))))

(check-expect (in-circ? 0 0 5 0 0) true)
(check-expect (in-circ? 0 0 5 0 4) true)
(check-expect (in-circ? 0 0 5 0 5) false)
(check-expect (in-circ? 0 0 5 3 4) false)
(check-expect (in-circ? 0 1 5 3 4) true)

; dist: Number Number -> Number
; To compute the distance from (0, 0) to (x, y)
;
; Examples:
;  - distance to (3, 4) is 5
;  - distance to (5, 12) is 13
;
; Strategy: domain knowledge (geometry)
(define (dist x y)
  (sqrt (+ (sqr x) (sqr y))))

(check-expect (dist 0 0)  0)
(check-expect (dist 3 4)  5)
(check-expect (dist 9 12) 15)
(check-expect (dist -3 4) 5)

; circ-draw : Radius -> Scene
; To render a circle of the given radius
;
; Examples:
;  - If r is 10, a circle of radius 10
;  - If r is 0, the message "You lose!"
;
; Strategy: interval decomp.
(define (circ-draw r)
  (cond
    [(> r 0)     (place-image
                  (circle r "solid" (rs->color r))
                  CCX CCY EMPTY)]
    [else        (place-image
                  (text LOSE-MSG FONT-SIZE (rs->color r))
                  CCX CCY EMPTY)]))

(check-expect (circ-draw DANGER)
              (place-image (circle DANGER "solid" "red") CCX CCY EMPTY))
(check-expect (circ-draw 0)
              (place-image (text LOSE-MSG FONT-SIZE "black") CCX CCY EMPTY))

; rs->color : Radius -> Color
; Chooses a color to indicate the danger level of the given radius
;
; Examples:
;  - If r is MARGINAL + 1, green
;  - If r is MARGINAL, yellow
;  - If r is 2, red
;
; Strategy: interval decomp.
(define (rs->color r)
  (cond
    [(> r MARGINAL)  "green"]
    [(> r DANGER)    "yellow"]
    [(> r 0)         "red"]
    [else            "black"]))

(check-expect (rs->color (+ MARGINAL 1))  "green")
(check-expect (rs->color MARGINAL)        "yellow")
(check-expect (rs->color (+ DANGER 1))    "yellow")
(check-expect (rs->color DANGER)          "red")
(check-expect (rs->color 1)               "red")
(check-expect (rs->color 0)               "black")

#;
(big-bang R0
 (on-draw circ-draw)
 (on-tick circ-step)
 (on-mouse circ-mouse))

;;
;; Part 2: adding structure
;;
;; Suppose that in addition to the radius of the circle, we want
;; to keep track of the number of times it has grown, as a kind of
;; score. This means we need to keep track of two values:
;;
;;  - the radius, and
;;  - the score.
;;
;; But we only get one world value. How can we do it? By defining a
;; new class of data that represents *composite* values, that is, values
;; composed of multiple other values that can be projected out.


;; [this is a structure data definition:]

; A CGWorld is (make-cgw Radius Number)
(define-struct cgw (radius score))
; interp. `radius` is the radius of the circle and `score`
; is the count of successful clicks

#|
The previous data definition creates four functions:

 - A *constructor*, make-cgw : Radius Number -> CGWorld
 - A *predicate*, cgw? : Any -> Boolean
 - Two *selectors*:
    - cgw-radius : CGWorld -> Radius
    - cgw-score : CGWorld -> Score

To do structural decomposition of a structure, we take an *inventory*
of available *selector expressions*:
|#

;Template for CGWorld:
#;
(define (process world ...)
  ... (cgw-radius world) ...
  ... (cgw-score world) ...)

(define CG-WORLD0 (make-cgw R0 0))

; cg-stop? : CGWorld -> Boolean
; Determines when the game is over.
;
; Examples:
;  - When radius is 0, true.
;  - When radius is 1, false.
;
; Strategy: function composition
(define (cg-stop? world)
  (zero? (cgw-radius world)))

(check-expect (cg-stop? (make-cgw 0 10)) true)
(check-expect (cg-stop? (make-cgw 1 10)) false)
(check-expect (cg-stop? (make-cgw 5 10)) false)

; cg-draw : CGWorld -> Scene
; Renders the circle and score of the world.
;
; Examples:
;  - (make-world 50 600) renders a circle of radius 50 and a
;    score of 600.
;  - (make-world 0 600) renders the message "You lose!" and a
;    score of 600.
;
; Strategy: struct. decomp.
(define (cg-draw world)
  (overlay/xy
   (text (number->string (cgw-score world)) FONT-SIZE TEXT-COLOR)
   (- TEXT-INSET) (- TEXT-INSET)
   (place-image (cg-draw-radius (cgw-radius world))
                CCX CCY EMPTY)))

(check-expect (cg-draw (make-cgw (+ 1 MARGINAL) 17))
              (overlay/xy
               (text "17" FONT-SIZE TEXT-COLOR)
               (- TEXT-INSET) (- TEXT-INSET)
               (place-image (circle (+ 1 MARGINAL) "solid" SAFE-COLOR)
                            CCX CCY EMPTY)))
(check-expect (cg-draw (make-cgw 0 17))
              (overlay/xy
               (text "17" FONT-SIZE TEXT-COLOR)
               (- TEXT-INSET) (- TEXT-INSET)
               (place-image (text LOSE-MSG FONT-SIZE TEXT-COLOR)
                            CCX CCY EMPTY)))

; cg-draw-radius : Radius -> Image
; Renders the circle or loss message based on the radius.
;
; Examples:
;  - if radius = 0, the "You lose!" message
;  - if radius > MARGINAL, a big green circle
;  - if radius < DANGER, a smallish red circle
;
; Strategy: interval decomp.
(define (cg-draw-radius radius)
  (cond
    [(zero? radius) (text LOSE-MSG FONT-SIZE (rs->color radius))]
    [else           (circle radius "solid" (rs->color radius))]))

(check-expect (cg-draw-radius 0)
              (text LOSE-MSG FONT-SIZE "black"))
(check-expect (cg-draw-radius (+ 1 MARGINAL))
              (circle (+ 1 MARGINAL) "solid" "green"))
(check-expect (cg-draw-radius (+ 1 DANGER))
              (circle (+ 1 DANGER) "solid" "yellow"))
(check-expect (cg-draw-radius 1)
              (circle 1 "solid" "red"))

; cg-tick : CGWorld -> CGWorld
; Shrinks the circle for each tick.
;
; Examples:
;  - (cg-tick (make-cgw 60 325)) => (make-cgw (- 60 SHRINK-RATE) 325)
;  - (cg-tick (make-cgw 0 325)) => (make-cgw 0 325)
;
; Strategy: struct. decomp.
(define (cg-tick world)
  (make-cgw (max 0 (- (cgw-radius world) SHRINK-RATE))
            (cgw-score world)))

(check-expect (cg-tick (make-cgw (+ 60 SHRINK-RATE) 325))
              (make-cgw 60 325))
(check-expect (cg-tick (make-cgw 1 325))
              (make-cgw 0 325))                       
(check-expect (cg-tick (make-cgw 0 325))
              (make-cgw 0 325))                       

; cg-mouse : CGWorld Number Number MouseEvt -> CGWorld
; Handles mouse events, responding to clicks in the circle.
;
; Examples:
;  - "move" event inside the circle => no change
;  - "button-up" event outside the circle => no change
;  - "button-up event inside the circle => radius increases by GROW-RATE
;    and score increases by 1
;
; Strategy: struct. decomp. (MouseEvt)
(define (cg-mouse world x y evt)
  (cond
    [(mouse=? "button-up" evt) (cg-mouse-click world x y)]
    [else world]))

(check-expect (cg-mouse (make-cgw 100 17) CCX CCY "move")
              (make-cgw 100 17))
(check-expect (cg-mouse (make-cgw 100 17) CCX (+ 150 CCY) "button-up")
              (make-cgw 100 17))
(check-expect (cg-mouse (make-cgw 100 17) CCX CCY "button-up")
              (make-cgw (+ 100 GROW-RATE) 18))
              

; cg-mouse-click : CGWorld Number Number -> CGWorld
; Updates the world based on a mouse click.
;
; Examples:
;  - inside circle => circle grows
(check-expect (cg-mouse-click (make-cgw 50 300) (+ 40 CCX) CCY)
              (make-cgw (+ GROW-RATE 50) 301))
;  - outside circle => no change
(check-expect (cg-mouse-click (make-cgw 50 300) (+ 60 CCX) CCY)
              (make-cgw 50 300))
;
; Strategy: decision tree
(define (cg-mouse-click world x y)
  (cond
    [(in-circ? x y (cgw-radius world) CCX CCY)
     (cg-mouse-click-inside world)]
    [else
     world]))

; cg-mouse-click-inside : CGWorld -> CGWorld
; Updates the world to respond to a click inside the circle.
;
; Examples:
(check-expect (cg-mouse-click-inside (make-cgw 46 9))
              (make-cgw (+ GROW-RATE 46) 10))
(check-expect (cg-mouse-click-inside (make-cgw 5 345))
              (make-cgw (+ GROW-RATE 5) 346))
;
; Strategy: struct. decomp.
(define (cg-mouse-click-inside world)
  (make-cgw (+ GROW-RATE (cgw-radius world))
            (+ 1 (cgw-score world))))

#;
(big-bang
 CG-WORLD0
 (on-mouse  cg-mouse)
 (on-tick   cg-tick)
 (on-draw   cg-draw)
 (stop-when cg-stop?))



#|
Making a template (for struct. decomp. strategy):
  I) Do you have an enumeration? If so, cond with right number of clauses
  II) Add COND questions for each possibility
  III) Do you have a structure? If so, add inventory of selector expressions
|#

;; (synonym)
; A World is a Number

;; (interval)
; A CelsiusTemperature is a Number in [-273, infinity)

;; (enumeration)
; A VertebrateClass is one of:
; - "Mammal"
; - "Reptile"
; - "Fish"
; - "Amphibian"
; - "Bird"

#;
(define (process-vertebate a-vertebrate)
  (cond
    [(string=? a-vertebrate "Mammal")     ...]
    [(string=? a-vertebrate "Reptile")    ...]
    [(string=? a-vertebrate "Fish")       ...]
    [(string=? a-vertebrate "Amphibian")  ...]
    [(string=? a-vertebrate "Bird")       ...]))


;; A build-in class of data:
;; ; A Posn is (make-posn Number Number)
;; ; interp. the point (x, y) in R^2
;; (define-struct posn (x y))

(define a-posn (make-posn 4 5))
(check-expect (posn-x a-posn) 4)
(check-expect (posn-y a-posn) 5)
(check-expect (posn? a-posn) true)

; (posn-x (make-posn A B)) ==> A
; (posn-y (make-posn A B)) ==> B

;; What about (x, y, z)

(define-struct 3posn (x y z))
; A ThreePosn is (make-3posn Number Number Number)
; interp. the point (x, y, z) in R^3 (3 space)

;; (constructor) make-3posn : Number Number Number -> ThreePosn
;; (selector)    3posn-x : ThreePosn -> Number
;; (selector)    3posn-y : _ -> _
;; (selector)    3posn-z : _ -> _
;; (predicate)   3posn? : Any -> Boolean

;; Every animal has a name, color, Linnaean class, and weight

(define-struct animal-rec (name color class weight))
;; [DrRacket creates functions:
;;   constructor: make-animal-rec
;;   selectors: animal-rec-name, animal-rec-{color,class,weight}
;;   predicate: animal-rec?]

;; An AnimalRec is (make-animal-rec String Color Vertebrate Number)
;; interp. name is the animal's name, color its color, class is which
;; vertebrate class it's in, and number is its weight in kg

(define bessie (make-animal-rec "Bessie the Cow" "brown" "Mammal" 400))
(define bob (make-animal-rec "Bob the Iguana" "green" "Reptile" 0.5))

#|
(define (animal-rec-template an-animal)
  ... (animal-rec-name an-animal) ...
  ... (animal-rec-color an-animal) ...
  ... (animal-rec-class an-animal) ...
  ... (animal-rec-weight an-animal) ...)
|#

; feed-animal : AnimalRec Number -> AnimalRec
; To increase weight of an animal by the weight of its meal.
;
; Examples:
;  - If we feed Bessie (weight 400) to Bob (weight 0.5), then we get
;    an animal identical to Bob but with weight 400.5.
;
; Strategy: struct. decomp.
(define (feed-animal an-animal meal)
  (make-animal-rec
   (animal-rec-name an-animal)
   (animal-rec-color an-animal)
   (animal-rec-class an-animal)
   (+ meal (animal-rec-weight an-animal))))

(check-expect (feed-animal bob (animal-rec-weight bessie))
              (make-animal-rec "Bob the Iguana" "green" "Reptile" 400.5))

; render-animal : AnimalRec -> Image
; Visualizes an animal.
;
; Examples:
;  - Rendering Bob the Iguana (weight 0.5 kg) results in a green
;    circle of radius proportional to (sqrt 0.5), with black text
;    "Bob the Iguana" superimposed.
;
; Strategy: struct. decomp.
(define (render-animal an-animal)
  (overlay
   (text (animal-rec-name an-animal) 12 "black")
   (circle (* 10 (sqrt (animal-rec-weight an-animal))) "solid"
           (animal-rec-color an-animal))))

(check-expect (render-animal bob)
              (overlay
               (text "Bob the Iguana" 12 "black")        
               (circle (* 10 (sqrt 0.5)) "solid" "green")))

