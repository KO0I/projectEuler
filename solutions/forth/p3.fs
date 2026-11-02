\ Project Euler 3: Largest Prime Factor.
require math.fs

: p3 ( -- answer )
  600851475143 2 1 { n d last }
  begin d n d / <= while
    n d mod 0= if d to last n d / to n
    else d 2 = if 3 else d 2 + then to d then
  repeat n 1 > if n else last then ;

[undefined] euler-testing [if] p3 . cr bye [then]
