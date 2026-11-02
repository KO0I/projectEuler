\ Project Euler 81: Path Sum: Two Ways.
require math.fs

: p81 ( -- n )
  s" tests/data/p081_matrix.txt" 6400 fixture-integers
  80 cells allocate throw 0 0 0 { matrix count costs row col answer }
  count 6400 <> abort" Wrong matrix size"
  costs 80 cells erase
  80 0 do i to row
    80 0 do i to col
      row 0= col 0> and if col 1- costs prefix@ col costs prefix! then
      row 0> col 0> and if
        col costs prefix@ col 1- costs prefix@ min col costs prefix!
      then
      row 80 * col + matrix prefix@ col cells costs + +!
    loop
  loop
  79 costs prefix@ to answer
  matrix free throw costs free throw answer ;

[undefined] euler-testing [if] p81 . cr bye [then]
