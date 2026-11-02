\ Project Euler 61: Cyclical Figurate Numbers.
require math.fs

create p61-values 900 cells allot

create p61-count 6 cells allot

create p61-path 6 cells allot

: p61-prepare 0 0 0 0 { sides value tally offset }
  p61-count 6 cells erase
  6 0 do
    i 3 + to sides
    200 1 do
      sides 2 - i i * * sides 4 - i * - 2 / to value
      value 1000 >= value 10000 < and value 100 mod 10 >= and if
        j p61-count prefix@ to tally
        tally 150 >= abort" Polygonal fixture exceeded capacity"
        j 150 * tally + to offset
        value offset p61-values prefix!
        tally 1+ j p61-count prefix!
      then
    loop
  loop ;

: p61-recurse 0 0 0 0 { depth used leading candidate answer repeated } ( depth used -- result )
  depth 6 = if
    5 p61-path prefix@ 100 mod 0 p61-path prefix@ 100 / = if
      0 6 0 do i p61-path prefix@ + loop exit
    then 0 exit
  then
  depth 1- p61-path prefix@ 100 mod to leading
  6 0 do
    used 1 i lshift and 0= if
      i p61-count prefix@ 0 do
        j 150 * i + p61-values prefix@ to candidate
        candidate 100 / leading = if
          false to repeated
          depth 0 do i p61-path prefix@ candidate = if true to repeated then loop
          repeated 0= if
            candidate depth p61-path prefix!
            depth 1+ used 1 j lshift or recurse to answer
            answer 0<> if answer unloop unloop exit then
          then
        then
      loop
    then
  loop 0 ;

: p61 ( -- n ) 0 { answer }
  p61-prepare
  5 p61-count prefix@ 0 do
    5 150 * i + p61-values prefix@ 0 p61-path prefix!
    1 1 5 lshift p61-recurse to answer
    answer 0<> if answer unloop exit then
  loop -1 ;

[undefined] euler-testing [if] p61 . cr bye [then]
