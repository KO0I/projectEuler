USING: io kernel locals math math.functions math.order math.parser ranges sequences ;
IN: scratchpad

! 1 <= a < b < c; the perimeter fixes c.
:: p9 ( -- n )
    0 :> answer!
    1 333 [a..b] [| a |
        a 1 + 499 [a..b] [| b |
            1000 a - b - :> c
            c b > [
                a sq b sq + c sq = [
                    a b * c * answer!
                ] when
            ] when
        ] each
    ] each
    answer ;
p9 number>string print
