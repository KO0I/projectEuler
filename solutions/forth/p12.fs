\ Project Euler 12: Highly Divisible Triangular Number.
require math.fs

: p12 ( -- answer )
  1 0 { n tri }
  begin n n 1+ * 2 / to tri
    tri divisor-count 500 > if tri exit then
    n 1+ to n
  again ;

[undefined] euler-testing [if] p12 . cr bye [then]
