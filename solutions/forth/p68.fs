\ Project Euler 68: Magic 5-gon Ring.
require math.fs

create p68-inner 5 cells allot

create p68-outer 5 cells allot

variable p68-best

: p68-search 0 0 0 0 0 0 { depth used first line all candidate representation valid } ( depth used -- )
  depth 5 < if
    10 1 do
      used 1 i lshift and 0= if
        i depth p68-inner prefix!
        depth 1+ used 1 i lshift or recurse
      then
    loop exit
  then
  11 1 do
    used 1 i lshift and 0= if
      i to first
      first 0 p68-inner prefix@ + 1 p68-inner prefix@ + to line
      used to all true to valid
      5 0 do
        line i p68-inner prefix@ - i 1+ 5 mod p68-inner prefix@ - to candidate
        candidate first < candidate 10 > or if false to valid leave then
        all 1 candidate lshift and if false to valid leave then
        candidate i p68-outer prefix!
        all 1 candidate lshift or to all
      loop
      valid if
        0 to representation
        5 0 do
          i p68-outer prefix@ to candidate
          candidate 10 = if representation 100 * 10 + to representation else
            representation 10 * candidate + to representation then
          representation 10 * i p68-inner prefix@ + to representation
          representation 10 * i 1+ 5 mod p68-inner prefix@ + to representation
        loop
        representation p68-best @ > if representation p68-best ! then
      then
    then
  loop ;

: p68 ( -- n ) 0 p68-best ! 0 0 p68-search p68-best @ ;

[undefined] euler-testing [if] p68 . cr bye [then]
