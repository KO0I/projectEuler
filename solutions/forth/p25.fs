\ Project Euler 25: 1000-digit Fibonacci Number.
require math.fs

: p25 ( -- answer )
  1 bnew 1 bnew 1 bnew 0 0 { a b temp old index }
  1 a bset-u 1 b bset-u 2 to index
  begin b bdecimal-length 1000 < while
    a b temp badd a to old b to a temp to b old to temp
    index 1+ to index
  repeat a bfree b bfree temp bfree index ;

\ Names scores: in-memory sort of quoted, comma-separated records.

[undefined] euler-testing [if] p25 . cr bye [then]
