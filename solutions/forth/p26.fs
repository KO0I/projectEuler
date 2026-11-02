\ Project Euler 26: Reciprocal Cycles.
require math.fs

: p26 ( -- n )
  1000 cells allocate throw 0 0 0 0 0 { seen best answer remainder position cycle }
  1000 2 do
    i 0 do -1 i seen prefix! loop
    1 i mod to remainder 0 to position
    begin remainder 0<> if remainder seen prefix@ -1 = else false then while
      position remainder seen prefix!
      position 1+ to position
      remainder 10 * i mod to remainder
    repeat
    remainder 0= if 0 else position remainder seen prefix@ - then to cycle
    cycle best > if cycle to best i to answer then
  loop seen free throw answer ;

[undefined] euler-testing [if] p26 . cr bye [then]
