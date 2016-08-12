#lang racket

#|

Use the Language|Choose Language ... menu item
and select "The Racket Language" before running
this file.

Change the definition here to be a list of strings
that names the full path to your C program and then
whatever arguments it needs (if any, maybe none).

Note that, on Windows, paths have the backslash
characters and strings in Racket need to have that
backslash characters escaped. So if you program
is in the root of the C: drive, you would write:

   "C:\\prog.exe"

After fixing that, skip down to the next commented region
to see how to write specific tests.

|#

(define program-and-arguments
  (list "C:\\Users\\robby\\git\\exp\\plt\\racket\\racket.exe"
        "-l" "racket/base"
        "-l" "racket/port"
        "-e" "(copy-port (current-input-port) (current-output-port))"))

(define-syntax (run-test stx)
  (syntax-case stx ()
    [(_ input expected-output)
     #`(run-test/proc #,(syntax-line stx) input expected-output)]))

(define failures 0)
(define tests 0)

(define (run-test/proc line _input _expected-output)
  (set! tests (+ tests 1))
  (unless (or (string? _input) ((listof (and/c string? (not/c #rx"\n"))) _input))
    (error 'run-test
           "expected a string or a list of strings without newlines for the input argument, got ~e"
           _input))
  (unless (or (string? _expected-output)
              ((listof (and/c string? (not/c #rx"\n"))) _expected-output))
    (error 'run-test
           "expected a string or a list of strings without newlines for the output argument, got ~e"
           _expected-output))
  (define (make-simple-string s)
    (cond
      [(string? s) s]
      [else
       (apply
        string-append
        (for/list ([i (in-list s)])
          (string-append i "\n")))]))
  (define input (make-simple-string _input))
  (define expected-output (make-simple-string _expected-output))
  (define out (open-output-string))
  (define err (open-output-string))
  (define in (open-input-string input))
  (define-values (_1 _2 pid _3 proc)
    (apply values (apply process*/ports out in err
                         program-and-arguments)))
  (proc 'wait)
  (define err-str (get-output-string err))
  (cond
    [(equal? err-str "")
     (define got-output (get-output-string out))
     (unless (equal? got-output expected-output)
       (set! failures (+ failures 1))
       (eprintf "test failed, line ~a\n      got: ~s\n expected: ~s\n"
                line got-output expected-output))]
    [else
     (set! failures (+ failures 1))
     (eprintf "test failed, line ~a, stderr output:\n~a\n"
              line err-str)]))

(unless (file-exists? (car program-and-arguments))
  (eprintf "The program does not exist. Double check that you spelled the name right.\n")
  (exit))

(define (summary)
  (cond
    [(= failures 0)
     (cond
       [(= tests 0) (void)]
       [(= tests 1) (printf "1 test passed.\n")]
       [(= tests 2) (printf "both tests passed.\n")]
       [else (printf "all ~a tests passed.\n" tests)])]
    [(= failures 1) (eprintf "\n1 test failed.\n")]
    [else (eprintf "\n~a tests failed.\n" failures)]))

#|

Down here, fill in the test cases. Each line should
have a call to `run-test` in it, where the first argument
is the input to be supplied to the program (in a string)
and the second argument is the output expected from
the program (also in a string)

|#

(run-test "a" "b")

#|

If you wish to supply multi-line inputs to your program,
call `run-test` with a list of strings, one for each line.
Do not add newlines to the strings, run-test will do that
for you. For example, this call supplies three lines to
your program and expects three lines back.

|#

(run-test (list "a" "b" "c")
          (list "d" "e" "f"))

#|

Call the summary function to get a summary of the test results

|#

(summary)
