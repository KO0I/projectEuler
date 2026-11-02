\ Project Euler 23: Non-Abundant Sums.
require math.fs

: p23 ( -- n )
  28124 allocate throw 28124 cells allocate throw 0 0 0
  { marked abundant count answer sum }
  marked 28124 erase
  28124 1 do i proper-divisor-sum i > if
    i abundant count cells + ! count 1+ to count
  then loop
  count 0 do
    count i do
      i abundant prefix@ j abundant prefix@ + to sum
      sum 28123 > if leave then
      1 marked sum + c!
    loop
  loop
  28124 1 do marked i + c@ 0= if answer i + to answer then loop
  marked free throw abundant free throw answer ;

[undefined] euler-testing [if] p23 . cr bye [then]
