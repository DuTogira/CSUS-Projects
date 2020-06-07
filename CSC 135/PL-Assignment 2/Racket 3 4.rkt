;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Racket 3 4|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
3
; function how-many: a b c -> int
(define (how-many a b c)
  (cond
    [(> (* b b) (* (* a c) 4)) 2]
    [(= (* b b) (* (* a c) 4)) 1]
    [(< (* b b) (* (* a c) 4)) 0]
    )
  )

(check-expect (how-many 1 2 3) 0)
(check-expect (how-many 1 0 -1) 2)
(check-expect (how-many 2 4 2) 1)

4
;function filter-out-symbol: string -> string
(require racket/string)
(define (filter-out-symbol phrase remove)
  (string-normalize-spaces (string-replace phrase remove "")))

(check-expect (filter-out-symbol "no no a thousand times no" "no")
              "a thousand times")
(check-expect (filter-out-symbol "I want to have a Tesla" "want to")
              "I have a Tesla")