USING: io kernel locals math math.functions math.order math.parser ranges sequences io.encodings.utf8 io.files splitting ;
IN: scratchpad

! Search all four legal directions of a 20-by-20 grid.
! The input separates rows with LF as well as spaces. Split on *all*
! ASCII whitespace; otherwise the last cell of one row merges with the first
! cell of the next. Accumulate the zero-padded cells explicitly in base 10.
: decimal-cell ( str -- n ) 0 [ 48 - swap 10 * + ] reduce ;
: read-grid ( -- seq )
    "tests/data/p011.txt" utf8 file-contents
    [ { 32 10 13 9 } member? ] split-when
    [ empty? not ] filter
    [ decimal-cell ] map ;

:: grid-product ( grid x y dx dy -- n )
    1 :> result!
    0 3 [a..b] [| step |
        y step dy * + 20 *
        x step dx * + +
        grid nth result * result!
    ] each
    result ;

:: p11 ( -- n )
    read-grid :> grid
    0 :> best!
    { { 1 0 } { 0 1 } { 1 1 } { -1 1 } } :> directions
    0 19 [a..b] [| y |
        0 19 [a..b] [| x |
            directions [| d |
                0 d nth :> dx
                1 d nth :> dy
                x 3 dx * + :> xx
                y 3 dy * + :> yy
                xx 0 >= xx 20 < and
                yy 0 >= and yy 20 < and [
                    grid x y dx dy grid-product :> candidate
                    candidate best > [ candidate best! ] when
                ] when
            ] each
        ] each
    ] each
    best ;
p11 number>string print
