\ Project Euler 79: Passcode Derivation.
require math.fs

: p79 ( -- n )
  s" tests/data/p079_keylog.txt" 150 fixture-integers
  100 allocate throw 10 allocate throw 10 allocate throw
  0 0 0 0 0 0 0 0 { triples count edges present emitted total next answer triple first second third available }
  edges 100 erase present 10 erase emitted 10 erase
  count 0 do
    i triples prefix@ to triple
    triple 100 / to first
    triple 10 / 10 mod to second
    triple 10 mod to third
    1 present first + c! 1 present second + c! 1 present third + c!
    1 edges first 10 * second + + c!
    1 edges second 10 * third + + c!
  loop
  10 0 do present i + c@ if total 1+ to total then loop
  total 0 do
    -1 to next
    10 0 do
      present i + c@ emitted i + c@ 0= and if
        true to available
        10 0 do
          edges i 10 * j + + c@ emitted i + c@ 0= and if false to available then
        loop
        available if i to next leave then
      then
    loop
    next 0< abort" Cyclic passcode constraints"
    answer 10 * next + to answer
    1 emitted next + c!
  loop
  triples free throw edges free throw present free throw emitted free throw answer ;

[undefined] euler-testing [if] p79 . cr bye [then]
