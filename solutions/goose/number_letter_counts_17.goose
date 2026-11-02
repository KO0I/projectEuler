// Project Euler 17: Number Letter Counts.
import euler from "../../support/goose";

fn letters(n0: i64) -> i64 {
    if n0 == 1000 {
        return 11;
    }
    let small = [0, 3, 3, 5, 4, 4, 3, 5, 5, 4, 3, 6, 6, 8, 8, 7, 7, 9, 8, 8];
    let tens = [0, 0, 6, 6, 5, 5, 5, 7, 6, 6];
    var n = n0;
    var s = 0;
    if n >= 100 {
        s += small[n / 100] + 7;
        n %= 100;
        if n > 0 {
            s += 3;
        }
    }
    if n >= 20 {
        s += tens[n / 10];
        n %= 10;
    }
    return s + small[n];
}

fn solve() -> i64 {
    var s = 0;
    for n in 1..1001 {
        s += letters(n);
    }
    return s;
}

fn main() {
    print(solve());
}
