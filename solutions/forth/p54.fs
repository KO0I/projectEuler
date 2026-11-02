\ Project Euler 54: Poker Hands.
require math.fs

: p54-rank { char } ( ascii -- rank )
  char [char] 2 >= char [char] 9 <= and if char [char] 0 - exit then
  char case [char] T of 10 endof [char] J of 11 endof
    [char] Q of 12 endof [char] K of 13 endof [char] A of 14 endof
    abort" Invalid card rank" endcase ;

: p54-value 0 0 0 0 0 0 0 0 0 0 0 0 0
  { text counts ordered pairs keys suit flush unique trip four pair-count straight category rank } ( ptr -- rank value )
  15 cells allocate throw to counts
  5 cells allocate throw to ordered
  2 cells allocate throw to pairs
  5 cells allocate throw to keys
  counts 15 cells erase keys 5 cells erase
  true to flush 0 to unique 0 to trip 0 to four 0 to pair-count 0 to straight
  text 1+ c@ to suit
  5 0 do
    text i 3 * + c@ p54-rank to rank
    rank counts prefix@ 1+ rank counts prefix!
    text i 3 * 1+ + c@ suit <> if false to flush then
  loop
  1 14 do
    i counts prefix@ 0> if
      i unique ordered prefix! unique 1+ to unique
      i counts prefix@ 2 = if i pair-count pairs prefix! pair-count 1+ to pair-count then
      i counts prefix@ 3 = if i to trip then
      i counts prefix@ 4 = if i to four then
    then
  -1 +loop
  unique 5 = if
    0 ordered prefix@ 4 ordered prefix@ - 4 = if 0 ordered prefix@ to straight else
      14 counts prefix@ 2 counts prefix@ and 3 counts prefix@ and
      4 counts prefix@ and 5 counts prefix@ and if 5 to straight then
    then
  then
  0 to category
  straight 0> flush and if 8 to category straight 0 keys prefix! else
    four 0> if 7 to category four 0 keys prefix!
      15 2 do i counts prefix@ 1 = if i 1 keys prefix! then loop
    else trip 0> pair-count 1 = and if
      6 to category trip 0 keys prefix! 0 pairs prefix@ 1 keys prefix!
    else flush if
      5 to category 5 0 do i ordered prefix@ i keys prefix! loop
    else straight 0> if 4 to category straight 0 keys prefix!
    else trip 0> if
      3 to category trip 0 keys prefix! 1 to unique
      1 14 do i counts prefix@ 1 = if i unique keys prefix! unique 1+ to unique then -1 +loop
    else pair-count 2 = if
      2 to category 0 pairs prefix@ 0 keys prefix! 1 pairs prefix@ 1 keys prefix!
      15 2 do i counts prefix@ 1 = if i 2 keys prefix! then loop
    else pair-count 1 = if
      1 to category 0 pairs prefix@ 0 keys prefix! 1 to unique
      1 14 do i counts prefix@ 1 = if i unique keys prefix! unique 1+ to unique then -1 +loop
    else 5 0 do i ordered prefix@ i keys prefix! loop
    then then then then then then then
  then
  5 0 do category 15 * i keys prefix@ + to category loop
  counts free throw ordered free throw pairs free throw keys free throw category ;

: p54 ( -- n )
  s" tests/data/p054_poker.txt" 30000 read-fixture
  0 { text length answer }
  length 30000 <> abort" Invalid poker dataset size"
  1000 0 do
    text i 30 * + p54-value
    text i 30 * 15 + + p54-value
    > if answer 1+ to answer then
  loop text free throw answer ;

[undefined] euler-testing [if] p54 . cr bye [then]
