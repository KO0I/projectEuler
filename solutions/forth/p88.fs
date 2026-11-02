\ Project Euler 88: Product-sum Numbers.
require math.fs

: p88-recurse { product sum count first minimum } ( product sum count first array -- )
  product sum - count + 12000 > if exit then
  product sum - count + dup 2 >= if
    dup minimum prefix@ product > if product swap minimum prefix! else drop then
  else drop then
  24000 product / 1+ first ?do
    product i * sum i + count 1+ i minimum recurse
  loop ;

: p88 ( -- n )
  12001 cells allocate throw 24001 allocate throw
  0 { minima seen answer }
  12001 0 do 24000 i minima prefix! loop
  seen 24001 erase
  1 0 0 2 minima p88-recurse
  12001 2 do i minima prefix@ dup seen + c@ 0= if
    dup seen + 1 swap c! answer + to answer
  else drop then loop
  minima free throw seen free throw answer ;

[undefined] euler-testing [if] p88 . cr bye [then]
