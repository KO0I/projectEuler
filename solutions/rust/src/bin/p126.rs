//! Count every cuboid layer below a finite bound.
fn solve() -> usize {
    const L: usize = 30_000;
    let mut count = vec![0u16; L];
    for a in 1.. {
        if 6 * a * a >= L {
            break;
        }
        for b in a.. {
            if 2 * (a * b + a * b + b * b) >= L {
                break;
            }
            for c in b.. {
                let first = 2 * (a * b + a * c + b * c);
                if first >= L {
                    break;
                }
                let mut cubes = first;
                let mut layer = 1;
                while cubes < L {
                    count[cubes] += 1;
                    cubes += 4 * (a + b + c) + 8 * (layer - 1);
                    layer += 1;
                }
            }
        }
    }
    (1..L).find(|&n| count[n] == 1000).unwrap()
}
fn main() {
    println!("{}", solve());
}
