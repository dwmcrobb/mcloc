include ../../Makefile.vars

HEADERS         = $(shell ls *.hh)
PKGPREPHEADERS  = $(HEADERS:%=${STAGING}${PREFIXDIR}/include/libMcloc/%)

all::
	@echo "Nothing to be done for 'all'"

pkgprep: ${PKGPREPHEADERS}

${STAGING}${PREFIXDIR}/include/libMcloc/%.hh: %.hh
	../../install-sh -c -m 444 $< $@

clean: pkgprep-clean

distclean:: clean pkgprep-clean
	rm -f *~

pkgprep-clean::
	rm -f ${PKGPREPHEADERS}
