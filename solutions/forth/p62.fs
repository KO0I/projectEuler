\ Project Euler 62: Cubic Permutations.
require math.fs

: p62-test 0 { signatures values count copies } ( addresses length -- minimum or zero )
  count 0 do
    0 to copies
    count 0 do
      i signatures prefix@ j signatures prefix@ = if copies 1+ to copies then
    loop
    copies 5 = if i values prefix@ unloop exit then
  loop 0 ;

: p62 ( -- n )
  15000 cells allocate throw 15000 cells allocate throw
  0 10 1 0 0 { signatures values count bucket n cube answer }
  begin
    n n * n * to cube
    cube bucket >= if
      signatures values count p62-test to answer
      answer 0<> if signatures free throw values free throw answer exit then
      bucket 10 * to bucket 0 to count
    then
    cube digit-signature count signatures prefix!
    cube count values prefix! count 1+ to count
    n 1+ to n
    count 15000 >= abort" Cubic permutation bucket capacity"
  again ;

[undefined] euler-testing [if] p62 . cr bye [then]
