;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname outline) (read-case-sensitive #t) (teachpacks ((lib "image.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "image.rkt" "teachpack" "2htdp")) #f)))
;; A VideoConnector is one of:
;; -- 'vga
;; -- 'dvi
;; -- 'mini-display-port
;; -- 'hdmi

;; VideoConnector -> Boolean
;; determines whether the given connector is digital

;; VideoConnector VideoConnector -> Boolean
;; determines whether converting between connectors requires a
;; digital-to-analog converter.

;; A Season is one of:
;; -- 'winter
;; -- 'spring
;; -- 'fall
;; -- 'summer

;; A Date is (make-date Number Number)
;;
;; Interpretation: (make-date month day) is the *day*th day of month *month*

(define SPRING-EQUINOX (make-date 3 20))
(define SUMMER-SOLSTICE (make-date 6 21))
(define AUTUMN-EQUINOX (make-date 9 23))
(define WINTER-SOLSTICE (make-date 12 21))

;; Date Date -> Boolean
;; determines whether the first date is earlier than or equal to the second

;; Date -> Boolean
;; determines whether the date is in the summer
                        
;; A Mark is one of:
;; -- 'check-plus
;; -- 'check
;; -- 'check-minus
;; -- 'nothing
;;
;; Interpretation: in order: excellent work, adequate work, nearly
;; adequate work, or inadequate work

;; A Grade is [0, 1]
;;
;; Interpretation: 1 is full credit, 0 is no credit, in between is partial

;; Mark -> Grade
;; Converts a symbolic mark to a numeric grade.
;;
;; Examples:
;;  - 'check-plus is worth 1
;;  - 'check is worth 0.9
;;  - 'check-minus is worth 0.7
;;  - 'nothing is worth 0
;;
;; Strategy: structural decomp.
(define (mark->grade mark)
  (cond
    [(symbol=? mark 'check-plus)  1]
    [(symbol=? mark 'check)       0.9]
    [(symbol=? mark 'check-minus) 0.7]
    [(symbol=? mark 'nothing)     0]))

(check-expect (mark->grade 'check-plus 1))
(check-expect (mark->grade 'check 0.9))
(check-expect (mark->grade 'check-minus 0.7))
(check-expect (mark->grade 'nothing 0))

;; A Breakdown is (make-breakdown Mark Mark Mark Mark Mark)
;;
;; Interpretation:
;;  - presentation: design is explained clearly
;;  - understanding: student understands code, based on Q&A
;;  - design: correct design process (recipe) is evident in code
;;  - correctness: code appears to work
;;  - conformance: assignment follows specification carefully
(define-struct breakdown (presentation
                          understanding
                          design
                          correctness
                          conformance))

;; GradeSheet -> Grade
;; Averages a grade sheet of marks to a single numeric grade

;; A HomeworkGrade is one of:
;; -- (make-grade-sheet Breakdown String)
;; -- 'missing
;; -- 'future
;;
;; Interpretation:
;;  - the marks for a homework along with a free-form comment, or
;;  - the homework is past due (and thus counts for zero), or
;;  - the homework is due in the future (and thus doesn't count yet).
(define-struct grade-sheet (breakdown comments))

;; A HomeworkGrades is (make-hw-grades hw1 hw2 hw3 hw4 hw5 hw6 hw7 hw8 ...?)
;;
;; Interpretation: the grades for each homework.
(define-struct hw-grades (hw1 hw2 hw3 hw4 hw5 hw6 hw7 hw8))

;; An ExamGrade is one of:
;; -- Grade
;; -- 'future
;;
;; Interpretation:
;;  - the grade on the exam, or
;;  - the exam hasn't been given yet.

;; An StudentGrades is (make-student-grades HomeworkGrades ExamGrade)
;;
;; Interpretation: homework grades and an exam grade
(define-struct student-grades (homework exam))

;; StudentGrades -> Grade
;; Computes a student's current grade

;; NetID is String
;;
;; Interpretation: A student's Northwestern NetID

;; A Gradebook is one of:
;; -- 'end-of-book
;; -- (make-entry NetID StudentGrades Gradebook)
;;
;; Interpretation: a collection of student records, each entry of which
;; associates a student (via NetID) with their grades.
(define-struct entry (netid grades rest))

;; A HWSpec is an Integer in [1, LAST-HOMEWORK]

;; Gradebook NetID HWSpec -> Grade
;; Looks up a given student's grade on a given assignment.

;; Gradebook -> Grade
;; Computes the class's average (mean) grade.

;; Gradebook HWSpec -> Grade
;; Computes the class's average (mean) grade on a given homework.

;; Gradebook HWSpec -> NetID
;; Finds the student with the highest grade on the given homework.

