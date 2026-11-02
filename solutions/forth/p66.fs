\ Project Euler 66: Diophantine Equation.
require math.fs

: p66-period 0 0 0 0 0 { n root m denominator a period } ( D -- period )
  n integer-sqrt to root
  0 to m 1 to denominator root to a 0 to period
  begin
    denominator a * m - to m
    n m m * - denominator / to denominator
    root m + denominator / to a
    period 1+ to period
    a root 2* =
  until period ;

: p66-numerator 0 0 0 0 0 0 0 0 0 0
  { n answer p0 p1 next old root m denominator a period steps } ( D big-result -- )
  8 bnew to p0 8 bnew to p1 8 bnew to next
  0 p0 bset-u 1 p1 bset-u
  n integer-sqrt to root
  n p66-period to period
  period 2 mod if period 2* else period then to steps
  0 to m 1 to denominator root to a
  steps 0 do
    i 0> if
      denominator a * m - to m
      n m m * - denominator / to denominator
      root m + denominator / to a
    then
    p1 next bcopy
    a next bmul-small!
    next p0 baddabs!
    p0 to old p1 to p0 next to p1 old to next
  loop
  p1 answer bcopy p0 bfree p1 bfree next bfree ;

: p66 ( -- n )
  8 bnew 8 bnew 0 { best value answer }
  1001 2 do
    i integer-sqrt dup * i <> if
      i value p66-numerator
      value best babscompare 0> if
        value best bcopy i to answer
      then
    then
  loop best bfree value bfree answer ;

[undefined] euler-testing [if] p66 . cr bye [then]
