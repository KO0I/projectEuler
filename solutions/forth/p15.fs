\ Project Euler 15: Lattice Paths.
require math.fs

: p15 ( -- n )
  21 cells allocate throw { ways }
  21 0 do 1 ways i cells + ! loop
  20 0 do 21 1 do ways i 1- cells + @ ways i cells + +! loop loop
  20 ways prefix@ ways free throw ;

[undefined] euler-testing [if] p15 . cr bye [then]
