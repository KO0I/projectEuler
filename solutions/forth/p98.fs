\ Project Euler 98: Anagramic Squares.
require math.fs

create p98-words 1800 cells allot

create p98-lengths 1800 cells allot

create p98-signatures 46800 allot

create p98-letters 26 cells allot

create p98-used 10 cells allot

variable p98-count

: p98-word@ ( index -- address ) cells p98-words + @ ;

: p98-length@ ( index -- n ) cells p98-lengths + @ ;

: p98-signature ( index -- address ) 26 * p98-signatures + ;

: p98-power 0 { exponent value } ( k -- 10^k )
  1 to value exponent 0 do value 10 * to value loop value ;

: p98-load 0 0 0 0 0 { raw length count position word-start letter signature } ( ptr size -- )
  0 to count 0 to position
  p98-signatures 46800 erase
  begin position length < while
    raw position + c@ [char] " <> abort" Invalid quoted word"
    position 1+ to position
    raw position + count cells p98-words + !
    position to word-start
    count p98-signature to signature
    begin position length < if raw position + c@ [char] " <> else false then while
      raw position + c@ [char] A - to letter
      letter 0< letter 25 > or abort" Invalid anagram letter"
      signature letter + c@ 1+ signature letter + c!
      position 1+ to position
    repeat
    position word-start - count cells p98-lengths + !
    count 1+ to count
    position 1+ to position
    position length < if
      raw position + c@ [char] , <> abort" Invalid anagram separator"
      position 1+ to position
    then
  repeat
  count 1786 <> abort" Incomplete anagram word fixture"
  count p98-count ! ;

: p98-pair 0 0 0 0 0 0 0 0 0 0 0
  { first second length lower upper root square letter digit power other consistent best }
  ( word-index word-index -- largest square )
  first p98-length@ to length
  length 1- p98-power integer-sqrt to lower
  lower lower * length 1- p98-power < if lower 1+ to lower then
  length p98-power 1- integer-sqrt to upper
  0 to best
  lower to root
  begin root upper <= while
    root root * to square
    p98-letters 26 cells 255 fill
    p98-used 10 cells 255 fill
    true to consistent
    length 0 do
      first p98-word@ i + c@ [char] A - to letter
      length i - 1- p98-power to power
      square power / 10 mod to digit
      letter cells p98-letters + @ -1 <> if
        letter cells p98-letters + @ digit <> if false to consistent leave then
      else
        digit cells p98-used + @ -1 <> if false to consistent leave then
        letter digit cells p98-used + !
        digit letter cells p98-letters + !
      then
    loop
    consistent if
      second p98-word@ c@ [char] A - cells p98-letters + @ 0<> if
        0 to other
        length 0 do
          second p98-word@ i + c@ [char] A - cells p98-letters + @ to digit
          other 10 * digit + to other
        loop
        other integer-sqrt dup * other = if
          square other max best max to best
        then
      then
    then
    root 1+ to root
  repeat best ;

: p98 ( -- largest-square )
  s" tests/data/p098_words.txt" 18000 read-fixture
  0 0 0 { raw length answer count i }
  raw length p98-load
  0 to answer
  p98-count @ 0 do
    i 1+ p98-count @ do
      i p98-length@ j p98-length@ = if
        i p98-signature 26 j p98-signature 26 compare 0= if
          j i p98-pair answer max to answer
        then
      then
    loop
  loop
  raw free throw answer ;

[undefined] euler-testing [if] p98 . cr bye [then]
