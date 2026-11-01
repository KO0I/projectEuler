//! Project Euler 65: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(65).expect("implemented Euler problem");
    println!("{answer}");
}
