\ProvidesClass{hw}
 
\LoadClass[12pt,letterpaper,answers]{exam}
\RequirePackage{xspace}
\newcommand{\course}[1]{\def\@course{#1\xspace}}
\newcommand{\problemlist}[1]{\begin{center}\large #1 \end{center}}
 
\makeatletter
\renewcommand{\maketitle}{
\newpage
 \null
 \firstpageheader{}{}{}
 \runningheader{\@author}{\@course \@title}{\@date}
 \begin{flushright}
  \@author \\ \@course \\ \@title \\ \@date
 \end{flushright}
} \makeatother
