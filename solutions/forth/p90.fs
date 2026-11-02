\ Project Euler 90: Cube Digit Pairs.
require math.fs

: p90-valid? 0 0 0 { a b x y squares } ( mask mask -- flag )
  1 6 lshift 1 9 lshift or to squares
  a squares and if a squares or to a then
  b squares and if b squares or to b then
  10 1 do
    i i * to x
    1 x 10 / lshift to y
    1 x 10 mod lshift to x
    a x and 0<> b y and 0<> and
    a y and 0<> b x and 0<> and or 0= if false unloop exit then
  loop true ;

: p90 ( -- n )
  210 cells allocate throw 0 0 { masks count answer }
  1024 0 do i bit-count 6 = if i count masks prefix! count 1+ to count then loop
  count 0 do count i do
    i masks prefix@ j masks prefix@ p90-valid? if answer 1+ to answer then
  loop loop masks free throw answer ;

[undefined] euler-testing [if] p90 . cr bye [then]
