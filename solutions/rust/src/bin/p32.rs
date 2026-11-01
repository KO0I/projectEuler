//! Project Euler 32: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(32).expect("implemented Euler problem");
    println!("{answer}");
}
