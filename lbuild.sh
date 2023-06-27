#!/bin/sh
cd ./latex-report/
xelatex report.tex
biber report
xelatex report.tex
xelatex report.tex
