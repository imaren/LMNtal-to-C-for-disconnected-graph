all:
	latexmk tex/main.tex

image:
	docker build -t my-texlive-ja .

ondocker: image
	docker run --rm -v ${PWD}:/workdir my-texlive-ja latexmk main.tex

clean:
	latexmk -C tex/main.tex
	rm -f main.bbl tex/__latexindent_temp.tex *.aux *.fdb_latexmk *.fls *.log

