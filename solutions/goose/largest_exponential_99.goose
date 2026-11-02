// Project Euler 99: Largest Exponential.
import euler from "../../support/goose";

fn solve() -> i64 {
    let a = numbers("tests/data/p099_base_exp.txt");
    assert(a.len == 2000);
    var best = 0.0;
    var answer = 0;
    for i in 1000 {
        let score = (a[2 * i + 1] as f64) * log(a[2 * i] as f64);
        if score > best {
            best = score;
            answer = i + 1;
        }
    }
    return answer;
}

fn main() {
    print(solve());
}
