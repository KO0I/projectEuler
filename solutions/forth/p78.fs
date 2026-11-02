\ Project Euler 78: Coin Partitions.
require math.fs

: p78 ( -- n )
  100001 cells allocate throw 0 0 0 0 0 { ways value k first second sign }
  ways 100001 cells erase 1 ways !
  100000 1 do
    0 to value 1 to k
    begin k 3 k * 1- * 2 / to first
      first i <= while
      first k + to second
      k 2 mod if 1 else -1 then to sign
      i first - ways prefix@ sign * value + to value
      second i <= if i second - ways prefix@ sign * value + to value then
      k 1+ to k
    repeat
    value 1000000 mod dup 0< if 1000000 + then dup i ways prefix! 0= if
      ways free throw i unloop exit
    then
  loop ways free throw -1 ;

[undefined] euler-testing [if] p78 . cr bye [then]
