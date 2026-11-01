USING: io kernel locals math math.functions math.order math.parser ranges sequences ;
IN: scratchpad

! For perimeter p, derive b = p(p-2a) / (2(p-a)).
! Check exact divisibility, then enforce a < b < c.
:: p39 ( -- n )
    0 :> best-count!
    0 :> best-perimeter!
    6 500 [a..b] [| half |
        half 2 * :> p
        0 :> count!
        1 p 3 /i [a..b] [| a |
            p p a 2 * - * :> numerator
            p a - 2 * :> denominator
            numerator denominator mod zero? [
                numerator denominator /i :> b
                p a - b - :> c
                a b < b c < and [
                    count 1 + count!
                ] when
            ] when
        ] each
        count best-count > [
            count best-count!
            p best-perimeter!
        ] when
    ] each
    best-perimeter ;
p39 number>string print
