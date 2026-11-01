USING: io kernel locals math math.functions math.order math.parser ranges sequences ;
IN: scratchpad

: digit-sum ( n -- sum ) number>string [ 48 - ] map sum ;
! Factor's product fold operates on an exact bignum.
: p20 ( -- n ) 100 [1..b] product digit-sum ;
p20 number>string print
