// Project Euler 62: Cubic Permutations.
import euler from "../../support/goose";

struct Cube {
    key: i64, value: i64,
}

fn solve() -> i64 {
    var cubes: Cube[>..] = [];
    var len = 1;
    var n = 1;
    loop {
        let c = n * n * n;
        if digits(c) > len {
            sort(cubes[..]) {
                a, b => a.key < b.key
            };
            var i = 0;
            var best = 1000000000000000000;
            while i < cubes.len {
                var j = i + 1;
                var small = cubes[i].value;
                while j < cubes.len && cubes[j].key == cubes[i].key {
                    small = min(small, cubes[j].value);
                    j++;
                }
                if j - i == 5 {
                    best = min(best, small);
                }
                i = j;
            }
            if best < 1000000000000000000 {
                return best;
            }
            len = digits(c);
            cubes = [];
        }
        cubes.push(Cube {
            signature(c), c,
        },
        );
        n++;
    }
}

fn main() {
    print(solve());
}
