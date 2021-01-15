include ../Makefile.vars

all::
	${MAKE} -C mcloc
	${MAKE} -C qmcloc

pkgprep::
	${MAKE} -C mcloc $@
	${MAKE} -C qmcloc $@

clean::
	${MAKE} -C mcloc $@
	-${MAKE} -C qmcloc $@

distclean::
	${MAKE} -C mcloc $@
	${MAKE} -C qmcloc $@

pkgprep-clean::
	${MAKE} -C mcloc $@
	${MAKE} -C qmcloc $@

