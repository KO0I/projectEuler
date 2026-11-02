// Project Euler 93: Arithmetic Expressions.
import euler from "../../support/goose";

recursive fn combine(v: f64[4], len: i64, seen: bool[10000]&) {
    if len == 1 {
        let n = round(v[0]) as! i64;
        if n > 0 && n < 10000 && abs(v[0] - (n as f64)) < 0.0000001 {
            seen[n] = true;
        }
        return;
    }
    for i in len {
        for j in i + 1..len {
            var next = [0.0; 4];
            var k = 0;
            for x in len {
                if x != i && x != j {
                    next[k] = v[x];
                    k++;
                }
            }
            let a = v[i];
            let b = v[j];
            for op in 6 {
                var value = 0.0;
                if op == 0 {
                    value = a + b;
                } else if op == 1 {
                    value = a - b;
                } else if op == 2 {
                    value = b - a;
                } else if op == 3 {
                    value = a * b;
                } else if op == 4 {
                    if b == 0.0 {
                        continue;
                    }
                    value = a / b;
                } else {
                    if a == 0.0 {
                        continue;
                    }
                    value = b / a;
                }
                next[k] = value;
                combine(next, len - 1, seen);
            }
        }
    }
}

fn solve() -> i64 {
    var best = 0;
    var answer = 0;
    for a in 1..7 {
        for b in a + 1..8 {
            for c in b + 1..9 {
                for d in c + 1..10 {
                    var seen = [false; 10000];
                    combine([(a as f64), (b as f64), (c as f64), (d as f64)], 4, seen);
                    var n = 1;
                    while n < 10000 && seen[n] {
                        n++;
                    }
                    if n - 1 > best {
                        best = n - 1;
                        answer = 1000 * a + 100 * b + 10 * c + d;
                    }
                }
            }
        }
    }
    return answer;
}

fn main() {
    print(solve());
}
