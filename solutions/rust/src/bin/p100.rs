//! Project Euler 100: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(100).expect("implemented Euler problem");
    println!("{answer}");
}
