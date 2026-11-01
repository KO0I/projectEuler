USING: io kernel locals math math.functions math.order math.parser ranges sequences ;
IN: scratchpad

: digit-sum ( n -- sum ) number>string [ 48 - ] map sum ;
! Factor has arbitrary-precision integers; no fixed limb array.
: p16 ( -- n ) 2 1000 ^ digit-sum ;
p16 number>string print
