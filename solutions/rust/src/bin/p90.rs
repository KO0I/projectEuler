//! Project Euler 90: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(90).expect("implemented Euler problem");
    println!("{answer}");
}
