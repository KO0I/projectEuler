\ Project Euler 39: Integer Right Triangles.
require math.fs

: p39 ( -- answer )
  0 0 0 0 0 0 0 { best-perimeter best-count count numerator denominator b c }
  1001 12 do 0 to count
    i 3 / 1 do
      j j i 2 * - * to numerator 2 j i - * to denominator
      numerator 0> numerator denominator mod 0= and if
        numerator denominator / to b j i - b - to c
        i b < b c < and if count 1+ to count then
      then
    loop count best-count > if count to best-count i to best-perimeter then
  loop best-perimeter ;

[undefined] euler-testing [if] p39 . cr bye [then]
