\ Project Euler 69: Totient Maximum.
require math.fs

: p69 ( -- answer )
  1 2 { result candidate }
  begin candidate is-prime? if
    result 1000000 candidate / > if result exit then result candidate * to result
  then candidate 1+ to candidate again ;

[undefined] euler-testing [if] p69 . cr bye [then]
