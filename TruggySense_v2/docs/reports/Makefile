# -------- Configuration --------
TEXDIR=latex
TEX=main.tex
BASENAME=MAP_Kramp_Thomas_VGV
WEEK1_DATE=2026-01-05   # Sunday of Week 1

# -------- Date calculations --------
DATE_YMD := $(shell date -d "sunday" +%Y%m%d)
DATE_DMY := $(shell date -d "sunday" +%d/%m/%Y)

WEEK := $(shell echo $$(( \
	( $$(date -d "sunday" +%s) - $$(date -d "$(WEEK1_DATE)" +%s) ) / 604800 + 1 \
)))

# DATE_YMD=20260426
# DATE_DMY=26/04/2026
# WEEK=16

OUTDIR=report_$(WEEK)
OUTPDF=$(BASENAME)_$(DATE_YMD).pdf

# -------- Build --------
all:
	mkdir -p $(OUTDIR)
	cd $(TEXDIR) && xelatex -interaction=nonstopmode \
		"\\def\\ReportDate{$(DATE_DMY)} \
		\\def\\ReportWeek{$(WEEK)} \
		\\input{$(TEX)}"
	cp $(TEXDIR)/main.pdf $(OUTDIR)/$(OUTPDF)

clean:
	rm -f $(TEXDIR)/*.aux $(TEXDIR)/*.log $(TEXDIR)/*.out
