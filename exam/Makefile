OBJS = exam_solution.tex

.PHONY: all clean cleanup

all: $(OBJS)
	pdflatex $(OBJS)
	biber $(basename $(OBJS))
	pdflatex $(OBJS)

cleanup:
	rm -f *.aux *.log *.out *.toc

clean:
	rm -f *.aux *.log *.out *.toc *.pdf



