// Project Euler 95: Amicable Chains.
import euler from "../../support/goose";

fn solve() -> i64 {
    var sums = [0; 1000001];
    for d in 1..500001 {
        var n = 2 * d;
        while n <= 1000000 {
            sums[n] += d;
            n += d;
        }
    }
    var done = [false; 1000001];
    var visit = [0; 1000001];
    var depth = [0; 1000001];
    var best = 0;
    var answer = 0;
    for start in 2..1000001 {
        if done[start] {
            continue;
        }
        var n = start;
        var k = 0;
        while n > 0 && n <= 1000000 && !done[n] && visit[n] != start {
            visit[n] = start;
            depth[n] = k;
            k++;
            n = sums[n];
        }
        if n > 0 && n <= 1000000 && visit[n] == start {
            let len = k - depth[n];
            var smallest = n;
            var p = sums[n];
            while p != n {
                smallest = min(smallest, p);
                p = sums[p];
            }
            if len > best || (len == best && smallest < answer) {
                best = len;
                answer = smallest;
            }
        }
        n = start;
        while n > 0 && n <= 1000000 && !done[n] {
            done[n] = true;
            n = sums[n];
        }
    }
    return answer;
}

fn main() {
    print(solve());
}
