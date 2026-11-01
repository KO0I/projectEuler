USING: io kernel locals math math.functions math.order math.parser ranges sequences ;
IN: scratchpad

! Iterate Fibonacci pairs; only even terms contribute.
:: p2 ( -- n )
    1 :> a! 2 :> b! 0 :> result!
    [ b 4000000 <= ] [
        b 2 mod zero? [ result b + result! ] when
        a b + :> next
        b a!
        next b!
    ] while
    result ;
p2 number>string print
