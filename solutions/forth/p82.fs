\ Project Euler 82: Path Sum: Three Ways.
require math.fs

: p82 ( -- n )
  s" tests/data/p082_matrix.txt" 6400 fixture-integers
  80 cells allocate throw 0 0 0 { matrix count costs column row answer }
  count 6400 <> abort" Wrong matrix size"
  80 0 do i 80 * matrix prefix@ i costs prefix! loop
  80 1 do i to column
    80 0 do
      i 80 * column + matrix prefix@ i cells costs + +!
    loop
    80 1 do
      i 1- costs prefix@ i 80 * column + matrix prefix@ +
      i costs prefix@ min i costs prefix!
    loop
    79 0 do
      78 i - to row
      row 1+ costs prefix@ row 80 * column + matrix prefix@ +
      row costs prefix@ min row costs prefix!
    loop
  loop
  0 costs prefix@ to answer
  80 1 do answer i costs prefix@ min to answer loop
  matrix free throw costs free throw answer ;

[undefined] euler-testing [if] p82 . cr bye [then]
