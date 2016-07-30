;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname lorenz) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
#|

The Lorenz attractor is a beautiful (IMO) non-repeating motion
in three dimensional space that is governed by the following
differential equations (for appropriate definitions of the constants
sigma, rho, and beta):

dx/dt = sigma * (y-x)
dy/dt = x*(rho - z) - y
dz/dy = x*y - beta*z

We can simulate the motion that these equations describe by
translating them into BSL. (If you are not familiar with differential
equations, nevermind; the assignment explains how to use them.)

|#

(define sigma 10)
(define beta 8/3)
(define rho 28)

(define time-step #i0.01)

(define (next-x x y z)
  (+ x (* time-step (* sigma (- y x)))))

(define (next-y x y z)
  (+ y (* time-step (- (* x (- rho z)) y))))

(define (next-z x y z)
  (+ z (* time-step (- (* x y) (* beta z)))))

(check-within (next-x 1 1 1) 1 0.01)
(check-within (next-y 1 1 1) 1.26 0.01)
(check-within (next-z 1 1 1) 0.98 0.01)


#|

The three functions next-x, next-y, and next-z compute the new positions for the
attractor given the old positions. That is, next-x consumes the current (x,y,z)
position and returns a new x coordinate. Ditto for next-y and next-z.

Note that the "#i" in front of the time-step signifies that this is an "inexact"
number. See the documentation on check-within to help you testing functions
that use those "inexact" numbers. There is a lot to say about these kinds of numbers;
for now just now that there may be some (usually slight) imprecision in these numbers.

Design a world structure that animates the motion of the Lorenz attractor.
The world structure should hold the three most recent points that the attractor
has visited.

Start by designing a structure for three dimensional points, much like the one
for posns, except with three coordinates. Include some examples and be sure
to include the initial point for the Lorenz simulation, namely the point
where x=1, y=1, and z=1.

To model the Lorenz attractor, you need to track the position in three dimensional
space, but to draw it, you need to map that two dimensions. Do that by discarding
the 'z' coordinate, multiplying the 'x' and 'y' coordinates by 10 and then adding
300 to them. Use a blank canvas of size 600x600 to draw the image.

The Wikipedia page (near the top) has an animation of the Lorenz attractor. Your world
program should look something like the dot that moves around, except instead of showing the
entire picture in the background, it should show the only the four most recent points,
connected by three lines.


|#
