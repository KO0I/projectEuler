USING: io kernel locals math math.functions math.order math.parser ranges sequences ;
IN: scratchpad

! Difference between square of sum and sum of squares.
: p6 ( -- n )
    100 101 * 2 /i sq
    100 101 * 201 * 6 /i - ;
p6 number>string print
