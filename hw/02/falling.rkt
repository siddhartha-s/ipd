;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname falling) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(require 2htdp/image)
(require 2htdp/universe)
#|

For this exercise you will design and implement
a minimalistic, one finger input game. The player
to control a paddle that moves back and forth
at the bottom of the screen. Falling from the heavens
are some items that you're trying to capture on
your paddle. The paddle never stays still; it continuously
moves back and forth along the bottom of the screen.

There is only a single kind of input
accepted (think like a thumb tap on a phone); the
tap reverses the direction of the paddle. That is,
if there is no input, then the paddle moves from
the left edge to the right edge and then back to
the left edge, over and over. When the user taps,
then the paddle reverses direction (even if it is
not at one of the edges). So, if the user wishes to
keep the paddle in one spot, they can tap repeatedly.

The player gets 10 points for each falling item that
the paddle catches and loses one point each time they
tap to reverse direction, but the score should never
go below zero.

Use the following World definition; note that there is
some ambiguity in this definition. For example, are the
posns of the fallers representing the centers or the
upper-left corners? You will need to figure our issues
like this one and make sure your code is consistent.

Either way, you should use the center of the faller to
determine if it has fallen off of the bottom and if
it has hit the paddle.
|#

;; a World is
;;   (make-world number        -- x coordinate of the paddle
;;               direction     -- which way the paddle is moving
;;               list-of-posn  -- faller positions
;;               number)       -- score
(define-struct world (paddle direction fallers score))


;; a list-of-posn is either
;;   - '()
;;   - (cons posn list-of-posn)

;; a direction is either
;;   - "left"
;;   - "right"

#|

For the first step, give your game some flavor.
Find or design an image to show as the falling
items and design an image to use as the paddle.
For the paddle, use 2htdp/image to make an image,
but for the falling you may find an image online
to include in your program (or you may design one
using 2htdp/image).

Make your falling image be about 20 pixels tall
and 20 pixels wide and make you paddle be about 12
pixels tall and 50 pixels wide. Use image-width and
image-height to check the sizes.

Define them with the names `faller` and `paddle`

|#

(define faller-image ...)
(define paddle-image ...)

#|

There are three separate pieces of the game
to be implemented: rendering the world
to an image to be shown on the screen, handling
user inputs, and adjusting the world as time passes.

Here are the sigantures of the three functions:

draw : World -> image

key : World Keypress -> World

tick : World -> World

draw and key are fairly well-described by the
description above; tick needs a little more
explanation. It has several, conceptually different
pieces:

1) move the fallers down the screen by one coordinate
2) if a faller touches the bottom of the screen,
   remove it from the world; if it overlaps with
   the paddle, also increment the score
3) possibly add a new faller to the list (starting
   at the top of the screen)
4) move the paddle

Be sure to design several different functions to
accomplish those tasks and not just one monolithic
function that does it all! (And think carefully about
how you might break up the other two functions, too.)

Don't forget: the coordinate system has the upper-most
y coordinate as 0 and increasing coordinates move
downwards on the screen. The left-most x coordinate
is 0 and increasing coordinates move to the right.

|#


#|
 use these two definitions in the design of your functions
 to know the width and height of the game's screen and the
 y location of the paddle
|#
(define world-width 200)
(define world-height 300)

#|

Use a definition like this one to (randomly) add a faller
to the current list. You may wish to adjust it based on
gameplay factors or the way you interpret posns as fallers.
Note that because of the randomness, this function is not
really testable with check-expect. It needs different
test suite support so we skip some tests.

|#
;; maybe-add-faller : [Listof posn] -> [Listof posn]
(check-expect (maybe-add-faller
               (list (make-posn 0 0)
                     (make-posn 1 1)
                     (make-posn 2 2)
                     (make-posn 3 3)
                     (make-posn 4 4)
                     (make-posn 5 5)
                     (make-posn 6 6)
                     (make-posn 7 7)
                     (make-posn 8 8)
                     (make-posn 9 9)))
              (list (make-posn 0 0)
                    (make-posn 1 1)
                    (make-posn 2 2)
                    (make-posn 3 3)
                    (make-posn 4 4)
                    (make-posn 5 5)
                    (make-posn 6 6)
                    (make-posn 7 7)
                    (make-posn 8 8)
                    (make-posn 9 9)))

(define (maybe-add-faller fallers)
  (cond
    [(< (length fallers) 20)
     (cond
       [(zero? (random 20))
        (cons (make-posn (random world-width) 0)
              fallers)]
       [else fallers])]
    [else fallers]))


#|

To run your game, do this:

|#
#;
(big-bang (make-world (/ world-width 2) "right" '() 0)
          [on-tick tick 1/200]
          [on-key key]
          [to-draw draw])
