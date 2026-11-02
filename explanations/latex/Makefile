TEX = project_euler_c_explanations.tex
PDF = build/project_euler_c_explanations.pdf
PYTHON ?= python3
GENERATOR = ../../scripts/generate_report_appendices.py
GENERATED = generated/problem_walkthroughs.tex generated/library_api.tex

.PHONY: all clean
all: $(PDF)

# The first pass writes an empty-to-populated TOC; the second inserts it and
# shifts chapter pages; the third consumes the corrected page references.
$(GENERATED): $(GENERATOR) ../../tests/answers.json $(wildcard ../../solutions/c/*.c) $(wildcard ../../solutions/rust/src/bin/*.rs) $(wildcard ../../solutions/forth/p*.fs) $(wildcard ../../solutions/factor/p*.factor)
	$(PYTHON) $(GENERATOR)

$(PDF): $(TEX) $(GENERATED) $(wildcard chapters/*.tex) $(wildcard ../../build/math_plots/*.pdf) $(wildcard ../../build/profile/latest-plots/*.pdf) Makefile
	mkdir -p build
	pdflatex -interaction=nonstopmode -halt-on-error -output-directory=build $(TEX)
	pdflatex -interaction=nonstopmode -halt-on-error -output-directory=build $(TEX)
	pdflatex -interaction=nonstopmode -halt-on-error -output-directory=build $(TEX)

clean:
	rm -rf build
