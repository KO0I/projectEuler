//! Reflect the direction vector across the ellipse normal and intersect again.
fn solve() -> u32 {
    let (mut x0, mut y0, mut x, mut y) = (0.0f64, 10.1, 1.4, -9.6);
    let mut count = 0;
    loop {
        let (vx, vy) = (x - x0, y - y0);
        let (nx, ny) = (4.0 * x, y);
        let scale = 2.0 * (vx * nx + vy * ny) / (nx * nx + ny * ny);
        let (rx, ry) = (vx - scale * nx, vy - scale * ny);
        let t = -(8.0 * x * rx + 2.0 * y * ry) / (4.0 * rx * rx + ry * ry);
        (x0, y0) = (x, y);
        x += t * rx;
        y += t * ry;
        count += 1;
        if x.abs() <= 0.01 && y > 0.0 {
            break;
        }
    }
    count
}
fn main() {
    println!("{}", solve());
}
