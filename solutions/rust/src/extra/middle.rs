//! Search, recurrence, graph, and exact-bigint Euler implementations.
use crate::extra_helpers::*;
use num_bigint::BigUint;
use num_traits::{One, Zero};
use std::collections::{HashMap, HashSet};

fn poker_value(cards: &[&str]) -> u64 {
    let mut counts=[0usize;15];let mut flush=true;
    let suit=cards[0].as_bytes()[1];
    for card in cards {let b=card.as_bytes();
        let value=b"23456789TJQKA".iter().position(|&c|c==b[0]).expect("rank")+2;
        counts[value]+=1;flush &= b[1]==suit;
    }
    let descending:Vec<usize>=(2..=14).rev().filter(|&i|counts[i]>0).collect();
    let mut pairs:Vec<usize>=descending.iter().copied().filter(|&i|counts[i]==2).collect();
    pairs.sort_unstable_by(|a,b|b.cmp(a));
    let triple=descending.iter().copied().find(|&i|counts[i]==3).unwrap_or(0);
    let quad=descending.iter().copied().find(|&i|counts[i]==4).unwrap_or(0);
    let straight=if descending.len()==5 && descending[0]-descending[4]==4 {descending[0]}
        else if descending==[14,5,4,3,2] {5} else {0};
    let mut key=Vec::new();
    let category;
    if straight>0 && flush {category=8;key.push(straight);}
    else if quad>0 {category=7;key.push(quad);key.push(*descending.iter().find(|&&v|v!=quad).unwrap());}
    else if triple>0 && pairs.len()==1 {category=6;key.push(triple);key.push(pairs[0]);}
    else if flush {category=5;key=descending;}
    else if straight>0 {category=4;key.push(straight);}
    else if triple>0 {category=3;key.push(triple);key.extend(descending.iter().filter(|&&v|v!=triple).copied());}
    else if pairs.len()==2 {category=2;key.extend(pairs);key.extend(descending.iter().filter(|&&v|counts[v]==1).copied());}
    else if pairs.len()==1 {category=1;key.push(pairs[0]);key.extend(descending.iter().filter(|&&v|counts[v]==1).copied());}
    else {category=0;key=descending;}
    let mut value=category;
    for n in key.into_iter().chain(std::iter::repeat(0)).take(5) {value=value*15+n;}
    value as u64
}
fn p59_score(cipher:&[u64],key:[u8;3])->Option<(i64,u64)> {
    const WEIGHTS:[i64;26]=[8,2,3,4,13,2,2,6,7,1,1,4,2,7,8,2,1,6,6,9,3,1,2,1,2,1];
    let mut score=0;let mut text=Vec::with_capacity(cipher.len());let mut sum=0;
    for (i,&c) in cipher.iter().enumerate() {let ch=(c as u8)^key[i%3];
        if !(32..=126).contains(&ch) {return None;}
        sum+=ch as u64;text.push(ch);
        score+= if ch==b' ' {15} else if ch.is_ascii_alphabetic() {WEIGHTS[(ch.to_ascii_lowercase()-b'a') as usize]}
            else if b".,;:'\"!?()-[]".contains(&ch) {-1}
            else if ch.is_ascii_digit() {-3} else {-12};
    }
    let mut matches=0;
    for word in [b" the ".as_slice(),b" and ",b" of ",b" to ",b" in ",b" that ",b" is ",b" a ",b" for ",b" with "] {
        for window in text.windows(word.len()) {if window==word {score+=100;matches+=1;}}
    }
    (matches>=3).then_some((score,sum))
}
fn p61() -> i64 {
    fn search(path:&mut Vec<u64>,used:u8,values:&[Vec<u64>])->Option<i64> {
        if path.len()==6 {return (path[5]%100==path[0]/100).then_some(path.iter().sum::<u64>() as i64);}
        let prefix=path.last().unwrap()%100;
        for kind in 0..6 {if used&(1<<kind)!=0 {continue;}
            for &v in &values[kind] {if v/100==prefix && !path.contains(&v) {
                path.push(v);
                if let Some(result)=search(path,used|(1<<kind),values) {return Some(result);}
                path.pop();
            }}
        }None
    }
    let mut values=vec![Vec::new();6];
    for (kind,out) in values.iter_mut().enumerate() {let sides=kind as i64+3;
        for n in 1i64.. {let v=((sides-2)*n*n-(sides-4)*n)/2;
            if v>=10000 {break;}if v>=1000&&v%100>=10 {out.push(v as u64);}
        }
    }
    for &v in &values[5] {let mut path=vec![v];
        if let Some(answer)=search(&mut path,1<<5,&values) {return answer;}
    }panic!("No polygonal cycle")
}
fn p68() -> i64 {
    fn search(inner:&mut [i64;5], depth:usize, used:u16,best:&mut u64) {
        if depth<5 {for value in 1..=9 {if used&(1<<value)!=0 {continue;}
            inner[depth]=value;search(inner,depth+1,used|(1<<value),best);
        }return;}
        for first in 1..=10i64 {if used&(1<<(first as u16))!=0 {continue;}
            let line=first+inner[0]+inner[1];let mut all=used;
            let mut outer=[0i64;5];let mut valid=true;
            for i in 0..5 {let candidate=line-inner[i]-inner[(i+1)%5];
                if candidate<first || candidate>10 || all&(1<<(candidate as u16))!=0 {valid=false;break;}
                outer[i]=candidate;all|=1<<(candidate as u16);
            }
            if valid {let mut representation=0;
                for i in 0..5 {representation=representation*(if outer[i]==10 {100}else{10})+(outer[i] as u64);
                    representation=representation*10+inner[i] as u64;representation=representation*10+inner[(i+1)%5] as u64;
                }
                *best=(*best).max(representation);
            }
        }
    }
    let mut best=0;search(&mut [0;5],0,0,&mut best);best as i64
}
fn p74_next(mut n:usize) -> usize {
    const FAC:[usize;10]=[1,1,2,6,24,120,720,5040,40320,362880];let mut sum=0;
    loop {sum+=FAC[n%10];n/=10;if n==0 {return sum;}}
}
fn p74() -> i64 {
    let len=7*362880+1;let mut length=vec![0u16;len];let mut seen=vec![0usize;len];
    let mut position=vec![0usize;len];let mut answer=0;
    for start in 1..1_000_000 {
        if length[start]==0 {let mut v=start;let mut path=Vec::new();
            while length[v]==0 && seen[v]!=start {seen[v]=start;position[v]=path.len();path.push(v);v=p74_next(v);}
            let mut end=path.len();let mut tail;
            if length[v]>0 {tail=length[v];}else {let first=position[v];tail=(path.len()-first) as u16;
                for &entry in &path[first..] {length[entry]=tail;}end=first;
            }
            for &entry in path[..end].iter().rev(){tail+=1;length[entry]=tail;}
        }
        if length[start]==60 {answer+=1;}
    }answer
}
fn p79() -> i64 {
    let mut edges=[[false;10];10];let mut present=[false;10];
    for line in include_str!("../../../../tests/data/p079_keylog.txt").lines() {
        let d:Vec<usize>=line.bytes().map(|b|(b-b'0') as usize).collect();assert_eq!(d.len(),3);
        for &v in &d {present[v]=true;}edges[d[0]][d[1]]=true;edges[d[1]][d[2]]=true;
    }
    let mut emitted=[false;10];let mut result=0;
    for _ in 0..present.iter().filter(|&&p|p).count() {
        let next=(0..10).find(|&v|present[v]&&!emitted[v]&&(0..10).all(|prior|!edges[prior][v]||emitted[prior])).expect("acyclic passcode");
        result=result*10+next as i64;emitted[next]=true;
    }result
}
fn p81() -> i64 {let input=matrix(include_str!("../../../../tests/data/p081_matrix.txt"));
    let mut cost=vec![0u64;input[0].len()];
    for (r,line) in input.iter().enumerate() {for (c,&value) in line.iter().enumerate(){
        if r==0 && c>0 {cost[c]=cost[c-1];}
        else if r>0&&c>0 {cost[c]=cost[c].min(cost[c-1]);}
        cost[c]+=value;
    }}cost[cost.len()-1] as i64
}
fn p82() -> i64 {let input=matrix(include_str!("../../../../tests/data/p082_matrix.txt"));
    let side=input.len();let mut cost:Vec<u64>=(0..side).map(|row|input[row][0]).collect();
    for c in 1..side {for r in 0..side {cost[r]+=input[r][c];}
        for r in 1..side {cost[r]=cost[r].min(cost[r-1]+input[r][c]);}
        for r in (0..side-1).rev() {cost[r]=cost[r].min(cost[r+1]+input[r][c]);}
    }*cost.iter().min().unwrap() as i64
}
fn p83() -> i64 {use std::cmp::Reverse;use std::collections::BinaryHeap;
    let input=matrix(include_str!("../../../../tests/data/p083_matrix.txt"));let side=input.len();
    let mut cost=vec![u64::MAX;side*side];cost[0]=input[0][0];
    let mut queue=BinaryHeap::new();queue.push((Reverse(cost[0]),0usize));
    while let Some((Reverse(d),u))=queue.pop() {if d!=cost[u]{continue;}
        if u==side*side-1 {return d as i64;}
        let (r,c)=(u/side,u%side);
        let neighbours=[(r.checked_sub(1),Some(c)),(Some(r+1).filter(|&v|v<side),Some(c)),
            (Some(r),c.checked_sub(1)),(Some(r),Some(c+1).filter(|&v|v<side))];
        for (nr,nc) in neighbours {if let (Some(nr),Some(nc))=(nr,nc) {let v=nr*side+nc;
            let nd=d+input[nr][nc];if nd<cost[v] {cost[v]=nd;queue.push((Reverse(nd),v));}
        }}
    }panic!("unreachable matrix destination")
}
fn p88() -> i64 {
    fn enumerate(product:usize,sum:usize,count:usize,first:usize, min:&mut [usize]){
        let k=product-sum+count;if k>=min.len(){return;}
        if k>=2 {min[k]=min[k].min(product);}
        for factor in first..=2*(min.len()-1)/product {
            enumerate(product*factor,sum+factor,count+1,factor,min);
        }
    }
    let limit=12000usize;let mut min=vec![2*limit;limit+1];
    enumerate(1,0,0,2,&mut min);
    min[2..].iter().copied().collect::<HashSet<_>>().into_iter().sum::<usize>() as i64
}
fn p90() -> i64 {let masks:Vec<u32>=(0..1u32<<10).filter(|m|m.count_ones()==6).collect();
    let mut count=0;for (i,&a) in masks.iter().enumerate(){for &b in &masks[i..] {
        let rev=(1<<6)|(1<<9);
        let a=if a&rev!=0 {a|rev}else{a};let b=if b&rev!=0 {b|rev}else{b};
        if [1,4,9,16,25,36,49,64,81].iter().all(|&s|{
            let (x,y)=(1<<(s/10),1<<(s%10));
            ((a&x!=0)&&(b&y!=0))||((a&y!=0)&&(b&x!=0))
        }){count+=1;}
    }}count
}
fn p95() -> i64 {
    const LIMIT:usize=1_000_000;
    let sums=proper_divisor_sums(LIMIT);let mut visited=vec![0usize;LIMIT+1];let mut depths=vec![0usize;LIMIT+1];
    let mut best=(0,usize::MAX);
    for start in 2..=LIMIT {if visited[start]!=0 {continue;}
        let mut n=start;let mut step=0;
        while n>0 && n<=LIMIT && visited[n]==0 {visited[n]=start;depths[n]=step;step+=1;n=sums[n];}
        if n==0 || n>LIMIT || visited[n]!=start {continue;}
        let length=step-depths[n];let mut minimum=n;let mut next=sums[n];
        while next!=n {minimum=minimum.min(next);next=sums[next];}
        if length>best.0 || length==best.0&&minimum<best.1 {best=(length,minimum);}
    }best.1 as i64
}
fn sudoku_solve(grid:&mut [u8;81],row:&mut [u16;9],col:&mut [u16;9],box_mask:&mut [u16;9],answers:&mut Vec<[u8;81]>){
    if answers.len()>=2{return;}
    let mut chosen=81;let mut options=0;let mut minimum=10;
    for i in 0..81 {if grid[i]!=0 {continue;}
        let (r,c)=(i/9,i%9);let b=r/3*3+c/3;
        let mask=0x3fe & !(row[r]|col[c]|box_mask[b]);let number=mask.count_ones();
        if number==0 {return;}if number<minimum {minimum=number;chosen=i;options=mask;}
    }
    if chosen==81 {answers.push(*grid);return;}
    let (r,c)=(chosen/9,chosen%9);let b=r/3*3+c/3;
    for digit in 1..=9 {let bit=1u16<<digit;if options&bit==0 {continue;}
        grid[chosen]=digit;row[r]|=bit;col[c]|=bit;box_mask[b]|=bit;
        sudoku_solve(grid,row,col,box_mask,answers);
        row[r]&=!bit;col[c]&=!bit;box_mask[b]&=!bit;grid[chosen]=0;
    }
}
fn p96() -> i64 {let mut lines=include_str!("../../../../tests/data/p096_sudoku.txt").lines();let mut sum=0;
    for index in 1..=50 {assert_eq!(lines.next().unwrap(),format!("Grid {index:02}"));
        let mut grid=[0u8;81];let mut row=[0u16;9];let mut col=[0u16;9];let mut bx=[0u16;9];
        for r in 0..9 {let text=lines.next().unwrap().as_bytes();assert_eq!(text.len(),9);
            for c in 0..9 {let d=text[c]-b'0';grid[r*9+c]=d;
                if d>0 {let bit=1u16<<d;let b=r/3*3+c/3;
                    assert_eq!((row[r]|col[c]|bx[b])&bit,0);row[r]|=bit;col[c]|=bit;bx[b]|=bit;
                }
            }
        }
        let mut answers=Vec::new();sudoku_solve(&mut grid,&mut row,&mut col,&mut bx,&mut answers);
        assert_eq!(answers.len(),1);sum+=(100*answers[0][0] as i64+10*answers[0][1] as i64+answers[0][2] as i64);
    }assert!(lines.next().is_none());sum
}
fn p101() -> i64 {fn sequence(n:i128)->i128 {let mut v=1;for _ in 0..10 {v=1-n*v;}v}
    fn fit(first:&[i128],n:usize)->i128 {let mut sum=0;let mut choose=1i128;
        for (j,&v) in first.iter().enumerate() {sum+=v*choose;
            if j+1<first.len() {choose=choose*(n-1-j) as i128/(j+1) as i128;}
        }sum
    }
    let mut total=0;
    for k in 1..=10 {let mut diffs:Vec<i128>=(1..=k).map(|v|sequence(v as i128)).collect();
        let mut first=Vec::new();for order in 0..k {first.push(diffs[0]);
            for i in 0..k-order-1 {diffs[i]=diffs[i+1]-diffs[i];}
        }
        let mut at=k+1;while fit(&first,at)==sequence(at as i128) {at+=1;}
        total+=fit(&first,at);
    }total as i64
}
fn p102() -> i64 {include_str!("../../../../tests/data/p102_triangles.txt").lines().filter(|line|{
    let p:Vec<i64>=line.split(',').map(|x|x.parse().expect("coord")).collect();assert_eq!(p.len(),6);
    let a=p[0]*p[3]-p[1]*p[2];let b=p[2]*p[5]-p[3]*p[4];let c=p[4]*p[1]-p[5]*p[0];
    (a>0&&b>0&&c>0)||(a<0&&b<0&&c<0)
}).count() as i64}
fn p105() -> i64 {
    include_str!("../../../../tests/data/p105_sets.txt").lines().map(|line|{
        let mut values:Vec<u64>=line.split(',').map(|v|v.parse().unwrap()).collect();values.sort_unstable();
        if !special_sum_set(&values) {0}else{values.iter().sum::<u64>() as i64}
    }).sum()
}
fn special_sum_set(values:&[u64])->bool {
    // Strictly increasing sorted positive values: compare all distinct
    // cardinalities by the boundary sums, then equal-cardinality subset sums.
    for k in 1..=(values.len()/2) {if values[..k+1].iter().sum::<u64>()<=values[values.len()-k..].iter().sum::<u64>(){return false;}}
    let mut sums:HashMap<(usize,u64),Vec<u64>>=HashMap::new();
    for mask in 1..(1u64<<values.len()) {let mut sum=0;let mut count=0;
        for (i,&v) in values.iter().enumerate(){if mask&(1<<i)!=0 {count+=1;sum+=v;}}
        let entry=sums.entry((count,sum)).or_default();
        if entry.iter().any(|prior|mask&prior==0) {return false;}entry.push(mask);
    }true
}
fn p107() -> i64 {let input:Vec<Vec<u64>>=include_str!("../../../../tests/data/p107_network.txt").lines()
        .map(|line|line.split(',').map(|v|if v=="-" {0}else{v.parse().unwrap()}).collect()).collect();
    let n=input.len();assert_eq!(n,40);let mut total=0;
    for i in 0..n {for j in i+1..n {total+=input[i][j];assert_eq!(input[i][j],input[j][i]);}}
    let mut in_tree=vec![false;n];let mut best=vec![u64::MAX;n];best[0]=0;
    let mut weight=0;
    for _ in 0..n {let u=(0..n).filter(|&i|!in_tree[i]).min_by_key(|&i|best[i]).unwrap();
        assert!(best[u]!=u64::MAX);weight+=best[u];in_tree[u]=true;
        for v in 0..n {if !in_tree[v]&&input[u][v]>0 {best[v]=best[v].min(input[u][v]);}}
    }(total-weight) as i64
}
fn p108_110(limit:u64)->i64 {
    const PS:[u64;15]=[2,3,5,7,11,13,17,19,23,29,31,37,41,43,47];
    fn walk(index:usize,max_exp:u32,value:u64,divs:u64,target:u64,best:&mut u64) {
        if divs>target {*best=(*best).min(value);return;}
        if index>=PS.len() {return;}
        let mut v=value;
        for exp in 1..=max_exp {if v>(*best-1)/PS[index] {break;}v*=PS[index];
            let factor=2*exp as u64+1;let next=if divs>target/factor {target+1}else{divs*factor};
            walk(index+1,exp,v,next,target,best);
        }
    }
    let target=2*limit;let mut best=1u64;let mut count=1u64;
    for &prime in &PS {if count>target {break;}
        best*=prime;count*=3;
    }
    assert!(count>target);walk(0,63,1,1,target,&mut best);best as i64
}
fn p111() -> i64 {
    fn recurse(position:u32,length:u32,digit:u64,changes:u32,value:u64,count:&mut u64,sum:&mut u64){
        if changes>length-position {return;}
        if position==length {if changes==0 && prime(value){*count+=1;*sum+=value;}return;}
        if digit!=0 || position!=0 {recurse(position+1,length,digit,changes,value*10+digit,count,sum);}
        if changes>0 {for d in if position==0 {1}else{0}..=9 {if d as u64!=digit {
            recurse(position+1,length,digit,changes-1,value*10+d as u64,count,sum);
        }}}
    }
    let mut total=0;
    for digit in 0..=9 {for changes in 0..=10 {let (mut count,mut sum)=(0,0);
            recurse(0,10,digit,changes,0,&mut count,&mut sum);
            if count>0 {total+=sum;break;}
        }
    }total as i64
}
fn p118() -> i64 {
    fn enumerate(mask:usize,value:u64,digit_sum:u32,counts:&mut [u64;512]){
        if mask!=0 && (matches!(value,2|3|5|7) || (digit_sum%3!=0 && value%2!=0 && value%5!=0 && prime(value))) {
            counts[mask]+=1;
        }
        for digit in 1..=9u64 {let bit=1usize<<(digit-1);
            if mask&bit==0 {enumerate(mask|bit,value*10+digit,digit_sum+digit as u32,counts);}
        }
    }
    fn partitions(mask:usize,counts:&[u64;512],cache:&mut [Option<u64>;512])->u64{
        if mask==0 {return 1;}
        if let Some(v)=cache[mask] {return v;}
        let lowest=mask&(!mask+1);let mut subtotal=0;let mut subset=mask;
        while subset>0 {if subset&lowest!=0 && counts[subset]>0 {
            subtotal+=counts[subset]*partitions(mask^subset,counts,cache);
        }subset=(subset-1)&mask;}
        cache[mask]=Some(subtotal);subtotal
    }
    let mut counts=[0u64;512];enumerate(0,0,0,&mut counts);
    partitions(511,&counts,&mut [None;512]) as i64
}
fn p119() -> i64 {for size in 2..=19u64 {let cutoff=10u64.pow(size as u32);
        let mut values=Vec::new();for base in 2..=9*size {let mut v=base*base;
            while v<cutoff {if v>=10 && digits(v).iter().map(|&v|v as u64).sum::<u64>()==base {values.push(v);}
                if v>(cutoff-1)/base {break;}v*=base;
            }
        }values.sort_unstable();values.dedup();if values.len()>=30 {return values[29] as i64;}
    }panic!("30th digit-power term not found")
}
pub fn solve(n:u32)->Option<i64> {
    Some(match n {
        54 => {let mut sum=0;
            for line in include_str!("../../../../tests/data/p054_poker.txt").lines() {
                let cards:Vec<&str>=line.split_whitespace().collect();assert_eq!(cards.len(),10);
                if poker_value(&cards[..5])>poker_value(&cards[5..]) {sum+=1;}
            }sum},
        59 => {let cipher=parse_numbers(include_str!("../../../../tests/data/p059_cipher.txt"));
            let mut best=None;let mut tied=false;
            for a in b'a'..=b'z' {for b in b'a'..=b'z' {for c in b'a'..=b'z' {
                if let Some((score,sum))=p59_score(&cipher,[a,b,c]) {
                    if best.is_none_or(|(prev,_)|(score>prev)) {best=Some((score,sum));tied=false;}
                    else if best.is_some_and(|(prev,_)|score==prev) {tied=true;}
                }
            }}}
            assert!(!tied,"Tied top English ciphertext key");best.expect("unique English plaintext").1 as i64},
        61 => p61(),
        62 => {let mut bucket=Vec::<(u64,u64)>::new();let mut magnitude=10u64;
            let mut answer=0;
            for n in 1.. {let cube=n*n*n;
                if cube>=magnitude {let mut groups:HashMap<u64,Vec<u64>>=HashMap::new();
                    for &(signature,value) in &bucket {groups.entry(signature).or_default().push(value);}
                    if let Some(best)=groups.values().filter(|v|v.len()==5).flat_map(|v|v.iter()).min(){answer=*best;break;}
                    bucket.clear();magnitude*=10;
                }
                bucket.push((decimal_signature(cube),cube));
            }answer as i64},
        66 => {let mut best=BigUint::zero();let mut answer=0;
            for d in 2..=1000u64 {let a0=isqrt(d);if a0*a0==d {continue;}
                let (mut m,mut den,mut a)=(0u64,1u64,a0);
                let (mut p0,mut p1,mut q0,mut q1)=(BigUint::zero(),BigUint::one(),BigUint::one(),BigUint::zero());
                loop {let p=&p1*a+&p0;let q=&q1*a+&q0;
                    if &p*&p==BigUint::one()+BigUint::from(d)*&q*&q {
                        if p>best {best=p;answer=d;}break;
                    }
                    p0=p1;p1=p;q0=q1;q1=q;
                    m=den*a-m;den=(d-m*m)/den;a=(a0+m)/den;
                }
            }answer as i64},
        68 => p68(),
        74 => p74(),
        79 => p79(),
        80 => {let scale=BigUint::from(10u32).pow(198);let mut sum=0usize;
            for n in 1..=100u64 {if is_square(n) {continue;}
                let root=big_isqrt(&(BigUint::from(n)*&scale));
                let text=root.to_string();assert_eq!(text.len(),100);
                sum+=text.bytes().map(|b|(b-b'0') as usize).sum::<usize>();
            }sum as i64},
        81 => p81(),82 => p82(),83 => p83(),
        88 => p88(),90 => p90(),95 => p95(),96 => p96(),
        101 => p101(),102 => p102(),104 => {let mut prev=1u64;let mut curr=1u64;
            let mut index=2u64;loop {index+=1;
                let next=(prev+curr)%1_000_000_000;prev=curr;curr=next;
                if pandigital_1_to_9(curr) {
                    let text=fibonacci_pair(index).0.to_string();
                    if text.len()>=9 && pandigital_1_to_9(text[..9].parse().unwrap()) {break index as i64;}
                }
            }},
        105 => p105(),107 => p107(),108 => p108_110(1000),110 => p108_110(4_000_000),
        111 => p111(),118 => p118(),119 => p119(),
        _ => return None,
    })
}
#[cfg(test)] mod tests {
    use super::*;
    #[test]fn poker_hands(){
        assert!(poker_value(&["5H","5C","6S","7S","KD"])<poker_value(&["2C","3S","8S","8D","TD"]));
        assert!(poker_value(&["2D","9C","AS","AH","AC"])>poker_value(&["3D","6D","7D","TD","QD"]));
        assert!(poker_value(&["2C","3C","4C","5C","AC"])>poker_value(&["9D","9S","9H","9C","3S"]));
    }
    #[test] fn minimization(){assert_eq!(p108_110(4),6);}
}
