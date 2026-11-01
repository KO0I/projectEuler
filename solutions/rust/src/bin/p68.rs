//! Project Euler 68: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(68).expect("implemented Euler problem");
    println!("{answer}");
}
