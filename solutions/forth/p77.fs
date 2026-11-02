\ Project Euler 77: Prime Summations.
require math.fs

: p77 ( -- n ) 128 cells allocate throw 2 { ways n }
  2 to n begin ways 128 cells erase 1 ways !
    n 1+ 2 do i is-prime? if
      n 1+ i do i j - ways prefix@ i cells ways + +! loop
    then loop
    n ways prefix@ 5000 > if ways free throw n exit then
    n 1+ to n
  again ;

[undefined] euler-testing [if] p77 . cr bye [then]
