// Project Euler 74: Digit Factorial Chains.
import euler from "../../support/goose";

fn step(n0: i64) -> i64 {
    let f = [1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880];
    var n = n0;
    var s = 0;
    loop {
        s += f[n % 10];
        n /= 10;
        if n == 0 {
            break;
        }
    }
    return s;
}

fn solve() -> i64 {
    var cache = [0; 2540161];
    var answer = 0;
    for start in 1..1000000 {
        var path = [0; 100];
        var len = 0;
        var n = start;
        while cache[n] == 0 {
            var cycle = -1;
            for i in len {
                if path[i] == n {
                    cycle = i;
                    break;
                }
            }
            if cycle >= 0 {
                for i in cycle..len {
                    cache[path[i]] = len - cycle;
                }
                break;
            }
            assert(len < 100);
            path[len] = n;
            len++;
            n = step(n);
        }
        var i = len - 1;
        while i >= 0 {
            if cache[path[i]] == 0 {
                cache[path[i]] = cache[step(path[i])] + 1;
            }
            i--;
        }
        if cache[start] == 60 {
            answer++;
        }
    }
    return answer;
}

fn main() {
    print(solve());
}
