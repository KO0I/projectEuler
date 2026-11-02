// Project Euler 42: Coded Triangle Numbers.
import euler from "../../support/goose";

fn solve() -> i64 {
    let a = read_words("tests/data/p042_words.txt");
    var count = 0;
    for w in a {
        var s = 0;
        for c in w {
            s += c - 'A' + 1;
        }
        if is_square(8 * s + 1) {
            count++;
        }
    }
    return count;
}

fn main() {
    print(solve());
}
