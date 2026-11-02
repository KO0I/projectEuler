\ Project Euler 76: Counting Summations.
require math.fs

: p76 ( -- n )
  101 cells allocate throw 0 { ways answer }
  ways 101 cells erase 1 ways !
  100 1 do
    101 i do i j - ways prefix@ i cells ways + +! loop
  loop
  100 ways prefix@ to answer ways free throw answer ;

[undefined] euler-testing [if] p76 . cr bye [then]
