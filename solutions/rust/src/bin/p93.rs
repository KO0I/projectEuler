//! Project Euler 93: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(93).expect("implemented Euler problem");
    println!("{answer}");
}
