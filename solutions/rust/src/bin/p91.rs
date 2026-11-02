//! Project Euler 91: Right Triangles with Integer Coordinates.

fn solve() -> i64 {
    {
        let mut count = 0;
        for x1 in 0i64..=50 {
            for y1 in 0i64..=50 {
                if x1 == 0 && y1 == 0 {
                    continue;
                }
                for x2 in 0i64..=50 {
                    for y2 in 0i64..=50 {
                        if x2 == 0 && y2 == 0 || x1 == x2 && y1 == y2 {
                            continue;
                        }
                        let dot0 = x1 * x2 + y1 * y2;
                        let dot1 = x1 * (x1 - x2) + y1 * (y1 - y2);
                        let dot2 = x2 * (x2 - x1) + y2 * (y2 - y1);
                        if dot0 == 0 || dot1 == 0 || dot2 == 0 {
                            count += 1;
                        }
                    }
                }
            }
        }
        count / 2
    }
}

fn main() {
    println!("{}", solve());
}
