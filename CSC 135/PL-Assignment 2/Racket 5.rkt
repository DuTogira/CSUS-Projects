;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Racket 5|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
5
;define pMinMax: L (list) -> list
(require racket)
(define (pMinMax L)
  (cond 
    [(null? L) '()]
    [(null? (rest L))
     (list (first L) (first L))]
    [else
     (let ([rst (pMinMax (rest L))]
           [fst (first L)])
       (cond
         [(> fst (first rst))
          (cons fst (rest rst))]
         [(< fst (first rst))
          (list (first rst) fst)]
         [else rst]))]))

(check-expect (pMinMax (list 1 2 3 4 5 6)) (list 6 1))
(check-expect (pMinMax (list 6 5 4 3 2 1)) (list 6 1))
(check-expect (pMinMax (list)) (list))
(check-expect (pMinMax (list -1 -1)) (list -1 -1))