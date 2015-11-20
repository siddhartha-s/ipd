#lang scribble/base

@(require scribble/core
          scribble/manual
          scribble/latex-properties
          scribble/html-properties
          (only-in pict scale))

@title[#:style (style
                #f
                (list
                 ;(document-version "6.2.1")
                 (latex-defaults
                  '(collects #"scribble" #"manual-prefix.tex")
                  '(collects #"scribble" #"manual-style.tex")
                  '())
                 (html-defaults
                  '(collects #"scribble" #"scribble-prefix.html")
                  '(collects #"scribble" #"manual-style.css")
                  '((collects #"scribble" #"manual-fonts.css")))
                 (css-style-addition '(collects #"scribble" #"manual-racket.css"))
                 (js-style-addition '(collects #"scribble" #"manual-racket.js"))))
       ]{Homework 7}
@bold{EECS 495 Intensive Program Design}

@bold{Due date (part 1):} Tuesday, December 1, 11:59 PM

@bold{Due date (part 2):} Tuesday, December 8, 11:59 PM

@bold{Pair program this assignment with your partner.}
Remember, that means that all code should be written while
both of you are sitting together.

@bold{Grading.} You will be graded in codewalks the Wednesdays
after this assignment is due. Again, your grade will
be based principally on your application of the design
recipe and your explanations in your code walk.

@bold{Setup.} Download this archive:
@elem[#:style (style "dl" (list (link-resource "hw6.tar.gz")))]{hw6.tar.gz}.
It contains a skeleton @link["https://cmake.org/"]{CMake} project
that you can use to generate a projects for a number of
systems. Through a Unix shell, you can run the tests easily by
running @tt{make test} in the project root directory.
(If there are failing tests, @tt{make report} will
give you more details.)

@bold{Submitting.} On both Tuesdays, return an archive of the
project to
@tt{burke.fetscher@"@"eecs.northwestern.edu}, with the
subject:
@centered{@tt{EECS 495 - HOMEWORK 7.<1 or 2> <your net id> <your partner's net id>}}

For the final homework assignment, you and your partner will implement a data structure
of your choice. You can either choose from the following options, or, if you
have an idea of your own, we'll need to approve it, so email or post on
Piazza to ask about it:

@itemlist[
 @item{Treaps}
 @item{Skiplists}
 @item{Counting Bloom Filters}
 @item{PATRICIA trees}
 @item{Octrees}
 @item{Min-max heaps}
 @item{Splay trees}
 @item{Fibonacci heaps}
 @item{Ropes}
 @item{B-trees}]

If you need help finding resources describing how these structures work, don't
hesitate to ask. Wikipedia is a good place to start for all of the above.

@section{Part 1 - Interface and representation}

For Part 1, you should decide on an interface, representation,
and signatures for all of your functions, but no implementation.
This should take the form of one or more data definitions and
headers for functions that operate on them. You will present
this in your codewalks on December 2, and we will give you feedback
on your representation and interface design before you proceed
with your implementation. (This portion of the assigment should also include
some examples in the form of test cases. Of course those test cases will
fail until you actually complete your implementation.)

We'll be looking for interfaces that are appropriately generic
(so you should be using templates) and well thought out.

@section{Part 2 -Implementation}

After getting feedback on your design, complete the implementation of
your data structure. You will present the complete implementation in
your codewalks on December 9.