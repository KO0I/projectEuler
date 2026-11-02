\ Project Euler 2: Even Fibonacci Numbers.
require math.fs

: p2 ( -- answer )
  1 2 0 0 { a b sum next }
  begin b 4000000 <= while
    b 2 mod 0= if sum b + to sum then
    a b + to next b to a next to b
  repeat sum ;

[undefined] euler-testing [if] p2 . cr bye [then]
