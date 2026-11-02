\ Project Euler 86: Cuboid Route.
require math.fs

: p86 ( -- n ) 0 0 0 0 0 0 { total largest squared root low high }
  1 to largest
  begin largest 2* 1+ 2 do
      largest largest * i i * + to squared
      squared integer-sqrt to root
      root root * squared = if
        i largest > if i largest - else 1 then to low
        i 2 / largest min to high
        high low >= if total high low - 1+ + to total then
      then
    loop
    total 1000000 > if largest exit then
    largest 1+ to largest
  again ;

[undefined] euler-testing [if] p86 . cr bye [then]
