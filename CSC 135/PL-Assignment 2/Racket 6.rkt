;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Racket 6|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
6
(require racket)
(define (incnth n)
  (lambda (x)
    (+ n x)
    )
  )

(check-expect ((incnth 3) 2) 5)
(check-expect ((incnth -2) 3) 1)