\ Project Euler 91: Right Triangles with Integer Coordinates.
require math.fs

: p91 ( -- n ) 0 0 0 0 0 0 0 0 { answer x1 y1 x2 y2 dot0 dot1 dot2 }
  51 0 do i to x1
    51 0 do i to y1
      x1 0<> y1 0<> or if
        51 0 do i to x2
          51 0 do i to y2
            x2 0<> y2 0<> or x1 x2 <> y1 y2 <> or and if
              x1 x2 * y1 y2 * + to dot0
              x1 x1 x2 - * y1 y1 y2 - * + to dot1
              x2 x2 x1 - * y2 y2 y1 - * + to dot2
              dot0 0= dot1 0= or dot2 0= or if answer 1+ to answer then
            then
          loop
        loop
      then
    loop
  loop answer 2 / ;

[undefined] euler-testing [if] p91 . cr bye [then]
