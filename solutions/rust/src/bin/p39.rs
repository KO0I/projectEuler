//! Project Euler 39: Integer Right Triangles.

fn solve() -> i64 {
    {
        let mut best_p = 0;
        let mut best_count = 0;
        for p in 12i64..=1000 {
            let mut count = 0;
            for a in 1..p / 3 {
                for b in a + 1..(p - a) / 2 {
                    let c = p - a - b;
                    if a * a + b * b == c * c {
                        count += 1;
                    }
                }
            }
            if count > best_count {
                best_count = count;
                best_p = p;
            }
        }
        best_p
    }
}

fn main() {
    println!("{}", solve());
}
