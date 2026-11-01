TEX = project_euler_c_explanations.tex
PDF = build/project_euler_c_explanations.pdf

.PHONY: all clean
all: $(PDF)

$(PDF): $(TEX) $(wildcard chapters/*.tex)
	mkdir -p build
	pdflatex -interaction=nonstopmode -halt-on-error -output-directory=build $(TEX)
	pdflatex -interaction=nonstopmode -halt-on-error -output-directory=build $(TEX)

clean:
	rm -rf build
