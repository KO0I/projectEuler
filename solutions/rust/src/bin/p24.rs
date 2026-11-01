//! Project Euler 24: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(24).expect("implemented Euler problem");
    println!("{answer}");
}
