USING: io kernel locals math math.functions math.order math.parser ranges sequences io.encodings.utf8 io.files splitting ;
IN: scratchpad

! Search all four legal directions of a 20-by-20 grid.
: read-grid ( -- seq )
    "tests/data/p011.txt" utf8 file-contents
    split-words [ string>number ] map ;

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
