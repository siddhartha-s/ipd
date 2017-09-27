;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname traffic-light) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
#|

In Chicago, some traffic lights have, in addition to the
red/orange/green phases, a special pedestrian
crossing indication that changes to walk slightly before
the light turns green and an orange hand that appears
slightly before the main traffic light turns orange.

Design a data-type to capture all of the different
states that the light can take on. (Our solution
has five states; yours should probably too, but if
you think a different number makes more sense, go
for it.) When changing from one state to the next,
either the light color should change or the
walk/don’t-walk status should change, never both.
Note that each state should show both the traffic
light and the walk sign.

Write functions to support a big-bang program that
draws a traffic light that cycles appropriately.

|#
