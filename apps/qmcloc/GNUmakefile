include ../../Makefile.vars

LFLAGS := $(LFLAGS:-Wl,-sdk_version=)
LFLAGS := $(LFLAGS:-Wl,12.3=)

ifeq ("${OSNAME}","darwin")
PKGTARGETS = ${STAGING}${PREFIXDIR}/../../Applications/qmcloc.app
else
PKGTARGETS = ${STAGING}${PREFIXDIR}/bin/qmcloc
endif

ifneq ("$(shell test -e ./Makefile && echo ex)","ex")
Makefile: qmcloc.pro
	${QMAKE} $<
endif

include Makefile

pkgprep: ${PKGTARGETS}

${STAGING}${PREFIXDIR}/bin/qmcloc: qmcloc
	../../install-sh -s -c -m 555 $< $@

${STAGING}${PREFIXDIR}/../../Applications/qmcloc.app: qmcloc.app/Contents/MacOS/qmcloc
	-mkdir ${STAGING}/Applications
	tar -cvf - qmcloc.app | (cd ${STAGING}/Applications && tar -xvf -)

clean: clean-tarprep

clean-tarprep::
	rm -Rf ${PKGTARGETS}

distclean: clean
