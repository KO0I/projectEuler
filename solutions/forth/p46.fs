\ Project Euler 46: Goldbach's Other Conjecture.
require math.fs

: p46 ( -- answer )
  9 0 0 0 { n representation half root }
  begin n is-prime? 0= if
    false to representation
    n 2 do i is-prime? if n i - 2 mod 0= if
      n i - 2 / to half half integer-sqrt to root
      root root * half = if true to representation leave then
    then then loop
    representation 0= if n exit then
  then n 2 + to n again ;

[undefined] euler-testing [if] p46 . cr bye [then]
