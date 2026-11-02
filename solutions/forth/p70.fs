\ Project Euler 70: Totient Permutation.
require math.fs

: p70 ( -- n )
  10000000 cells allocate throw 0 1 0 { phi best best-phi value }
  10000000 0 do i i phi prefix! loop
  10000000 2 do
    i phi prefix@ i = if
      10000000 i do
        i phi prefix@ dup j / - i phi prefix!
      j +loop
    then
  loop
  10000000 2 do
    i phi prefix@ to value
    i digit-signature value digit-signature = if
      best 0= if i to best value to best-phi else
        i best-phi * best value * < if i to best value to best-phi then
      then
    then
  loop phi free throw best ;

[undefined] euler-testing [if] p70 . cr bye [then]
