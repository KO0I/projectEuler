\ Project Euler 85: Counting Rectangles.
require math.fs

: p85 ( -- n ) 2000000 0 0 0 0 0 { closest best width height rectangles diff }
  2001 1 do i to width
    width 1+ 1 do i to height
      width width 1+ * height height 1+ * * 4 / to rectangles
      rectangles 2000000 - abs to diff
      diff closest < if diff to closest width height * to best then
      rectangles 2100000 > if leave then
    loop
  loop best ;

[undefined] euler-testing [if] p85 . cr bye [then]
