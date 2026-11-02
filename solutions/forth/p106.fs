\ Project Euler 106: Special Subset Sums: Meta-testing.
require math.fs

: p106 ( -- n ) 0 0 0 { total middle catalan }
  7 2 do
    i 2* i choose to middle
    middle i 1+ / to catalan
    12 i 2* choose middle 2 / catalan - * total + to total
  loop total ;

[undefined] euler-testing [if] p106 . cr bye [then]
