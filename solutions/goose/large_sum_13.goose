// Project Euler 13: Large Sum.
import euler from "../../support/goose";

fn solve() -> i64 {
    let t = file_text("lsum");
    var sums = [0; 60];
    var col = 0;
    var rows = 0;
    for ch in t {
        if ch >= '0' && ch <= '9' {
            sums[49 - col] += ch - '0';
            col++;
        } else if col > 0 {
            assert(col == 50);
            rows++;
            col = 0;
        }
    }
    if col > 0 {
        rows++;
    }
    assert(rows == 100);
    for i in 59 {
        sums[i + 1] += sums[i] / 10;
        sums[i] %= 10;
    }
    var hi = 59;
    while sums[hi] == 0 {
        hi--;
    }
    var answer = 0;
    for i in 10 {
        answer = 10 * answer + sums[hi - i];
    }
    return answer;
}

fn main() {
    print(solve());
}
