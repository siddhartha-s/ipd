;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname alpaca-genealogy) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
#|
Problems

The Alpaca Owners and Breeders Association needs
your help! They’re having trouble using the database of
detailed pedigree records that they keep for all registered
alpacas.

But first, some background. For every alpaca in the
registry, they keep several different pieces of
information:

- name
- sex
- date of birth
- fleece color
- sire (father), if known
- dam (mother), if known

Unsurprisingly, AOBA uses DrRacket to store and query the
alpaca registry, with the following data definitions:
|#

; A Sex is one of:
; - "female"
; - "male"

; A DoB is (make-date Year Month Day)
;   where
; Year is an Integer in [1900, 2100]
; Month is an Integer in [1, 12]
; Day is an Integer in [1, 31]
(define-struct date (year month day))

; An Alpaca is one of:
; - (make-alpaca String Sex DoB Color Alpaca Alpaca)
; - "unknown"
(define-struct alpaca (name sex dob color sire dam))
      
;; An examples, here’s the representation of the record for some Alpacas:

(define dana-andrews
  (make-alpaca "Dana Andrews"
               "female"
               (make-date 1996 8 14)
               "silver"
               "unknown"
               "unknown"))
(define jericho
  (make-alpaca "Jericho de Chuchata"
               "male"
               (make-date 1997 11 23)
               "black"
               "unknown"
               "unknown"))
(define sylvan 
  (make-alpaca "MA Sylvan"
               "male"
               (make-date 2001 5 16)
               "black"
               "unknown"
               "unknown"))
(define mfa
  (make-alpaca "MFA Independence"
               "female"
               (make-date 2002 7 2)
               "black"
               jericho
               dana-andrews))
(define irene
  (make-alpaca "Irene of Acorn Alpacas"
               "female"
               (make-date 2007 5 21)
               "silver"
               sylvan
               mfa))

#|

First, add two more alpacas.
(Come back here and add more Alpacas that
you think would make good test cases as
you design the rest of the functions for
this homework assignment.)

|#


#|
Second, AOBA would like a program to make a rather simple
query: Given an alpaca, they would like to find out the
names of all the female-line ancestors of the given alpaca
in a list, youngest to oldest, and including the given
alpaca. So for example, given the structure for Irene above,
it should return the list

(list "Irene of Acorn Alpacas"
      "MFA Independence"
      "Dana Andrews")

which contains Irene's name, her mother's name, and her
grandmother's name, and then stops because her great
grandmother is unknown. Design the function female-line.
(You probably need a new data definition in order to write
the correct signature.)

|#


#|
Many breeders raise alpacas for their fleece, which comes in
a wide variety of colors and may be made into a wide variety
of textiles. Some breeders are interested in breeding
alpacas with new colors and patterns, and to do so, they
need to understand how fleece colors and patterns are
inherited.

You can help them by designing a function has-color? that
takes an alpaca pedigree and a color, and reports whether or
not that color is known to appear anywhere in the pedigree tree.
|#


#|
AOBA is worried about fraud in their registry. Eventually
they’ll send investigators into the field, but first they’d
like to run a simple sanity check on the database. Given the
pedigree record for an alpaca, there are two simple errors
that you can find:

Some alpaca in the tree has a birthday before one of his or
her parents.

Some alpaca in the tree has a male alpaca listed as dam or a
female alpaca listed as sire.

Design a function pedigree-error? that returns true if a
given Alpaca has one of those two obvious errors in his or
her pedigree, and false otherwise.
|#


#|
For all other problems in this assignment, you may assume
that all alpaca records are valid, in the sense that
pedigree-error? answers false for them. In the next problem,
for example, it will save you trouble if you don’t have to
consider the possibility that any alpaca’s date of birth
could precede its parents’.

Tracing back an alpaca’s ancestry as far as possible is a
point of pride in the alpaca-raising community. Design a
function oldest-ancestor that, given an alpaca’s pedigree
record, returns its oldest known ancestor's name and
returns #false if there is no known ancestor.

Hint: Use a data definition:

  An maybe-name is either:
     #false
     string

and write functions that operate on it, discovering which
ones you need as you work out oldest-ancestor.
|#

#|
AOBA also wants a way to list all the known ancestors of a
given alpaca (including the given alpaca) in reverse birth
order. For example, for Irene, the result would be:

  (list irene
        mfa
        sylvan
        jericho
        dana-andrews)

Design a function all-ancestors/sorted to perform this task.

Hint: In order to do so, you will need a data definition for
a list of alpacas (the conventional one will do), and you
will likely need a helper function merge-alpacas that, given
two sorted lists of alpacas, merges them into a single
sorted list of alpacas. See HTDP/2e section 26.5 for how
to design a template for this:

  http://www.ccs.neu.edu/home/matthias/HtDP2e/part_four.html#%28part._sec~3atwo-inputs~3adesign%29

|#
