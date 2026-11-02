\ Project Euler 102: Triangle Containment.
require math.fs

: signed-fixture 0 0 0 0 0 0 0 { addr u cap raw length numbers count value negative active } ( c-addr u limit -- array count )
  addr u cap 16 * read-fixture to length to raw
  cap cells allocate throw to numbers
  length 0 do
    raw i + c@ dup [char] - = if
      negative 0= active 0= and if -1 to negative then
      drop
    else dup [char] 0 [char] 9 1+ within if
      [char] 0 - value 10 * + to value true to active
    else drop
      active if
        count cap >= abort" Signed fixture too long"
        value negative 0< if negate then count numbers prefix!
        count 1+ to count
        0 to value 0 to negative false to active
      else 0 to negative then
    then then
  loop
  active if value negative 0< if negate then count numbers prefix! count 1+ to count then
  raw free throw numbers count ;

: p102 ( -- n )
  s" tests/data/p102_triangles.txt" 6000 signed-fixture
  0 0 0 0 0 0 0 0 0 0 { coords count answer a b c d e f cross1 cross2 cross3 }
  count 6000 <> abort" Invalid triangle fixture size"
  6000 0 do i 6 mod 0= if
    i coords prefix@ to a i 1+ coords prefix@ to b
    i 2 + coords prefix@ to c i 3 + coords prefix@ to d
    i 4 + coords prefix@ to e i 5 + coords prefix@ to f
    a d * b c * - to cross1
    c f * d e * - to cross2
    e b * f a * - to cross3
    cross1 0> cross2 0> and cross3 0> and
    cross1 0< cross2 0< and cross3 0< and or if
      answer 1+ to answer
    then
  then loop
  coords free throw answer ;

[undefined] euler-testing [if] p102 . cr bye [then]
