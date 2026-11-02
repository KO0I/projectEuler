\ Project Euler 94: Almost Equilateral Triangles.
require math.fs

: p94 ( -- n ) 2 0 0 0 0 0 0 0 { x y nx ny perimeter a b sum }
  begin x 1000000002 <= while
    x 3 mod 2 = if x 1+ 3 / to a a 1+ to b x 2 + to perimeter
    else x 1- 3 / to a a 1- to b x 2 - to perimeter then
    a 1 > b 0> and perimeter 1000000000 <= and if
      b y * 4 mod 0= if sum perimeter + to sum then
    then
    x 2* y 3 * + to nx
    x y 2* + to ny
    nx to x ny to y
  repeat sum ;

[undefined] euler-testing [if] p94 . cr bye [then]
