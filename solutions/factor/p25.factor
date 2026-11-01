USING: io kernel locals math math.functions math.order math.parser ranges sequences ;
IN: scratchpad

! Find first Fibonacci index whose exact decimal length is 1000.
:: p25 ( -- n )
    1 :> a! 1 :> b! 2 :> index!
    [ b number>string length 1000 < ] [
        a b + :> next
        b a! next b!
        index 1 + index!
    ] while
    index ;
p25 number>string print
