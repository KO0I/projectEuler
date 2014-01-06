//  Solved without Computer!

//  Brute method: evaluate every 5 consecutive digits and compare
//    This would take 195 evaluations to be complete
//
//  Better method: the largest possible product would be 9^5, or 59049, so
//    for any block of 1000 digits to sift through, first look for a
//    string of 5 number 9s. 
//    Decrement the sequence... look for 9,9,9,9,8 and then 9,9,9,9,7 and so
//    on until a match is found. Compute the product once and then evaluate.
//    One computation, but lots of searching required.
//
//  Non-program method I used.
//    Search for the longest string(s) of 9's. Then append the end of the
//    string with 8. If no match, then 7,6...1 until a match is found.
//      If all sequences of n 9's match 0, then try a smaller string of 9's.
//      Else, continue finding the sequence of numbers that have the largest
//      non-zero values.
//      Corner case: if there are a lot of zeros in the list of given numbers,
//      then the output number will be rather small.
