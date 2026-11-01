USING: io kernel locals math math.functions math.order math.parser ranges sequences ;
IN: scratchpad

! Inclusion/exclusion over the fixed interval 0..999.
:: p1 ( -- n )
    0 :> result!
    0 999 [a..b] [| n |
        n 3 mod zero? n 5 mod zero? or [
            result n + result!
        ] when
    ] each
    result ;
p1 number>string print
