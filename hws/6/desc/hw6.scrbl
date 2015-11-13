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
                 (js-style-addition '(collects #"scribble" #"manual-racket.js"))))]{Homework 5}
@bold{EECS 495 Intensive Program Design}

@bold{Due date:} Tuesday, November 3, 11:59 PM @;{TODO - udpate - when?}

@bold{Pair program this assignment with your partner.}
Remember, that means that all code should be written while
both of you are sitting together.

@bold{Grading.} You will be graded in codewalks the Wednesday
after this assignment is due. Again, your grade will
be based principally on your application of the design
recipe and your explanations in your code walk.

@bold{Setup.} Download this archive: @;{TODO - link}
It contains a project etc...

@bold{Submitting.} Complete both exercises in a single file
and submit the file to @tt{5-heaps-and-codes}.


@section{Disjoint Sets of Integers}

In this exercise you will design a data structue representing disjoint
sets of nonnegative integers.
The operations required are:
@itemlist[
 @item{Create an initial set of sets. (We'll use a constructor for this.)}
 @item{@tt{find}, which takes an integer as its argument,
  and returns a representative member of the set it belongs to.}
 @item{@tt{union}, which takes two integers as its arguments and
        combines the sets they belong to.}]
It's easiest to see how these work with an example.
Suppose you start by constructing four sets, each containing an integer from
the range @italic{[0, 4)}:
@centered{@italic{{0} {1} {2} {3}}}
At this point, @tt{find(@italic{n})}@italic{ = n}, for any @italic{n}
in @italic{[0, 4)}, since each set only has one member.

After the operation @tt{union(1,3)}, your sets will look like:
@centered{@italic{{0} {1,3} {2}}}
At this point, you don't know if @tt{find(1)}
is @tt{1} or @tt{3}, but it is a requirement that there is only
one representative for each set, so @tt{find(1) == find(3)}. In fact,
this is how we can check to see if two numbers belong to the same set.

After the operation @tt{union(1,2)}, the sets will look like:
@centered{@italic{{0} {1,3,2}}}
And after @tt{union(3,0)}, there will be a single set and for any
@italic{m,n ∈ [0, 4)}, @tt{find(@italic{m}) == find(@italic{n})}.

You'll implement these sets as a class with a constructor creating
a range of singleton sets, as above, and two public functions
@tt{do_union} and @tt{find}. A skeleton for the class is in
@tt{src/union_find} in the provided archive. The constructor should
take a size argument @italic{n} and create a representation of the singleton
sets: @italic{{0} {1} ... {n-1}}.

Implement your sets using a vector @tt{v}, such that each element in the vector
is also an index into the vector. If the element at position @tt{n} is @tt{n}, then @tt{n}
is a representative, otherwise we can find @tt{n}'s representative by
looking at the element at position @tt{v[n]}. So if the vector looks like:
@tt{[0,1,2,3]}, then that is a representation of the singleton sets constructed
initially above. And the vector @tt{[0,1,2,1]} is one possible representation
of the situation after @tt{union(1,3)}, and in this case @tt{find(3) == 1}.

}