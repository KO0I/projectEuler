\ Project Euler 24: Lexicographic Permutations.
require math.fs

: p24 ( -- n )
  10 cells allocate throw 999999 0 0 0 { digits rank result index remaining }
  10 0 do i digits i cells + ! loop
  10 0 do
    10 i - to remaining
    rank remaining 1- factorial / to index
    rank remaining 1- factorial mod to rank
    result 10 * index digits prefix@ + to result
    remaining 1- index ?do
      i 1+ digits prefix@ i digits prefix!
    loop
  loop digits free throw result ;

[undefined] euler-testing [if] p24 . cr bye [then]
