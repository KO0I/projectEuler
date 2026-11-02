! Shared higher-level Factor Euler solvers. All compute their results at runtime;
! no solution returns the reference answer as a literal.
USING: arrays assocs combinators hashtables euler.shared io.encodings.utf8
io.files kernel locals math math.bitwise math.functions math.order math.parser
math.primes math.ratios ranges sequences sets sorting splitting strings vectors ;
IN: euler.advanced

CONSTANT: english-units { 0 3 3 5 4 4 3 5 5 4 3 6 6 8 8 7 7 9 8 8 }
CONSTANT: english-tens { 0 0 6 6 5 5 5 7 6 6 }
:: english-count ( n -- length )
    n 1000 = [ 11 ] [
        0 :> result!
        n 100 >= [
            n 100 /i english-units nth 7 + result!
            n 100 mod zero? not [ result 3 + result! ] when
        ] when
        n 100 mod :> rest
        rest 20 < [ result rest english-units nth + result! ] [
            rest 10 /i english-tens nth result + result!
            rest 10 mod english-units nth result + result!
        ] if result
    ] if ;
: p17 ( -- n ) 1 1000 [a..b] [ english-count ] map sum ;

:: mask-digits ( value length -- mask unique? )
    0 :> mask! t :> valid!
    0 length 1 - [a..b] [| k |
        value 10 k ^ /i 10 mod :> digit
        2 digit ^ :> bit
        mask bit /i 2 mod zero?
        [ mask bit + mask! ] [ f valid! ] if
    ] each mask valid ;
:: p43-extend ( suffix length phase -- total )
    suffix length mask-digits :> valid :> mask
    valid not [ 0 ] [
        phase 6 = [
            0 :> total!
            1 9 [a..b] [| d |
                mask 2 d ^ /i 2 mod zero? [
                    d 10 length ^ * suffix + total + total!
                ] when
            ] each total
        ] [
            phase { 13 11 7 5 3 2 } nth :> divisor
            0 :> total!
            0 9 [a..b] [| digit |
                mask 2 digit ^ /i 2 mod zero? [
                    digit 100 * suffix 10 length 2 - ^ /i 100 mod +
                    divisor mod zero? [
                        digit 10 length ^ * suffix + length 1 + phase 1 +
                        p43-extend total + total!
                    ] when
                ] when
            ] each total
        ] if
    ] if ;
:: p43 ( -- n )
    0 :> result!
    0 999 [a..b] [| seed |
        seed 17 mod zero? [ seed 3 0 p43-extend result + result! ] when
    ] each result ;

:: p44 ( -- n )
    0 :> best!
    2 4000 [a..b] [| k |
        k 3 k * 1 - * 2 /i :> large
        1 k 1 - [a..b] [| j |
            j 3 j * 1 - * 2 /i :> small
            large small - :> gap
            best zero? gap best < or [
                gap 5 polygonal? large small + 5 polygonal? and
                [ gap best! ] when
            ] when
        ] each
    ] each best ;

:: p50 ( -- n )
    999999 euler-sieve :> sieve
    V{ } clone :> primes
    2 999999 [a..b] [| n | n sieve nth [ n primes push ] when ] each
    primes length 1 + 0 <array> :> prefix
    0 0 prefix set-nth
    1 primes length [a..b] [| i |
        i 1 - prefix nth i 1 - primes nth + i prefix set-nth
    ] each
    0 :> bestlen! 0 :> bestvalue!
    0 primes length 1 - [a..b] [| a |
        a bestlen + 1 + :> b!
        [ b primes length <= ] [
            b prefix nth a prefix nth - :> value
            value 1000000 >= [ primes length 1 + b! ] [
                value sieve nth [ b a - bestlen! value bestvalue! ] when
                b 1 + b!
            ] if
        ] while
    ] each bestvalue ;

:: p57 ( -- n )
    3 :> numerator! 2 :> denominator! 0 :> count!
    1000 [| |
        numerator decimal-digits denominator decimal-digits >
        [ count 1 + count! ] when
        numerator denominator 2 * + :> next-n
        numerator denominator + denominator!
        next-n numerator!
    ] times count ;

:: p62 ( -- n )
    H{ } clone :> groups!
    1 :> candidate! 0 :> digits! 0 :> answer!
    [ answer zero? ] [
        candidate 3 ^ :> cube
        cube decimal-digits :> count
        count digits > [
            groups >alist [| pair |
                pair second first 5 = [ pair second second ] [ f ] if
            ] map [ f = not ] filter
            dup empty? not [ infimum answer! ] [ drop ] if
            H{ } clone groups!
            count digits!
        ] when
        cube digit-signature :> key
        key groups at dup [
            dup first 1 + swap second 2array key groups set-at
        ] [ drop 1 cube 2array key groups set-at ] if
        candidate 1 + candidate!
    ] while answer ;

:: p65 ( -- n )
    1 :> numerator! 0 :> denominator!
    99 0 [a..b] [| index |
        index zero? [ 2 ] [
            index 3 mod 2 = [ index 1 + 3 /i 2 * ] [ 1 ] if
        ] if :> partial
        numerator partial * denominator + :> newnum
        numerator denominator!
        newnum numerator!
    ] each numerator digit-sum ;

:: p70 ( -- n )
    9999999 phi-sieve :> phi
    0 :> bestnum! 1 :> bestphi!
    2 9999999 [a..b] [| n |
        n phi nth :> totient
        bestnum zero? n bestphi * bestnum totient * < or [
            n digit-signature totient digit-signature = [
                n bestnum! totient bestphi!
            ] when
        ] when
    ] each bestnum ;

:: p74-digit-factorial ( n -- next )
    n number>string [ 48 - factorial-number ] map sum ;
:: p74 ( -- n )
    2600000 0 <array> :> lengths
    0 :> answer!
    1 999999 [a..b] [| start |
        H{ } clone :> positions
        V{ } clone :> path
        start :> current!
        [ current lengths length < current lengths nth zero? and
          current positions key? not and ] [
            path length current positions set-at
            current path push
            current p74-digit-factorial current!
        ] while
        current lengths length < current lengths nth zero? not and [
            current lengths nth :> continuation!
            path reverse [| x |
                continuation 1 + continuation!
                continuation x lengths set-nth
            ] each
        ] [
            current positions at :> cycle-start
            path length cycle-start - :> cycle-size
            cycle-start path length 1 - [a..b] [| index |
                cycle-size index path nth lengths set-nth
            ] each
            cycle-start 0 > [
                cycle-start 1 - 0 [a..b] [| index |
                    index 1 + path nth lengths nth 1 +
                    index path nth lengths set-nth
                ] each
            ] when
        ] if
        start lengths nth 60 = [ answer 1 + answer! ] when
    ] each answer ;

:: p79 ( -- n )
    "tests/data/p079_keylog.txt" read-numbers :> entries
    100 f <array> :> edge
    10 f <array> :> present
    entries [| code |
        code 100 /i :> a code 10 /i 10 mod :> b code 10 mod :> c
        t a present set-nth t b present set-nth t c present set-nth
        t a 10 * b + edge set-nth
        t b 10 * c + edge set-nth
    ] each
    0 :> result! 0 :> steps!
    [ present [ t = ] any? ] [
        -1 :> chosen!
        0 9 [a..b] [| digit |
            digit present nth [
                0 9 [a..b] [| before |
                    before present nth before 10 * digit + edge nth and
                ] any? not [ digit chosen! ] when
            ] when
        ] each
        chosen -1 = [ "Keylog is cyclic" throw ] when
        result 10 * chosen + result!
        f chosen present set-nth
        steps 1 + steps!
    ] while result ;

:: p80 ( -- n )
    0 :> answer!
    1 100 [a..b] [| n |
        n euler-isqrt :> root
        root sq n = not [
            n 10 198 ^ * euler-isqrt digit-sum answer + answer!
        ] when
    ] each answer ;

: read-matrix ( -- seq ) "tests/data/p081_matrix.txt" read-numbers >array ;
:: p81 ( -- n )
    read-matrix :> costs
    6400 0 <array> :> dp
    0 79 [a..b] [| r |
        0 79 [a..b] [| c |
            r 80 * c + :> index
            r zero? c zero? and [ 0 ] [
                r zero? [ index 1 - dp nth ] [
                    c zero? [ index 80 - dp nth ] [
                        index 80 - dp nth index 1 - dp nth min
                    ] if
                ] if
            ] if
            index costs nth + index dp set-nth
        ] each
    ] each 6399 dp nth ;
:: p82 ( -- n )
    read-matrix :> matrix
    80 0 <array> :> dist
    0 79 [a..b] [| r | r 80 * matrix nth r dist set-nth ] each
    1 79 [a..b] [| c |
        0 79 [a..b] [| r |
            r dist nth r 80 * c + matrix nth +
            r dist set-nth
        ] each
        1 79 [a..b] [| r |
            r 1 - dist nth r 80 * c + matrix nth +
            r dist nth min r dist set-nth
        ] each
        78 0 [a..b] [| r |
            r 1 + dist nth r 80 * c + matrix nth +
            r dist nth min r dist set-nth
        ] each
    ] each dist infimum ;
:: p83 ( -- n )
    read-matrix :> matrix
    6400 2147483647 <array> :> dist
    6400 f <array> :> visited
    0 matrix nth 0 dist set-nth
    6400 [| |
        -1 :> choice! 2147483647 :> smallest!
        0 6399 [a..b] [| index |
            index visited nth not index dist nth smallest < and [
                index choice! index dist nth smallest!
            ] when
        ] each
        choice -1 >= [
            t choice visited set-nth
            choice 80 /i :> row choice 80 mod :> col
            { -80 80 -1 1 } [| step |
                choice step + :> next
                next 0 >= next 6400 < and [
                    step -1 = col 0 = and not
                    step 1 = col 79 = and not and [
                        smallest next matrix nth + :> proposal
                        proposal next dist nth < [ proposal next dist set-nth ] when
                    ] when
                ] when
            ] each
        ] when
    ] times 6399 dist nth ;

:: p87 ( -- n )
    50000000 f <array> :> seen
    7072 euler-sieve :> sieve
    V{ } clone :> primes
    2 7071 [a..b] [| n | n sieve nth [ n primes push ] when ] each
    primes [| a |
        a sq :> x
        x 50000000 < [
            primes [| b |
                b 3 ^ x + :> y
                y 50000000 < [
                    primes [| c |
                        c 4 ^ y + :> z
                        z 50000000 < [ t z seen set-nth ] when
                    ] each
                ] when
            ] each
        ] when
    ] each seen [ t = ] count ;

:: p88-search ( first product total factors minima -- )
    product total - factors + :> k
    k 2 >= k 12000 <= and [
        product k minima nth min k minima set-nth
    ] when
    24000 product /i :> limit
    first limit <= [
        first limit [a..b] [| x |
            x product * :> next-product
            next-product 24000 <= [
                x next-product total x + factors 1 + minima p88-search
            ] when
        ] each
    ] when ;
:: p88 ( -- n )
    12001 24001 <array> :> minima
    2 1 0 0 minima p88-search
    2 12001 minima subseq members sum ;

CONSTANT: roman-map H{ { 73 1 } { 86 5 } { 88 10 } { 76 50 } { 67 100 }
      { 68 500 } { 77 1000 } }
: roman-value ( codepoint -- n ) roman-map at ;
CONSTANT: roman-lengths { 0 1 2 3 2 1 2 3 4 2 }
:: roman-min-length ( value -- length )
    value 1000 /i value 100 /i 10 mod roman-lengths nth +
    value 10 /i 10 mod roman-lengths nth +
    value 10 mod roman-lengths nth + ;
:: roman-number ( str -- n )
    0 :> total!
    0 str length 1 - [a..b] [| i |
        i str nth roman-value :> current
        i 1 + str length < [
            i 1 + str nth roman-value current >
        ] [ f ] if
        [ total current - total! ] [ total current + total! ] if
    ] each total ;
:: p89 ( -- n )
    "tests/data/p089_roman.txt" utf8 file-contents "\n" split
    [ empty? not ] filter
    [| str | str length str roman-number roman-min-length - ] map sum ;

:: dice-has? ( mask digit -- ? )
    digit 6 = digit 9 = or [
        mask 2 6 ^ /i 2 mod zero? not
        mask 2 9 ^ /i 2 mod zero? not or
    ] [ mask 2 digit ^ /i 2 mod zero? not ] if ;
:: p90-countbits ( mask -- n )
    0 :> count!
    0 9 [a..b] [| digit |
        mask 2 digit ^ /i 2 mod zero? not [ count 1 + count! ] when
    ] each count ;
:: p90-pair? ( a b -- ? )
    { { 0 1 } { 0 4 } { 0 9 } { 1 6 } { 2 5 } { 3 6 }
      { 4 9 } { 6 4 } { 8 1 } }
    [| target |
        a target first dice-has? b target second dice-has? and
        b target first dice-has? a target second dice-has? and or
    ] all? ;
:: p90 ( -- n )
    0 :> count!
    0 1023 [a..b] [| a |
        a p90-countbits 6 = [
            a 1023 [a..b] [| b |
                b p90-countbits 6 = a b p90-pair? and [
                    count 1 + count!
                ] when
            ] each
        ] when
    ] each count ;

:: p94 ( -- n )
    2 :> x! 0 :> y! 0 :> total!
    [ x 1000000002 <= ] [
        x 3 mod 2 = [
            x 1 + 3 /i :> side
            side 1 + :> base
            x 2 + :> perimeter
            side 1 > base y * 4 mod zero? and
            perimeter 1000000000 <= and
            [ total perimeter + total! ] when
        ] [
            x 1 - 3 /i :> side
            side 1 - :> base
            x 2 - :> perimeter
            side 1 > base 0 > and
            base y * 4 mod zero? and
            perimeter 1000000000 <= and
            [ total perimeter + total! ] when
        ] if
        x 2 * y 3 * + :> next-x
        x y 2 * + y!
        next-x x!
    ] while total ;

:: p95 ( -- n )
    1000000 divisor-sum-sieve :> divisor-sums
    1000001 0 <array> :> seen
    1000001 0 <array> :> position
    0 :> bestlength! 0 :> bestmember!
    2 1000000 [a..b] [| start |
        start :> n! 0 :> step!
        V{ } clone :> path
        [ n 0 > n 1000000 <= and [
            n seen nth dup start = not swap zero? and
          ] [ f ] if ] [
            start n seen set-nth
            step n position set-nth
            n path push step 1 + step!
            n divisor-sums nth n!
        ] while
        n 0 > n 1000000 <= and [
            n seen nth start = [
                step n position nth - :> cycle
                cycle bestlength > [
                    cycle bestlength!
                    n position nth path length path subseq infimum bestmember!
                ] when
            ] when
        ] when
    ] each bestmember ;

:: p99 ( -- n )
    "tests/data/p099_base_exp.txt" read-numbers :> data
    -1.0e100 :> greatest! 0 :> answer!
    0 999 [a..b] [| index |
        index 2 * data nth >float log
        index 2 * 1 + data nth * :> score
        score greatest > [ score greatest! index 1 + answer! ] when
    ] each answer ;

:: p101-u ( n -- result )
    0 :> total! 1 :> term!
    0 10 [a..b] [| index |
        index even? [ total term + total! ] [ total term - total! ] if
        term n * term!
    ] each total ;
:: p101-fit ( count -- value )
    0 :> total!
    1 count [a..b] [| i |
        i p101-u :> term!
        1 count [a..b] [| j |
            i j = not [
                term count 1 + j - * i j - / term!
            ] when
        ] each
        total term + total!
    ] each total ;
: p101 ( -- n ) 1 10 [a..b] [ p101-fit ] map sum ;

:: p102-cross ( ax ay bx by -- cross )
    ax by * ay bx * - ;
:: p102-contains? ( points -- ? )
    points first points second points third points fourth p102-cross :> cross-ab
    points third points fourth 4 points nth 5 points nth p102-cross :> cross-bc
    4 points nth 5 points nth points first points second p102-cross :> cross-ca
    cross-ab 0 > cross-bc 0 > and cross-ca 0 > and
    cross-ab 0 < cross-bc 0 < and cross-ca 0 < and or ;
:: p102 ( -- n )
    "tests/data/p102_triangles.txt" read-signed-numbers :> values
    0 :> answer!
    0 999 [a..b] [| i |
        i 6 * i 6 * 6 + values subseq p102-contains?
        [ answer 1 + answer! ] when
    ] each answer ;

:: p104-fib-pair ( n -- a b )
    n zero? [ 0 1 ] [
        n 2 /i p104-fib-pair :> b :> a
        a b 2 * a - * :> c
        a sq b sq + :> d
        n even? [ c d ] [ d c d + ] if
    ] if ;
:: p104 ( -- n )
    1 :> a! 1 :> b! 2 :> index! 0 :> answer!
    [ answer zero? ] [
        a b + 1000000000 mod :> next
        b a! next b!
        index 1 + index!
        b decimal-digits 9 = b pandigital-nine? and [
            index p104-fib-pair drop
            number>string 0 9 rot subseq decimal-cell
            pandigital-nine? [ index answer! ] when
        ] when
    ] while answer ;

:: special-sum? ( set -- ? )
    set sort :> nums
    nums length :> size
    t :> valid!
    1 size 2 /i [a..b] [| k |
        0 k 1 + nums subseq sum
        nums length k - nums length nums subseq sum <= [ f valid! ] when
    ] each
    H{ } clone :> sums
    1 2 size ^ 1 - [a..b] [| bits |
        0 :> total!
        0 size 1 - [a..b] [| j |
            bits 2 j ^ /i 2 mod zero? not [
                j nums nth total + total!
            ] when
        ] each
        total sums key? [ f valid! ] [ t total sums set-at ] if
    ] each valid ;
:: p105 ( -- n )
    "tests/data/p105_sets.txt" utf8 file-contents "\n" split
    [ empty? not ] filter
    [| line |
        line "," split [ decimal-cell ] map :> values
        values special-sum? [ values sum ] [ 0 ] if
    ] map sum ;

:: p107 ( -- n )
    "tests/data/p107_network.txt" utf8 file-contents "\n" split
    [ empty? not ] filter :> lines
    1600 0 <array> :> weight
    0 :> total!
    0 39 [a..b] [| r |
        r lines nth "," split :> entries
        0 39 [a..b] [| c |
            c entries nth :> token
            token "-" = not [
                token decimal-cell :> edge
                edge r 40 * c + weight set-nth
                r c < [ edge total + total! ] when
            ] when
        ] each
    ] each
    40 f <array> :> reached
    t 0 reached set-nth
    0 :> tree!
    39 [| |
        999999999 :> best! -1 :> next!
        0 39 [a..b] [| a |
            a reached nth [
                0 39 [a..b] [| b |
                    b reached nth not [
                        a 40 * b + weight nth :> distance
                        distance 0 > distance best < and [
                            distance best! b next!
                        ] when
                    ] when
                ] each
            ] when
        ] each
        next -1 = [ "Disconnected network" throw ] when
        t next reached set-nth best tree + tree!
    ] times total tree - ;

:: p108-divisors ( value -- count )
    value :> remaining! 1 :> ways! 2 :> p!
    [ p sq remaining <= ] [
        remaining p mod zero? [
            0 :> exponent!
            [ remaining p mod zero? ] [
                remaining p /i remaining!
                exponent 1 + exponent!
            ] while
            ways exponent 2 * 1 + * ways!
        ] when
        p 1 + p!
    ] while
    remaining 1 > [ ways 3 * ways! ] when
    ways 1 + 2 /i ;
:: p108 ( -- n )
    1 :> n!
    [ n p108-divisors 1000 <= ] [ n 1 + n! ] while n ;

CONSTANT: p110-primes { 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 }
:: p110-search ( index exponent current divisors best -- )
    divisors 7999999 > [ current best first min 0 best set-nth ] [
        index p110-primes length < [
            index p110-primes nth :> prime
            current :> candidate!
            1 exponent [a..b] [| power |
                candidate prime * candidate!
                candidate best first >= [ ] [
                    index 1 + power candidate
                    divisors power 2 * 1 + * best p110-search
                ] if
            ] each
        ] when
    ] if ;
:: p110 ( -- n )
    1000000000000000000 1array :> best
    0 13 1 1 best p110-search
    best first ;

:: p109 ( -- n )
    V{ 0 } clone :> scores
    1 20 [a..b] [| n |
        n scores push n 2 * scores push n 3 * scores push
    ] each
    25 scores push 50 scores push
    V{ } clone :> doubles
    1 20 [a..b] [| n | n 2 * doubles push ] each
    50 doubles push
    0 :> answer!
    doubles [| finish |
        0 scores length 1 - [a..b] [| i |
            i scores length 1 - [a..b] [| j |
                i scores nth j scores nth + finish + 100 <
                [ answer 1 + answer! ] when
            ] each
        ] each
    ] each answer ;

:: p119 ( -- n )
    V{ } clone :> found
    2 200 [a..b] [| base |
        base sq :> power!
        [ power 100000000000000000 <= ] [
            power 10 >= power digit-sum base = and [ power found push ] when
            power base * power!
        ] while
    ] each found members sort :> ordered
    29 ordered nth ;

:: p51-family ( n replace-mask primes -- ? )
    n decimal-digits :> width
    -1 :> chosen! 0 :> coefficient! t :> valid!
    0 width 1 - [a..b] [| k |
        replace-mask 2 k ^ /i 2 mod zero? not [
            n 10 k ^ /i 10 mod :> digit
            chosen -1 = [ digit chosen! ] [
                chosen digit = not [ f valid! ] when
            ] if
            coefficient 10 k ^ + coefficient!
        ] when
    ] each
    valid not [ f ] [
        n chosen coefficient * - :> base
        0 :> found! 0 :> smallest!
        0 9 [a..b] [| digit |
            digit zero? replace-mask 2 width 1 - ^ /i 2 mod zero? not and not [
                base coefficient digit * + :> candidate
                candidate primes nth [
                    found zero? [ candidate smallest! ] when
                    found 1 + found!
                ] when
            ] when
        ] each
        found 8 = smallest n = and
    ] if ;
:: p51 ( -- n )
    999999 euler-sieve :> sieve
    0 :> answer!
    11 999999 [a..b] [| n |
        answer zero? n sieve nth and [
            1 2 n decimal-digits ^ 1 - [a..b] [| mask |
                answer zero? [
                    n mask sieve p51-family [ n answer! ] when
                ] when
            ] each
        ] when
    ] each answer ;

:: poker-face ( letter -- rank )
    letter 50 >= letter 57 <= and [ letter 48 - ] [
        letter 84 = [ 10 ] [ letter 74 = [ 11 ] [
            letter 81 = [ 12 ] [ letter 75 = [ 13 ] [ 14 ] if ] if
        ] if ] if
    ] if ;
:: poker-value ( cards -- value )
    15 0 <array> :> counts
    t :> flush!
    cards first second :> suit
    cards [| card |
        card first poker-face :> rank
        rank counts nth 1 + rank counts set-nth
        card second suit = not [ f flush! ] when
    ] each
    V{ } clone :> ordered
    14 2 [a..b] [| rank |
        rank counts nth 0 > [ rank ordered push ] when
    ] each
    0 :> straight!
    ordered length 5 = [
        ordered first ordered last - 4 = [ ordered first straight! ] when
        14 counts nth 1 =
        { 2 3 4 5 } [| rank | rank counts nth 1 = ] all? and
        [ 5 straight! ] when
    ] when
    counts [ 4 = ] any? :> four
    counts [ 3 = ] any? :> triple
    counts [ 2 = ] count :> pairs
    straight 0 > flush and [ 8 ] [
        four [ 7 ] [
            triple pairs 1 = and [ 6 ] [
                flush [ 5 ] [
                    straight 0 > [ 4 ] [
                        triple [ 3 ] [
                            pairs 2 = [ 2 ] [ pairs 1 = [ 1 ] [ 0 ] if ] if
                        ] if
                    ] if
                ] if
            ] if
        ] if
    ] if :> category
    category 4 = category 8 = or [
        category 15 * straight + 15 4 ^ *
    ] [
        V{ } clone :> priority
        4 1 [a..b] [| repeat |
            14 2 [a..b] [| rank |
                rank counts nth repeat = [
                    repeat [| | rank priority push ] times
                ] when
            ] each
        ] each
        category :> value!
        priority [| rank | value 15 * rank + value! ] each
        value
    ] if ;
:: p54 ( -- n )
    "tests/data/p054_poker.txt" utf8 file-contents
    [ whitespace? ] split-when [ empty? not ] filter :> cards
    0 :> answer!
    0 999 [a..b] [| i |
        i 10 * :> offset
        offset offset 5 + cards subseq poker-value
        offset 5 + offset 10 + cards subseq poker-value >
        [ answer 1 + answer! ] when
    ] each answer ;

CONSTANT: english-frequency { 8 2 3 4 13 2 2 6 7 1 1 4 2 7 8 2 1 6 6 9 3 1 2 1 2 1 }
CONSTANT: english-phrases { " the " " and " " of " " to " " in "
  " that " " is " " a " " for " " with " }
:: english-score ( text -- n )
    0 :> score! t :> valid!
    text [| letter |
        letter 32 < letter 126 > or [ f valid! ] when
        letter 32 = [ score 15 + score! ] [
            letter 65 >= letter 90 <= and [
                score letter 65 - english-frequency nth + score!
            ] [
                letter 97 >= letter 122 <= and [
                    score letter 97 - english-frequency nth + score!
                ] [
                    letter 48 >= letter 57 <= and
                    [ score 3 - score! ] [ score 1 - score! ] if
                ] if
            ] if
        ] if
    ] each
    0 :> hits!
    english-phrases [| phrase |
        text phrase split length 1 - :> count
        hits count + hits!
    ] each
    valid hits 3 >= and [ score hits 100 * + ] [ -1000000000 ] if ;
:: p59 ( -- n )
    "tests/data/p059_cipher.txt" read-numbers :> cipher
    -1000000000000 :> best! 0 :> answer!
    97 122 [a..b] [| a |
        97 122 [a..b] [| b |
            97 122 [a..b] [| c |
                V{ } clone :> text
                cipher [| x index |
                    index 3 mod 0 = [ a ] [ index 3 mod 1 = [ b ] [ c ] if ] if
                    x bitxor text push
                ] each-index
                text english-score :> score
                score best > [
                    score best!
                    text sum answer!
                ] when
            ] each
        ] each
    ] each answer ;

:: p60-concat ( a b -- n ) b decimal-digits pow10 a * b + ;
:: p60-search ( vertices depth value primes adjacent best -- )
    5 depth - :> needed
    vertices length needed >= [
        value 0 needed vertices subseq [| v | v primes nth ] map sum +
        best first < [
            needed 1 = [
                value vertices first primes nth + 0 best set-nth
            ] [
                0 vertices length needed - [a..b] [| i |
                    i vertices nth :> current
                    V{ } clone :> next
                    i 1 + vertices length < [
                        i 1 + vertices length 1 - [a..b] [| j |
                            j vertices nth :> candidate
                            current primes length * candidate + adjacent nth [
                                candidate next push
                            ] when
                        ] each
                    ] when
                    next depth 1 + value current primes nth + primes adjacent best
                    p60-search
                ] each
            ] if
        ] when
    ] when ;
:: p60-at-limit ( limit best -- )
    limit euler-sieve :> sieve
    V{ } clone :> primes
    3 limit [a..b] [| p |
        p 5 = not p sieve nth and [ p primes push ] when
    ] each
    primes length :> length
    length length * f <array> :> edges
    0 length 1 - [a..b] [| i |
        i primes nth :> a
        i 1 + length < [
            i 1 + length 1 - [a..b] [| j |
                j primes nth :> b
                a b + best first < a b + 3 mod zero? not and [
                    a b p60-concat prime?
                    b a p60-concat prime? and [
                        t i length * j + edges set-nth
                        t j length * i + edges set-nth
                    ] when
                ] when
            ] each
        ] when
    ] each
    0 length 1 - [a..b] >array 0 0 primes edges best p60-search ;
:: p60 ( -- n )
    99999999 1array :> best
    10000 best p60-at-limit
    best first 99999999 = [ "No graph clique" throw ] when
    best first 1 - best p60-at-limit
    best first ;

:: p61-recurse ( path used groups -- n )
    path length 6 = [
        path last 100 mod path first 100 /i =
        [ path sum ] [ 0 ] if
    ] [
        0 :> answer!
        path last 100 mod :> prefix
        0 5 [a..b] [| kind |
            answer zero? used 2 kind ^ /i 2 mod zero? and [
                kind groups nth [| value |
                    answer zero? value 100 /i prefix = and
                    value path member? not and [
                        path clone :> next
                        value next push
                        next used 2 kind ^ + groups p61-recurse
                        dup 0 > [ answer! ] [ drop ] if
                    ] when
                ] each
            ] when
        ] each answer
    ] if ;
:: p61 ( -- n )
    V{ } clone :> groups
    3 8 [a..b] [| sides |
        V{ } clone :> values
        1 :> n!
        [ sides 2 - n sq * sides 4 - n * - 2 /i 10000 < ] [
            sides 2 - n sq * sides 4 - n * - 2 /i :> value
            value 1000 >= value 100 mod 10 >= and [ value values push ] when
            n 1 + n!
        ] while
        values groups push
    ] each
    0 :> answer!
    5 groups nth [| value |
        answer zero? [
            V{ } clone :> path
            value path push
            path 32 groups p61-recurse
            dup 0 > [ answer! ] [ drop ] if
        ] when
    ] each answer ;

:: p66-minimum-x ( d -- x )
    d euler-isqrt :> root
    0 :> m! 1 :> q! root :> a!
    0 :> oldp! 1 :> p! 1 :> oldq! 0 :> denominator!
    0 :> answer!
    [ answer zero? ] [
        a p * oldp + :> next-p
        a denominator * oldq + :> next-q
        next-p sq d next-q sq * - 1 = [ next-p answer! ] when
        p oldp! next-p p!
        denominator oldq! next-q denominator!
        q a * m - m!
        d m sq - q /i q!
        root m + q /i a!
    ] while answer ;
:: p66 ( -- n )
    0 :> best! 0 :> answer!
    2 1000 [a..b] [| d |
        d euler-isqrt :> root
        root sq d = not [
            d p66-minimum-x :> x
            x best > [ x best! d answer! ] when
        ] when
    ] each answer ;

:: p68-extend ( inner used best -- )
    inner length 5 = [
        1 10 [a..b] [| outer-first |
            used 2 outer-first ^ /i 2 mod zero? [
                outer-first inner first inner second + + :> total
                used :> occupied!
                V{ } clone :> outside
                t :> valid!
                0 4 [a..b] [| index |
                    total index inner nth - index 1 + 5 mod inner nth - :> number
                    number outer-first >= number 10 <= and [
                        occupied 2 number ^ /i 2 mod zero? [
                            occupied 2 number ^ + occupied!
                            number outside push
                        ] [ f valid! ] if
                    ] [ f valid! ] if
                ] each
                valid [
                    0 :> representation!
                    0 4 [a..b] [| i |
                        i outside nth :> outer
                        representation outer decimal-digits pow10 * outer + representation!
                        representation 10 * i inner nth + representation!
                        representation 10 * i 1 + 5 mod inner nth + representation!
                    ] each
                    representation best first max 0 best set-nth
                ] when
            ] when
        ] each
    ] [
        1 9 [a..b] [| n |
            used 2 n ^ /i 2 mod zero? [
                inner clone :> next
                n next push
                next used 2 n ^ + best p68-extend
            ] when
        ] each
    ] if ;
:: p68 ( -- n )
    0 1array :> best
    V{ } clone 0 best p68-extend
    best first ;

:: p84-add ( square doubles weight next -- )
    square 3 * doubles + :> index
    index next nth weight + index next set-nth ;
:: p84-chest ( square doubles weight next -- )
    0 doubles weight 16 / next p84-add
    10 0 weight 16 / next p84-add
    square doubles weight 14 * 16 / next p84-add ;
:: p84-chance ( square doubles weight next -- )
    { 0 11 24 39 5 } [| target |
        target doubles weight 16 / next p84-add
    ] each
    10 0 weight 16 / next p84-add
    square 7 = [ 15 ] [ square 22 = [ 25 ] [ 5 ] if ] if
    doubles weight 8 / next p84-add
    square 22 = [ 28 ] [ 12 ] if
    doubles weight 16 / next p84-add
    square 36 = [
        33 doubles weight 16 / next p84-chest
    ] [
        square 3 - doubles weight 16 / next p84-add
    ] if
    square doubles weight 6 * 16 / next p84-add ;
:: p84-roll ( square doubles a b weight next -- )
    a b = doubles 2 = and [
        10 0 weight next p84-add
    ] [
        a b = [ doubles 1 + ] [ 0 ] if :> next-doubles
        square a + b + 40 mod :> target
        target 30 = [ 10 0 weight next p84-add ] [
            { 7 22 36 } target swap member? [
                target next-doubles weight next p84-chance
            ] [
                { 2 17 33 } target swap member? [
                    target next-doubles weight next p84-chest
                ] [ target next-doubles weight next p84-add ] if
            ] if
        ] if
    ] if ;
:: p84 ( -- n )
    120 120.0 recip <array> :> probability!
    300 [| |
        120 0.0 <array> :> next
        0 39 [a..b] [| square |
            0 2 [a..b] [| doubles |
                square 3 * doubles + probability nth 16 / :> weight
                1 4 [a..b] [| a |
                    1 4 [a..b] [| b |
                        square doubles a b weight next p84-roll
                    ] each
                ] each
            ] each
        ] each next probability!
    ] times
    40 f <array> :> used
    0 :> answer!
    3 [| |
        -1 :> chosen! -1.0 :> best!
        0 39 [a..b] [| square |
            square used nth not [
                0 :> mass!
                0 2 [a..b] [| doubles |
                    mass square 3 * doubles + probability nth + mass!
                ] each
                mass best > [ mass best! square chosen! ] when
            ] when
        ] each
        t chosen used set-nth
        answer 100 * chosen + answer!
    ] times answer ;

:: p93-expressions ( terms reachable -- )
    terms length 1 = [
        terms first :> value
        value integer? value 0 > and value 10000 < and [
            t value reachable set-nth
        ] when
    ] [
        0 terms length 2 - [a..b] [| i |
            i 1 + terms length 1 - [a..b] [| j |
                i terms nth :> a j terms nth :> b
                V{ } clone :> rest
                0 terms length 1 - [a..b] [| k |
                    k i = k j = or not [ k terms nth rest push ] when
                ] each
                V{ } clone :> options
                a b + options push
                a b - options push
                b a - options push
                a b * options push
                b zero? not [ a b / options push ] when
                a zero? not [ b a / options push ] when
                options [| candidate |
                    rest clone :> next
                    candidate next push
                    next reachable p93-expressions
                ] each
            ] each
        ] each
    ] if ;
:: p93-run ( a b c d -- n )
    10000 f <array> :> reached
    { a b c d } reached p93-expressions
    1 :> n!
    [ n 10000 < n reached nth and ] [ n 1 + n! ] while
    n 1 - ;
:: p93 ( -- n )
    0 :> bestlen! 0 :> answer!
    0 6 [a..b] [| a |
        a 1 + 7 [a..b] [| b |
            b 1 + 8 [a..b] [| c |
                c 1 + 9 [a..b] [| d |
                    a b c d p93-run :> length
                    length bestlen > [
                        length bestlen!
                        a 1000 * b 100 * + c 10 * + d + answer!
                    ] when
                ] each
            ] each
        ] each
    ] each answer ;

:: p96-options ( board position -- digits )
    position 9 /i :> row
    position 9 mod :> column
    row 3 /i :> box-r column 3 /i :> box-c
    10 f <array> :> used
    0 80 [a..b] [| i |
        i board nth :> digit
        digit 0 > [
            i 9 /i row = i 9 mod column = or
            i 9 /i 3 /i box-r = i 9 mod 3 /i box-c = and or [
                t digit used set-nth
            ] when
        ] when
    ] each
    V{ } clone :> options
    1 9 [a..b] [| d | d used nth not [ d options push ] when ] each
    options ;
:: p96-solve ( board -- ? )
    -1 :> chosen! V{ } clone :> best-options!
    10 :> minimum!
    t :> valid!
    0 80 [a..b] [| i |
        i board nth zero? [
            board i p96-options :> choices
            choices empty? [ f valid! ] when
            choices length minimum < [
                i chosen! choices best-options! choices length minimum!
            ] when
        ] when
    ] each
    valid not [ f ] [
        chosen -1 = [ t ] [
            f :> solved!
            best-options [| d |
                solved not [
                    d chosen board set-nth
                    board p96-solve [ t solved! ] [ 0 chosen board set-nth ] if
                ] when
            ] each solved
        ] if
    ] if ;
:: p96 ( -- n )
    "tests/data/p096_sudoku.txt" utf8 file-contents "\n" split
    [ empty? not ] filter :> lines
    0 :> answer!
    0 49 [a..b] [| grid |
        81 0 <array> :> board
        0 8 [a..b] [| row |
            grid 10 * row + 1 + lines nth :> line
            0 8 [a..b] [| col |
                col line nth 48 - row 9 * col + board set-nth
            ] each
        ] each
        board p96-solve [
            0 board nth 100 * 1 board nth 10 * + 2 board nth +
            answer + answer!
        ] [ "Unsolved sudoku" throw ] if
    ] each answer ;

:: p98-pair ( a b -- n )
    a length :> length
    10 length 1 - ^ :> first-square
    first-square euler-isqrt :> lower!
    lower sq first-square < [ lower 1 + lower! ] when
    10 length ^ 1 - euler-isqrt :> upper
    0 :> answer!
    lower upper [a..b] [| root |
        root sq number>string :> digits
        26 -1 <array> :> mapping
        10 -1 <array> :> used
        t :> valid!
        0 length 1 - [a..b] [| i |
            i a nth 65 - :> letter
            i digits nth 48 - :> digit
            letter mapping nth -1 = [
                digit used nth -1 = [
                    digit letter mapping set-nth
                    letter digit used set-nth
                ] [ f valid! ] if
            ] [
                letter mapping nth digit = not [ f valid! ] when
            ] if
        ] each
        valid b first 65 - mapping nth 0 = not and [
            0 :> other!
            b [| ch | other 10 * ch 65 - mapping nth + other! ] each
        other euler-isqrt :> root
        root sq other = [
                other root sq max answer max answer!
            ] when
        ] when
    ] each answer ;
:: p98 ( -- n )
    "tests/data/p098_words.txt" read-upper-words :> words
    words [ >array sort ] map :> signatures
    0 :> answer!
    0 words length 2 - [a..b] [| i |
        i 1 + words length 1 - [a..b] [| j |
            i signatures nth j signatures nth = [
                i words nth j words nth p98-pair
                answer max answer!
            ] when
        ] each
    ] each answer ;

:: p103-cardinal? ( chosen -- ? )
    t :> valid!
    1 3 [a..b] [| k |
        chosen length k 1 + >= [
            0 k 1 + chosen subseq sum :> small
            0 :> large!
            7 k - 6 [a..b] [| i |
                i chosen length < [ i chosen nth ] [
                    chosen last i chosen length - 1 + +
                ] if large + large!
            ] each
            small large <= [ f valid! ] when
        ] when
    ] each valid ;
:: p103-search ( chosen sums best upper -- )
    chosen length 7 = [
        chosen sum 0 upper set-nth
        chosen clone 0 best set-nth
    ] [
        7 chosen length - :> remaining
        chosen empty? [ 1 ] [ chosen last 1 + ] if :> candidate!
        [ chosen sum remaining candidate * +
          remaining remaining 1 - * 2 /i + upper first < ] [
            chosen clone :> next
            candidate next push
            next p103-cardinal? [
                sums [ candidate + ] map :> additions
                additions [| n | n sums member? not ] all? [
                    next sums additions append best upper p103-search
                ] when
            ] when
            candidate 1 + candidate!
        ] while
    ] if ;
:: p103 ( -- n )
    V{ 20 } clone :> seed
    { 11 18 19 20 22 25 } [| n | n 20 + seed push ] each
    seed clone 1array :> best
    seed sum 1array :> upper
    V{ } clone { 0 } best upper p103-search
    best first [ number>string ] map "" join decimal-cell ;

:: p111-build ( digit mask position current result -- )
    position 10 = [
        current prime? [
            current result first + 0 result set-nth
            result second 1 + 1 result set-nth
        ] when
    ] [
        mask 2 position ^ /i 2 mod zero? [
            position zero? digit zero? and not [
                position 9 = digit 2 mod zero? digit 5 = or and not [
                    digit mask position 1 + current 10 * digit + result p111-build
                ] when
            ] when
        ] [
            0 9 [a..b] [| d |
                d digit = not position zero? d zero? and not and
                position 9 = d 2 mod zero? d 5 = or and not and [
                    digit mask position 1 + current 10 * d + result p111-build
                ] when
            ] each
        ] if
    ] if ;
:: p111 ( -- n )
    0 :> total!
    0 9 [a..b] [| digit |
        f :> found!
        1 10 [a..b] [| changes |
            found not [
                0 0 2array :> result
                1 1023 [a..b] [| mask |
                    mask p90-countbits changes = [
                        digit mask 0 0 result p111-build
                    ] when
                ] each
                result second 0 > [
                    result first total + total!
                    t found!
                ] when
            ] when
        ] each
    ] each total ;

:: p118-construct ( unused full value counts -- )
    unused zero? [
        value prime? [
            full counts nth 1 + full counts set-nth
        ] when
    ] [
        1 9 [a..b] [| digit |
            2 digit 1 - ^ :> bit
            unused bit /i 2 mod zero? not [
                unused bit - full value 10 * digit + counts p118-construct
            ] when
        ] each
    ] if ;
:: p118-subset? ( subset remaining -- ? )
    t :> valid!
    0 8 [a..b] [| bit |
        subset 2 bit ^ /i 2 mod zero? not [
            remaining 2 bit ^ /i 2 mod zero? [ f valid! ] when
        ] when
    ] each valid ;
:: p118-solve ( remaining counts memory -- ways )
    remaining zero? [ 1 ] [
        remaining memory nth -1 = not [ remaining memory nth ] [
            1 :> least!
            [ remaining least /i 2 mod zero? not ] [ least 2 * least! ] until
            0 :> answer!
            1 remaining [a..b] [| subset |
                subset least /i 2 mod zero? not
                subset remaining p118-subset? and [
                    subset counts nth :> count
                    count 0 > [
                        remaining subset - counts memory p118-solve
                        count * answer + answer!
                    ] when
                ] when
            ] each
            answer remaining memory set-nth answer
        ] if
    ] if ;
:: p118 ( -- n )
    512 0 <array> :> counts
    511 511 0 counts p118-construct
    ! All submasks are generated from the same pool, not just from 511.
    1 510 [a..b] [| mask |
        mask mask 0 counts p118-construct
    ] each
    512 -1 <array> :> memory
    511 counts memory p118-solve ;
