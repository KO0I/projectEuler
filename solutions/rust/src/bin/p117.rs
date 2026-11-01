//! Project Euler 117: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(117).expect("implemented Euler problem");
    println!("{answer}");
}
