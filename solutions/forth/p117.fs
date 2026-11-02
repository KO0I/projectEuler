\ Project Euler 117: Red, Green, and Blue Tiles.
require math.fs

: p117 ( -- n )
  51 cells allocate throw 0 { ways result }
  ways 51 cells erase 1 ways !
  51 1 do
    5 1 do j i >= if j i - ways prefix@ j cells ways + +! then loop
  loop
  50 ways prefix@ to result ways free throw result ;

[undefined] euler-testing [if] p117 . cr bye [then]
