\ Project Euler 107: Minimal Network.
require math.fs

: p107 ( -- n )
  s" tests/data/p107_network.txt" 8000 read-fixture
  1600 cells allocate throw 40 cells allocate throw 40 allocate throw
  0 0 0 0 0 0 0 0 0
  { raw length edges distance chosen count number active char total cost best vertex candidate }
  0 to count 0 to number false to active
  length 0 do
    raw i + c@ to char
    char [char] 0 [char] 9 1+ within if
      char [char] 0 - number 10 * + to number true to active
    else char [char] - = if
      true to active 0 to number
    else active if
      count 1600 >= abort" Network too large"
      number count edges prefix! count 1+ to count
      false to active 0 to number
    then then then
  loop
  active if number count edges prefix! count 1+ to count then
  count 1600 <> abort" Network requires 40x40 values"
  40 0 do
    40 i 1+ ?do
      j 40 * i + edges prefix@ total + to total
    loop
  loop
  chosen 40 erase
  40 0 do 1000000 i distance prefix! loop
  0 0 distance prefix!
  40 0 do
    1000000 to best -1 to vertex
    40 0 do
      chosen i + c@ 0= if i distance prefix@ best < if
        i distance prefix@ to best i to vertex
      then then
    loop
    vertex 0< abort" Graph disconnected"
    best cost + to cost
    1 chosen vertex + c!
    40 0 do
      vertex 40 * i + edges prefix@ to candidate
      candidate 0> chosen i + c@ 0= and if
        candidate i distance prefix@ < if candidate i distance prefix! then
      then
    loop
  loop
  raw free throw edges free throw distance free throw chosen free throw
  total cost - ;

[undefined] euler-testing [if] p107 . cr bye [then]
