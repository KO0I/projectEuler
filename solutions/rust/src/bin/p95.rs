//! Project Euler 95: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(95).expect("implemented Euler problem");
    println!("{answer}");
}
