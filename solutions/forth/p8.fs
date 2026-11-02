\ Project Euler 8: Largest Product in a Series.
require math.fs

: p8 ( -- answer )
  s" tests/data/p008.txt" 1200 read-fixture 0 0 0 0 { buffer length digits count best product }
  1000 allocate throw to digits
  0 to count 0 to best
  length 0 do
    buffer i + c@ dup [char] 0 >= over [char] 9 <= and if
      [char] 0 - digits count + c! count 1+ to count
    else drop then
  loop
  count 1000 <> abort" p008 digit fixture must have 1000 digits"
  988 0 do
    1 to product
    13 0 do product digits j i + + c@ * to product loop
    product best > if product to best then
  loop buffer free throw digits free throw best ;

[undefined] euler-testing [if] p8 . cr bye [then]
