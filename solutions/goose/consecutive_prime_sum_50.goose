// Project Euler 50: Consecutive Prime Sum.
import euler from "../../support/goose";

fn solve() -> i64 {
    let p = sieve(999999);
    var sums: i64[>..] = [0];
    var total = 0;
    for n in 2..1000000 {
        if p[n] {
            total += n;
            sums.push(total);
        }
    }
    var best = 0;
    var answer = 0;
    for i in sums.len {
        var j = i + best + 1;
        while j < sums.len {
            let s = sums[j] - sums[i];
            if s >= 1000000 {
                break;
            }
            if p[s] {
                best = j - i;
                answer = s;
            }
            j++;
        }
    }
    return answer;
}

fn main() {
    print(solve());
}
