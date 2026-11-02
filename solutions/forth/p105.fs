\ Project Euler 105: Special Subset Sums: Testing.
require math.fs

: p105-special? 0 0 0 0 0 0 { values count sums k small large size candidate }
  count 1 do
    i values prefix@ to candidate
    i to size
    begin size 0> if size 1- values prefix@ candidate > else false then while
      size 1- values prefix@ size values prefix!
      size 1- to size
    repeat
    candidate size values prefix!
  loop
  count 1 do
    i 2* 1+ count <= if
      0 to small 0 to large
      i 1+ 0 do i values prefix@ small + to small loop
      i 0 do count 1- i - values prefix@ large + to large loop
      small large <= if false unloop exit then
    then
  loop
  4096 cells allocate throw to sums sums 4096 cells erase
  1 to size
  count 0 do
    size 0 do
      i sums prefix@ j values prefix@ + size i + sums prefix!
    loop
    size 2* to size
  loop
  size 1 do
    i sums prefix@ to candidate
    i to k
    begin k 0> if k 1- sums prefix@ candidate > else false then while
      k 1- sums prefix@ k sums prefix!
      k 1- to k
    repeat
    candidate k sums prefix!
  loop
  size 1 do
    i sums prefix@ i 1- sums prefix@ = if sums free throw false unloop exit then
  loop sums free throw true ;

: p105 ( -- n )
  s" tests/data/p105_sets.txt" 30000 read-fixture
  12 cells allocate throw 0 0 0 0 0 0 { raw length values total count value active element line-sum }
  0 to count 0 to value false to active 0 to total
  length 0 do
    raw i + c@ dup [char] 0 [char] 9 1+ within if
      [char] 0 - value 10 * + to value true to active
    else
      dup to element drop
      active if
        count 12 >= abort" Too many set elements"
        value count values prefix! count 1+ to count
        value line-sum + to line-sum
        0 to value false to active
      then
      element 10 = if
        count 0> if
          values count p105-special? if line-sum total + to total then
          0 to count 0 to line-sum
        then
      then
    then
  loop
  active if
    count 12 >= abort" Too many set elements"
    value count values prefix! count 1+ to count
    value line-sum + to line-sum
  then
  count 0> if values count p105-special? if total line-sum + to total then then
  raw free throw values free throw total ;

[undefined] euler-testing [if] p105 . cr bye [then]
