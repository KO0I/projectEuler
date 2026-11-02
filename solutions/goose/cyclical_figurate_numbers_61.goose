// Project Euler 61: Cyclical Figurate Numbers.
import euler from "../../support/goose";

var values = [[0; 200];
    6];
var sizes = [0; 6];

recursive fn search(first: i64, last: i64, used: i64, sum: i64) -> i64 {
    if used == 63 {
        return if last % 100 == first / 100 {
            sum
        } else {
            0
        };
    }
    for k in 6 {
        if used & (1 << k) != 0 {
            continue;
        }
        for i in sizes[k] {
            let v = values[k][i];
            if v / 100 == last % 100 {
                let ans = search(first, v, used | (1 << k), sum + v);
                if ans > 0 {
                    return ans;
                }
            }
        }
    }
    return 0;
}

fn solve() -> i64 {
    for kind in 6 {
        for n in 1..200 {
            let v = ((kind + 1) * n * n - (kind - 1) * n) / 2;
            if v >= 1000 && v < 10000 && v % 100 >= 10 {
                values[kind][sizes[kind]] = v;
                sizes[kind]++;
            }
        }
    }
    for i in sizes[0] {
        let start = values[0][i];
        let answer = search(start, start, 1, start);
        if answer > 0 {
            return answer;
        }
    }
    abort("no cycle");
}

fn main() {
    print(solve());
}
