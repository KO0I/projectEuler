\ Project Euler 115: Counting Block Combinations II.
require math.fs

: p115 ( -- n )
  256 cells allocate throw 0 0 { ways result index }
  ways 256 cells erase 1 ways !
  256 1 do
    i 1- ways prefix@ to result
    i 50 >= if i 1+ 50 do
      i j = if 1 else j i - 1- ways prefix@ then
      result + to result
    loop then
    result i ways prefix!
    result 1000000 > if ways free throw i unloop exit then
  loop ways free throw -1 ;

[undefined] euler-testing [if] p115 . cr bye [then]
