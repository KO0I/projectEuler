\ Project Euler 74: Digit Factorial Chains.
require math.fs

create p74-factorials 1 , 1 , 2 , 6 , 24 , 120 , 720 , 5040 , 40320 , 362880 ,

: p74-next { number } ( n -- sum )
  0 begin number 0> while
    number 10 mod p74-factorials swap cells + @ +
    number 10 / to number
  repeat ;

: p74 ( -- n )
  2540161 cells allocate throw 2540161 cells allocate throw
  2540161 cells allocate throw 256 cells allocate throw
  0 0 0 0 0 0 0 { length visited positions path answer start number count first cycle tail }
  length 2540161 cells erase visited 2540161 cells erase
  1000000 1 do i to start
    start length prefix@ 0= if
      start to number 0 to count
      begin
        number length prefix@ 0=
        number visited prefix@ start <> and
      while
        start number visited prefix!
        count number positions prefix!
        number count path prefix!
        count 1+ to count
        count 256 >= abort" Unexpected digit-chain depth"
        number p74-next to number
      repeat
      number length prefix@ if
        number length prefix@ to tail
      else
        number positions prefix@ to first
        count first - to cycle
        count first do cycle i path prefix@ length prefix! loop
        first to count cycle to tail
      then
      begin count 0> while
        count 1- to count tail 1+ to tail
        tail count path prefix@ length prefix!
      repeat
    then
    start length prefix@ 60 = if answer 1+ to answer then
  loop
  length free throw visited free throw positions free throw path free throw answer ;

[undefined] euler-testing [if] p74 . cr bye [then]
