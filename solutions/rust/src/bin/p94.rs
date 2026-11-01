//! Project Euler 94: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(94).expect("implemented Euler problem");
    println!("{answer}");
}
