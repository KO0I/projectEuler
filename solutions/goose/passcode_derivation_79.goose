// Project Euler 79: Passcode Derivation.
import euler from "../../support/goose";

fn solve() -> i64 {
    let input = numbers("tests/data/p079_keylog.txt");
    var edge = [[false; 10];
        10];
    var used = [false; 10];
    for n in input {
        let a = n / 100;
        let b = n / 10 % 10;
        let c = n % 10;
        edge[a][b] = true;
        edge[b][c] = true;
        used[a] = true;
        used[b] = true;
        used[c] = true;
    }
    var answer = 0;
    loop {
        var chosen = -1;
        var remaining = false;
        for d in 10 {
            if !used[d] {
                continue;
            }
            remaining = true;
            var incoming = false;
            for x in 10 {
                if used[x] && edge[x][d] {
                    incoming = true;
                }
            }
            if !incoming {
                assert(chosen < 0);
                chosen = d;
            }
        }
        if !remaining {
            return answer;
        }
        assert(chosen >= 0);
        answer = answer * 10 + chosen;
        used[chosen] = false;
    }
}

fn main() {
    print(solve());
}
