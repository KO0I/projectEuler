\ Project Euler 5: Smallest Multiple.
require math.fs

: p5 ( -- answer )
  1 { result }
  21 2 do result result i integer-gcd / i * to result loop result ;

[undefined] euler-testing [if] p5 . cr bye [then]
