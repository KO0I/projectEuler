// Project Euler 40: Champernowne's Constant.
import euler from "../../support/goose";

fn solve() -> i64 {
    var pos = 0;
    var target = 1;
    var answer = 1;
    var n = 1;
    while target <= 1000000 {
        let len = digits(n);
        if pos + len >= target {
            answer *= n / ipow(10, pos + len - target) % 10;
            target *= 10;
        }
        pos += len;
        n++;
    }
    return answer;
}

fn main() {
    print(solve());
}
