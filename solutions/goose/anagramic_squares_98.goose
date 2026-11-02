// Project Euler 98: Anagramic Squares.
import euler from "../../support/goose";

fn solve() -> i64 {
    let ws = read_words("tests/data/p098_words.txt");
    var best = 0;
    for i in ws.len {
        for j in i + 1..ws.len {
            let a = ws[i];
            let b = ws[j];
            if a.len != b.len {
                continue;
            }
            var ca = [0; 26];
            var cb = [0; 26];
            for c in a {
                ca[c - 'A']++;
            }
            for c in b {
                cb[c - 'A']++;
            }
            if ca != cb {
                continue;
            }
            let lo = ceil(sqrt(ipow(10, a.len - 1) as f64)) as! i64;
            let hi = sqrt((ipow(10, a.len) - 1) as f64) as! i64;
            for r in lo..hi + 1 {
                let square = r * r;
                var map = [-1; 26];
                var used = [false; 10];
                var valid = true;
                var t = square;
                var k = a.len - 1;
                while k >= 0 {
                    let ch = a[k] - 'A';
                    let d = t % 10;
                    t /= 10;
                    if map[ch] >= 0 {
                        if map[ch] != d {
                            valid = false;
                            break;
                        }
                    } else {
                        if used[d] {
                            valid = false;
                            break;
                        }
                        map[ch] = d;
                        used[d] = true;
                    }
                    k--;
                }
                if !valid || map[b[0] - 'A'] == 0 {
                    continue;
                }
                var other = 0;
                for ch in b {
                    other = other * 10 + map[ch - 'A'];
                }
                if is_square(other) {
                    best = max(best, max(square, other));
                }
            }
        }
    }
    return best;
}

fn main() {
    print(solve());
}
