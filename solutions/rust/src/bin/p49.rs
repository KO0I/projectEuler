//! Project Euler 49: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(49).expect("implemented Euler problem");
    println!("{answer}");
}
