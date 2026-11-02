\ Project Euler 83: Path Sum: Four Ways.
require math.fs

: p83-relax 0 { vertex costs distance current candidate } ( index costs dist value -- )
  current vertex costs prefix@ + to candidate
  candidate vertex distance prefix@ < if candidate vertex distance prefix! then ;

: p83 ( -- n )
  s" tests/data/p083_matrix.txt" 6400 fixture-integers
  6400 cells allocate throw 6400 allocate throw
  0 0 0 0 0 { costs count distance used best vertex row col candidate }
  count 6400 <> abort" Matrix requires 6400 values"
  used 6400 erase
  6400 0 do 100000000 i distance prefix! loop
  0 costs prefix@ 0 distance prefix!
  6400 0 do
    100000000 to best -1 to vertex
    6400 0 do
      used i + c@ 0= if
        i distance prefix@ best < if
          i to vertex i distance prefix@ to best
        then
      then
    loop
    vertex 0< abort" Disconnected matrix graph"
    1 used vertex + c!
    vertex 6399 = if
      costs free throw distance free throw used free throw best unloop exit
    then
    vertex 80 / to row vertex 80 mod to col
    col 0> if vertex 1- costs distance best p83-relax then
    col 79 < if vertex 1+ costs distance best p83-relax then
    row 0> if vertex 80 - costs distance best p83-relax then
    row 79 < if vertex 80 + costs distance best p83-relax then
  loop -1 ;

[undefined] euler-testing [if] p83 . cr bye [then]
