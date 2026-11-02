// Project Euler 26: Reciprocal Cycles.
import euler from "../../support/goose";

fn solve() -> i64 {
    var best = 0;
    var answer = 0;
    for d in 2..1000 {
        var seen = [-1; 1000];
        var r = 1;
        var k = 0;
        while r != 0 && seen[r] < 0 {
            seen[r] = k;
            r = r * 10 % d;
            k++;
        }
        if r != 0 && k - seen[r] > best {
            best = k - seen[r];
            answer = d;
        }
    }
    return answer;
}

fn main() {
    print(solve());
}
