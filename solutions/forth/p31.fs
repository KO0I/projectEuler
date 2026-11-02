\ Project Euler 31: Coin Sums.
require math.fs

create p31-coins 1 , 2 , 5 , 10 , 20 , 50 , 100 , 200 ,

: p31 ( -- n )
  201 cells allocate throw 0 0 { ways coin result }
  ways 201 cells erase 1 ways !
  8 0 do i p31-coins prefix@ to coin
    201 coin do i coin - ways prefix@ i cells ways + +! loop
  loop
  200 ways prefix@ to result ways free throw result ;

[undefined] euler-testing [if] p31 . cr bye [then]
