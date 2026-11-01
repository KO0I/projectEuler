//! Project Euler 53: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(53).expect("implemented Euler problem");
    println!("{answer}");
}
