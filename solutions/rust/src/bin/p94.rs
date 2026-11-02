//! Project Euler 94: Almost Equilateral Triangles.

fn solve() -> i64 {
    {
        let mut sum = 0i64;
        let (mut x, mut y) = (2i64, 0i64);
        loop {
            let (nx, ny) = (2 * x + 3 * y, x + 2 * y);
            x = nx;
            y = ny;
            let perimeter = if x % 3 == 2 { x + 2 } else { x - 2 };
            if perimeter > 1_000_000_000 {
                break;
            }
            let a = if x % 3 == 2 { (x + 1) / 3 } else { (x - 1) / 3 };
            let b = if x % 3 == 2 { a + 1 } else { a - 1 };
            if perimeter > 0 && a > 1 && b > 0 && (b * y) % 4 == 0 {
                sum += perimeter;
            }
        }
        sum
    }
}

fn main() {
    println!("{}", solve());
}
