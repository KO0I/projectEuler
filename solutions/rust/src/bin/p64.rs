//! Project Euler 64: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(64).expect("implemented Euler problem");
    println!("{answer}");
}
