//! Project Euler 84: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(84).expect("implemented Euler problem");
    println!("{answer}");
}
