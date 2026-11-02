\ Project Euler 72: Counting Fractions.
require math.fs

: p72 ( -- n )
  1000001 cells allocate throw 0 { phi result }
  1000001 0 do i i phi prefix! loop
  1000001 2 do
    i phi prefix@ i = if
      1000001 i do
        i phi prefix@ dup j / - i phi prefix!
      j +loop
    then
  loop
  1000001 2 do result i phi prefix@ + to result loop
  phi free throw result ;

[undefined] euler-testing [if] p72 . cr bye [then]
