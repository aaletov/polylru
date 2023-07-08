#!/bin/sh
set -e
cd ./latex-report/
xelatex report.tex
biber report
xelatex report.tex
xelatex report.tex
