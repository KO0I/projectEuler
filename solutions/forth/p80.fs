\ Project Euler 80: Square Root Digital Expansion.
require math.fs

: p80-root-digits 0 0 0 0 0 0 { number remainder root trial answer digit found } ( n -- digit-sum )
  12 bnew to remainder 12 bnew to root 12 bnew to trial
  0 remainder bset-u 0 root bset-u
  100 0 do
    100 remainder bmul-small!
    i 0= if number remainder badd-small! then
    false to found
    10 0 do
      9 i - to digit
      root trial bcopy
      20 trial bmul-small!
      digit trial badd-small!
      digit trial bmul-small!
      remainder trial babscompare 0>= if
        remainder trial bsubabs!
        10 root bmul-small!
        digit root badd-small!
        answer digit + to answer
        true to found leave
      then
    loop
    found 0= abort" Square-root digit extraction failed"
  loop
  remainder bfree root bfree trial bfree answer ;

: p80 ( -- n ) 0 { total }
  101 2 do
    i integer-sqrt dup * i <> if i p80-root-digits total + to total then
  loop total ;

[undefined] euler-testing [if] p80 . cr bye [then]
