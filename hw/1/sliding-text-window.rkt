;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname sliding-text-window) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(require 2htdp/universe)
(require 2htdp/image)
#|

Write a universe program that will, piece by piece,
display the `complete-message` below. At any given
moment, we should see exactly 3 characters of the
message and, as time ticks, the three characters
that are visible should move forward through the
message below. For example, at the first time tick,
we should see

  Wel

and the in the second time tick we should see:

  elc

and then

  lco

until time ticks to the end of the string. At that point,
we should see:

  IPD

and then

  PD!

and then the beginning of the string should appear, right
next to the content at the end, namely:

  D!W

and then

  !We

and then

  Wel

as before. And the entire process should repeat, for all time.

Try to make it be the case that if someone changes the
definition of `complete-message`, then your program should still
work (displaying the different message). If there are any situations
where your program would fail to work for some possible complete-messages,
note them in comments in your solution.

Think carefully about what the World state should be. It
needs to be some kind of a number, but not just any old number
is allowed. 

|#

(define complete-message "Welcome to IPD!")


#|
Note: when making the image, consider using a fixed-width font. The font
named “Courier New” or “Monospace” might work. (Usually most
computers have one of those installed.) Below is a helper function that
should do the trick on all platforms (but will look best on a mac)
|#

;; monspaced-text : string -> image
;; the body of this function uses something called a "symbol"
;; it is a lot like a string, except it begins with a ' mark
;; and ends with whitespace. Don't worry about this too much;
;; they are needed to call “text/font”
(define (monospaced-text str)
  (text/font str
             36
             "black"
             "Menlo" 'modern
             'normal 'normal #f))
