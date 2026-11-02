\ Project Euler 73: Counting Fractions in a Range.
require math.fs

: p73 ( -- n ) 0 1 3 4000 11999 0 0 0
  { count a b c d k next-c next-d }
  begin
    c 2 * d = if count exit then
    count 1+ to count
    12000 b + d / to k
    k c * a - to next-c
    k d * b - to next-d
    c to a d to b next-c to c next-d to d
  again ;

[undefined] euler-testing [if] p73 . cr bye [then]
