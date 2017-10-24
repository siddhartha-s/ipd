# IPD Homework 7: Hexapawn

  - **Due: Tuesday, November 14 at 11:59 PM**

## Summary

For this homework, you will write an interactive program implementing
the two-player abstract strategy game
[Hexapawn](https://en.wikipedia.org/wiki/Hexapawn).

## Goals

To design a whole program from scratch, while maintaining good style and
logical factoring.

## Specification

The rules of Hexapawn are available in [the Hexapawn Wikipedia
article](https://en.wikipedia.org/wiki/Hexapawn), and your program
should implement these rules.

Unlike traditional 3×3 Hexapawn, your game should support board
dimensions from 3×3 to 8×8. Changing a dimension should be as easy as
changing a constant (or you can integrate dimension selection into your
UI).

At each turn, your game should display the state of the board and which
player’s turn it is. It should then allow the player to select a move.
Here is an example of what that display might look like for a 5×3 game:

````
   | a | b | c | d | e |
---+---+---+---+---+---+---
 3 | B | B | B | B | B | 3
---+---+---+---+---+---+---
 2 |   | W |   |   |   | 2
---+---+---+---+---+---+---
 1 | W |   | W | W | W | 1
---+---+---+---+---+---+---
   | a | b | c | d | e |

Player B’s move:
  0: a3–a2
  1: a3–b2
  2: c3–c2
  3: c3–b2
  4: d3–d2
  5: e3–e2
>
````

If you wish, you may improve upon the display. For example, you could
rotate the display of the board so that each player moves up the board,
rather than player B moving down.

When a player wins, either by reaching the other side or leaving the
other player with no available moves, a messages to this effect should
be displayed and the program should terminate.

## Deliverables

Your deliverables are:

  - The source code of the `hexapawn` program as described
    above.

  - A plain text file `EVALUATION.md` (which can use
    [Markdown](https://help.github.com/articles/github-flavored-markdown/)
    formatting if you like) in which you describe the design of your
    program. In particular please discuss, briefly:

      - how you tested your programs to ensure correctness and

      - anything that surprised you while doing this assignment.

    When discussing your code, please provide file and line number
    references.

## How to submit

Please submit a ZIP or TAR archive of your whole project. Your project
should build in CLion using the configuration in CMakeLists.txt. Before
creating the archive, be sure to clean your project (*Run* menu /
*Clean*).
