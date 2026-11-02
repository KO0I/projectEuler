\ Project Euler 22: Names Scores.
require math.fs

create names-address 5200 cells allot

create names-length 5200 cells allot

: name-addr ( index -- addr ) cells names-address + @ ;

: name-len ( index -- len ) cells names-length + @ ;

: name-swap 0 0 { a b addr len } ( index index -- )
  a name-addr to addr a name-len to len
  b name-addr a cells names-address + !
  b name-len a cells names-length + !
  addr b cells names-address + ! len b cells names-length + ! ;

: name-sort 0 0 0 0 { lo hi left right pivot-addr pivot-len } ( lo hi -- )
  lo hi >= if exit then lo to left hi to right
  lo hi + 2 / name-addr to pivot-addr lo hi + 2 / name-len to pivot-len
  begin left right <= while
    begin left hi <= if
      left name-addr left name-len pivot-addr pivot-len compare 0<
    else false then while left 1+ to left repeat
    begin right lo >= if
      right name-addr right name-len pivot-addr pivot-len compare 0>
    else false then while right 1- to right repeat
    left right <= if left right name-swap left 1+ to left right 1- to right then
  repeat
  lo right < if lo right recurse then left hi < if left hi recurse then ;

: name-value 0 { addr count sum } ( addr u -- score )
  0 to sum count 0 ?do addr i + c@ [char] A - 1+ sum + to sum loop sum ;

: p22 ( -- answer )
  s" names.txt" 60000 read-fixture 0 0 0 0 { buf count start inside found sum }
  0 to found false to inside 0 to sum
  count 0 do
    buf i + c@ [char] " = if
      inside if
        buf start + names-address found cells + !
        i start - names-length found cells + ! found 1+ to found false to inside
      else i 1+ to start true to inside then
    then
  loop
  found 5163 <> abort" p022 expected 5163 names"
  0 found 1- name-sort
  found 0 do i name-addr i name-len name-value i 1+ * sum + to sum loop
  buf free throw sum ;

[undefined] euler-testing [if] p22 . cr bye [then]
