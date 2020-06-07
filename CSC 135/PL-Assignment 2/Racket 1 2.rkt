;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |Racket 1 2|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
1
(define (volume r)
  (* (* (/ (expt r 3) 3) 3.14) 4))

(check-expect (volume 3) 113.04)
(check-expect (volume 5.1) 555.36552)

(define (shell-volume r1 r2) (- (volume r1) (volume r2)))

(check-expect (shell-volume 5.1 3) 442.32552)
(check-expect (shell-volume 3 3) 0)

2
(define (close? n1 n2)
  (cond
    [(<= (abs (- n1 n2)) .001) #t]
    [#t #f]
  )
)

(check-expect (close? 5 4.999) #t)
(check-expect (close? 4.999 5) #t)
(check-expect (close? 5 1) #f)

(define (close2? n1 n2 limit)
  (cond
    [(<= (abs (- n1 n2)) limit) #t]
    [#t #f]
  )
)
(check-expect (close2? 5 4 1) #t)
(check-expect (close2? 5 3 1) #f)
(check-expect (close2? 5 5 5) #t)