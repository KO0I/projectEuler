\ Project Euler 93: Arithmetic Expressions.
require math.fs

defer p93-expression

create p93-num 16 cells allot

create p93-den 16 cells allot

create p93-reached 10000 allot

: p93-n@ { depth index } ( depth index -- n ) depth 4 * index + cells p93-num + @ ;

: p93-d@ { depth index } ( depth index -- d ) depth 4 * index + cells p93-den + @ ;

: p93-set { n d depth index } ( n d depth index -- )
  n depth 4 * index + cells p93-num + !
  d depth 4 * index + cells p93-den + ! ;

: p93-rational 0 { n d factor } ( n d -- reduced-n reduced-d )
  d 0< if n negate to n d negate to d then
  n abs d integer-gcd to factor
  n factor / d factor / ;

: p93-test { n d depth size } ( n d depth count -- )
  n d p93-rational to d to n
  n d depth size 2 - p93-set
  depth size 1- p93-expression ;

:noname 0 0 0 0 0 0 0 0
  { depth size a b c d next other one two } ( depth count -- )
  size 1 = if
    depth 0 p93-n@ to a depth 0 p93-d@ to b
    b 1 = a 0> and a 10000 < and if 1 p93-reached a + c! then
    exit
  then
  size 0 do
    i to one
    size i 1+ ?do
      i to two
      depth one p93-n@ to a depth one p93-d@ to b
      depth two p93-n@ to c depth two p93-d@ to d
      0 to next
      size 0 do
        i one <> i two <> and if
          depth i p93-n@ depth i p93-d@ depth 1+ next p93-set
          next 1+ to next
        then
      loop
      a d * c b * + b d * depth 1+ size p93-test
      a d * c b * - b d * depth 1+ size p93-test
      c b * a d * - b d * depth 1+ size p93-test
      a c * b d * depth 1+ size p93-test
      c 0<> if a d * b c * depth 1+ size p93-test then
      a 0<> if c b * d a * depth 1+ size p93-test then
    loop
  loop ; is p93-expression

: p93-run 0 0 { a b c d position answer } ( a b c d -- consecutive )
  p93-reached 10000 erase
  a 1 0 0 p93-set b 1 0 1 p93-set
  c 1 0 2 p93-set d 1 0 3 p93-set
  0 4 p93-expression
  1 to position
  begin position 10000 < if p93-reached position + c@ 0<> else false then while
    position 1+ to position
  repeat position 1- ;

: p93 0 0 0 0 0 0 { a b c d best answer } ( -- digits )
  0 to best 0 to answer
  7 0 do i to a
    8 a 1+ do i to b
      9 b 1+ do i to c
        10 c 1+ do i to d
          a b c d p93-run best > if
            a b c d p93-run to best
            a 1000 * b 100 * + c 10 * + d + to answer
          then
        loop
      loop
    loop
  loop answer ;

\ Problem 103: prove the constructed seven-element bound by exhaustive
\ increasing-set search with subset-collision and cardinality pruning.

[undefined] euler-testing [if] p93 . cr bye [then]
