\ Project Euler 17: Number Letter Counts.
require math.fs

create letter-small
  0 , 3 , 3 , 5 , 4 , 4 , 3 , 5 , 5 , 4 ,
  3 , 6 , 6 , 8 , 8 , 7 , 7 , 9 , 8 , 8 ,

create letter-tens 0 , 0 , 6 , 6 , 5 , 5 , 5 , 7 , 6 , 6 ,

: small-letters ( n -- count ) cells letter-small + @ ;

: tens-letters ( n -- count ) cells letter-tens + @ ;

: count-letters 0 { n answer } ( n -- count )
  n 1000 = if 11 exit then 0 to answer
  n 100 >= if
    n 100 / small-letters 7 + answer + to answer
    n 100 mod to n n 0<> if answer 3 + to answer then
  then
  n 20 >= if n 10 / tens-letters answer + to answer n 10 mod to n then
  n 0> if n small-letters answer + to answer then answer ;

: p17 ( -- answer ) 0 1001 1 do i count-letters + loop ;

[undefined] euler-testing [if] p17 . cr bye [then]
