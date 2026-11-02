// Project Euler 22: Names Scores.
import euler from "../../support/goose";

fn solve() -> i64 {
    var names = read_words("names.txt");
    sort(names[..]) {
        a, b => compare(a, b) < 0
    };
    var sum = 0;
    for i in names.len {
        var score = 0;
        for ch in names[i] {
            score += ch - 'A' + 1;
        }
        sum += (i + 1) * score;
    }
    return sum;
}

fn main() {
    print(solve());
}
