\ Project Euler 45: Triangular, Pentagonal, and Hexagonal.
require math.fs

: p45 ( -- n ) 144 { index }
  begin index index 2* 1- * dup pentagonal? if exit then drop index 1+ to index again ;

[undefined] euler-testing [if] p45 . cr bye [then]
