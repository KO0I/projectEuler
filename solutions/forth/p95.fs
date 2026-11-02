\ Project Euler 95: Amicable Chains.
require math.fs

: p95 ( -- n )
  1000001 cells allocate throw 1000001 cells allocate throw 1000001 cells allocate throw
  0 0 0 0 0 0 0 0 { successors visited depth best-length best-member start number step minimum first cycle-start }
  successors 1000001 cells erase visited 1000001 cells erase
  500001 1 do
    1000001 i 2* do
      j i successors prefix@ + i successors prefix!
    j +loop
  loop
  1000001 2 do i to start
    start visited prefix@ 0= if
      start to number 0 to step
      begin number 0> number 1000000 <= and if number visited prefix@ 0= else false then while
        start number visited prefix!
        step number depth prefix!
        step 1+ to step
        number successors prefix@ to number
      repeat
      number 0> number 1000000 <= and if number visited prefix@ start = else false then if
        step number depth prefix@ - to first
        number to minimum number to cycle-start
        number successors prefix@ to number
        begin number cycle-start <> while
          number minimum min to minimum
          number successors prefix@ to number
        repeat
        first best-length > if first to best-length minimum to best-member else
          first best-length = minimum best-member < and if minimum to best-member then
        then
      then
    then
  loop
  successors free throw visited free throw depth free throw best-member ;

[undefined] euler-testing [if] p95 . cr bye [then]
