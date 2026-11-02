\ Project Euler 103: Special Subset Sums: Optimum.
require math.fs

create p103-current 7 cells allot

create p103-best 7 cells allot

create p103-sums 1024 cells allot

create p103-seen 4096 allot

variable p103-bound

: p103-value@ { index } index cells p103-current + @ ;

: p103-cardinal? 0 0 0 0 0 { chosen k low high index last } ( chosen -- flag )
  1 to k
  begin 2 k * 1+ 7 <= while
    chosen k 1+ >= if
      0 to low 0 to high
      k 1+ 0 do i p103-value@ low + to low loop
      7 k - to index
      begin index 7 < while
        index chosen < if index p103-value@
        else chosen 1- p103-value@ index chosen - + 1+ then
        high + to high
        index 1+ to index
      repeat
      low high <= if false exit then
    then
    k 1+ to k
  repeat true ;

: p103-search 0 0 0 0 0 0 0 0 0 0
  { depth total left value minsum source newseen newlist oldlist size check index } ( depth sum -- )
  depth 7 = if
    total p103-bound !
    p103-current p103-best 7 cells move
    exit
  then
  7 depth - to left
  depth 0= if 1 else depth 1- p103-value@ 1+ then to value
  begin
    total left value * + left left 1- * 2 / + to minsum
    minsum p103-bound @ <
  while
    value depth cells p103-current + !
    depth 1+ p103-cardinal? if
      depth 512 * p103-seen + to oldlist
      depth 128 * cells p103-sums + to source
      1 depth lshift to size
      true to check
      size 0 do
        source i cells + @ value + oldlist + c@ 0<> if
          false to check leave
        then
      loop
      check if
        depth 1+ 512 * p103-seen + to newseen
        oldlist newseen 512 move
        depth 1+ 128 * cells p103-sums + to newlist
        source newlist size cells move
        size 0 do
          source i cells + @ value + to index
          1 newseen index + c!
          index newlist size i + cells + !
        loop
        depth 1+ total value + recurse
      then
    then
    value 1+ to value
  repeat ;

: p103 ( -- concatenation )
  20 0 cells p103-best + !
  11 18 19 20 22 25 0 0 0 0 { a b c d e f value index power result }
  0 p103-bound !
  a 20 + 1 cells p103-best + !
  b 20 + 2 cells p103-best + !
  c 20 + 3 cells p103-best + !
  d 20 + 4 cells p103-best + !
  e 20 + 5 cells p103-best + !
  f 20 + 6 cells p103-best + !
  7 0 do i cells p103-best + @ p103-bound @ + p103-bound ! loop
  p103-seen 4096 erase
  1 p103-seen c!
  0 0 cells p103-sums + !
  0 0 p103-search
  0 to result
  7 0 do
    i cells p103-best + @ to value
    10 to power
    begin power value <= while power 10 * to power repeat
    result power * value + to result
  loop result ;

\ Problem 84: 4-sided dice and the exact independent-draw Markov model.
\ Unlike the fixed cyclic-deck model, Chance/Chest cards are uniformly
\ reshuffled after drawing. Both reproduce the requested 3-square modal code.

[undefined] euler-testing [if] p103 . cr bye [then]
