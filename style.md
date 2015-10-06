# IPD Style Rules

## Naming

  - If the homework assignment specifies a function name, you *must* use
    that function name.

  - Function and parameter names must be in `hyphenated-lowercase`;
    write the words in lowercase and join with hyphens.

  - Constant names must be in `HYPHENATED-UPPERCASE`;
    write the words in uppercase and join with hyphens.

  - Data definition names must be in `UpperCamelCase`;
    capitalize each word and append them.

  - Names of things must reflect their purposes. Do not use generic
    names such as `number` or `function`.

  - Names with larger scope, such as function names, should generally be
    more descriptive (and longer) than names with smaller scope, such as
    function parameters.

## Code formatting

### Line breaking

  - Lines must be at most 80 characters long.

      - Longer lines must be broken; insert line breaks where you think
        they will maximize readability. Prefer to break between larger
        syntactic elements rather than smaller.

        Not great:
        ```racket
        (check-expect (my-function arg1 arg2
                                   arg3 arg4) result)
        ```

        Better:
        ```racket
        (check-expect (my-function arg1 arg2
                                   arg3 arg4)
                      result)
        ```

        Better still:
        ```racket
        (check-expect
          (my-function arg1 arg2 arg3 arg4)
          result)
        ```

      - DrRacket can help you with this. In *Preferences*, go to the
        *Editing* section, and select “Maximum character width guide”
        with a width of 80. This will cause DrRacket to draw a right
        margin in your editor window.

  - In function definitions, a mandatory line break follows the function
    name and arguments list:

    Bad:
    ```racket
    (define (my-function arg1 arg2) function-body-stuff)
    ```

    Good:
    ```racket
    (define (my-function arg1 arg2)
      function-body-stuff)
    ```

  - Parentheses and square brackets (`(`, `)`, `[`, `]`) must never
    appear on lines by themselves.

      - Put left delimiters on the same line as the code that follows.

      - Put right delimiters on the same line as the code that precedes.

    Bad:
    ```racket
    (define (foo bar) (
      cond
        [(test1? bar) ...]
        [(test2? bar) ...]
    )
    ```

    Good:
    ```racket
    (define (foo bar)
      (cond
        [(test1? bar) ...]
        [(test2? bar) ...])
    ```

### Horizontal spacing

  - You must indent your code properly, which means that the indentation
    follows the nesting structure.

      - The easiest way to do this in DrRacket is to select the code you
        wish to re-indent and press the *Tab* key.

      - It’s a good idea to reindent everything before submitting.

  - Adjacent items *must* have space between them, except that adjacent
    delimiters of the same direction *must not* have space between them.

      - Delimiters must have no space inside them—that is, the next token
        must follow without space in between.

        Bad:
        ```racket
        ( cond
          [ (test? qux)
            ( cons 4 ( list 3 2 1 ) ) ]
          ...
        ```

        Good:
        ```racket
        (cond
          [(test? qux)
           (cons 4 (list 3 2 1))]
          ...
        ```

      - Delimiters must have space outside them except when directly inside
        another delimiter:

        Bad:
        ```racket
        (+(* a(+ b c)(f e))d)
        ```

        Good:
        ```racket
        (+ (* a (+ b c) (f e)) d)
        ```

## Program layout

Programs must be laid out in the following order:

1.  Data definitions (for the whole program)

2.  Constants (for the whole program)

3.  The most important or main function:

    1.  Signature

    2.  Purpose

    3.  Functional examples

    4.  Strategy

    5.  Code of function

    6.  Tests

4.  Each secondary or auxiliary function in turn:

    1.  Signature

    2.  Purpose

    3.  Functional examples (optional for trivial helpers)

    4.  Strategy

    5.  Code of function

    6.  Tests (optional for trivial helpers)

Note that:

  - Data definitions and constants come before all function definitions.

  - Each function definition holds together, with no other functions,
    data definitions, or constants in the middle of it.

## Design recipe details

### Step 2

### Step 3

### Step 4


