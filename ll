\documentclass{article}
\title{Project Euler Problem 17\\Number Letter Counts}
\author{Patrick Harrington}
\usepackage{amsmath}

\begin{document}
\maketitle
\section{Problem Statement}
 If the numbers 1 to 5 are written out in words: one, two, three, four,
 five, then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total.

 If all the numbers from 1 to 1000 (one thousand) inclusive were written out
 in words, how many letters would be used?


 NOTE: Do not count spaces or hyphens. For example, 342 (three hundred and
 forty-two) contains 23 letters and 115 (one hundred and fifteen) contains 20
 letters. The use of ``and'' when writing out numbers is in compliance with
 British usage.

\section{Method}
  Since we are converting each number to a string in the range {1,1000}, we
  might as well save ourselves a great deal of work and find common quantities.
  \subsection{Redifining the numbers}
  \begin{center}
  \begin{tabular}[center]{c|c}
    Number  & Number of Letters \\
    \hline
    One & 3\\
    Two & 3\\
    Three & 5\\
    Four & 4\\
    Five & 4\\
    Six & 3\\
    Seven & 5\\
    Eight & 5\\
    Nine & 4\\
  \end{tabular}
\end{center}

\end{document}
