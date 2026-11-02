\ Project Euler 14: Longest Collatz Sequence.
require math.fs

: p14 ( -- answer )
  1 0 0 0 { best-start best-length n length }
  1000000 1 do
    i to n 1 to length
    begin n 1 <> while
      n 2 mod 0= if n 2 / to n else n 3 * 1+ to n then
      length 1+ to length
    repeat
    length best-length > if length to best-length i to best-start then
  loop best-start ;

[undefined] euler-testing [if] p14 . cr bye [then]
