\ Project Euler 114: Counting Block Combinations I.
require math.fs

: p114 ( -- n )
  51 cells allocate throw 0 { ways result }
  ways 51 cells erase 1 ways !
  51 1 do
    i 1- ways prefix@ i ways prefix!
    i 3 >= if i 1+ 3 do
      i j = if 1 else j i - 1- ways prefix@ then
      j cells ways + +!
    loop then
  loop
  50 ways prefix@ to result ways free throw result ;

[undefined] euler-testing [if] p114 . cr bye [then]
