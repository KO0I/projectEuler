//! Project Euler 116: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(116).expect("implemented Euler problem");
    println!("{answer}");
}
