\ Project Euler 52: Permuted Multiples.
require math.fs

: p52 ( -- n ) 1 9 0 0 { lower upper signature candidate }
  begin upper 6 / 1+ lower ?do
      i digit-signature to signature
      true 7 2 do signature i j * digit-signature = and loop
      if i unloop exit then
    loop upper 1+ to lower upper 10 * 9 + to upper
  again ;

[undefined] euler-testing [if] p52 . cr bye [then]
