\ Project Euler 28: Number Spiral Diagonals.
require math.fs

: p28 ( -- answer )
  1 { result }
  1002 3 do i 2 mod 1 = if
    i i * 4 * i 1- 6 * - result + to result
  then loop result ;

[undefined] euler-testing [if] p28 . cr bye [then]
