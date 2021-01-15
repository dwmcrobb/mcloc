include ../../Makefile.vars

PKGTARGETS = ${STAGING}${PREFIXDIR}/man/man1/mcloc.1

all: mcloc.html

pkgprep:: ${PKGTARGETS}

${STAGING}${PREFIXDIR}/man/man1/mcloc.1: mcloc.1
	../../install-sh -c -m 644 $< $@

mcloc.html: mcloc.1 mcloc.css
	mandoc -T html -O style=./mcloc.css $< > $@

clean::
	rm -f mcloc.html

distclean:: clean pkgprep-clean

pkgprep-clean::
	rm -f ${PKGTARGETS}
