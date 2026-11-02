\ Project Euler 116: Red, Green or Blue Tiles.
require math.fs

: p116 ( -- n )
  51 cells allocate throw 0 0 0 { ways total length result }
  5 2 do
    i to length ways 51 cells erase 1 ways !
    51 1 do
      i 1- ways prefix@ to result
      i length >= if i length - ways prefix@ result + to result then
      result i ways prefix!
    loop
    50 ways prefix@ 1- total + to total
  loop ways free throw total ;

[undefined] euler-testing [if] p116 . cr bye [then]
