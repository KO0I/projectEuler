USING: io kernel locals math math.functions math.order math.parser ranges sequences ;
IN: scratchpad

! A ring with side s contributes 4*s^2 - 6*(s-1).
:: p28 ( -- n )
    1 :> answer!
    1 500 [a..b] [| ring |
        ring 2 * 1 + :> side
        side sq 4 * side 1 - 6 * -
        answer + answer!
    ] each
    answer ;
p28 number>string print
