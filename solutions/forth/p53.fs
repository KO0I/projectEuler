\ Project Euler 53: Combinatoric Selections.
require math.fs

: p53 ( -- n )
  101 cells allocate throw 0 0 { row count value }
  row 101 cells erase 1 row !
  101 1 do
    i 1+ 1 do
      j i - 1+ to value
      value row prefix@ value 1- row prefix@ + 1000001 min
      value row prefix!
    loop
    i 1+ 0 do i row prefix@ 1000000 > if count 1+ to count then loop
  loop row free throw count ;

[undefined] euler-testing [if] p53 . cr bye [then]
