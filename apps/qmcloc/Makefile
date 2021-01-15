include ../../Makefile.vars

FBSDICON  = usr/local/share/icons/hicolor/scalable/apps/qmcloc.svg
LINUXICON = usr/share/icons/hicolor/scalable/apps/qmcloc.svg
MACOSAPP  = qmcloc.app/Contents/MacOS/qmcloc

all:: Makefile.qmake
	${MAKE} -f $<

clean:: Makefile.qmake
	${MAKE} -f $< $@

Makefile.qmake: qmcloc.pro
	  qmake -o Makefile.qmake

pkgprep:: all pkgprep-${OSNAME}

pkgprep-darwin:: ${STAGING}/Applications/${MACOSAPP}

${STAGING}/Applications/${MACOSAPP}: ${MACOSAPP}
	../../install-sh -d ${STAGING}/Applications
	tar -cf - qmcloc.app | (cd ${STAGING}/Applications && tar -xvf -)

pkgprep-freebsd: ${STAGING}${PREFIXDIR}/bin/qmcloc ${STAGING}/${FBSDICON}

pkgprep-linux: ${STAGING}${PREFIXDIR}/bin/qmcloc ${STAGING}/${LINUXICON}

${STAGING}${PREFIXDIR}/bin/qmcloc: qmcloc
	../../install-sh -s -c -m 555 $< $@

${STAGING}/${LINUXICON}: resources/images/icon.svg
	../../install-sh -c -m 444 $< $@

${STAGING}/${FBSDICON}: resources/images/icon.svg
	../../install-sh -c -m 444 $< $@

distclean:: clean pkgprep-clean
	rm -f Makefile.qmake *~

pkgprep-clean:: pkgprep-${OSNAME}-clean

pkgprep-freebsd-clean::
	rm -f ${STAGING}${PREFIXDIR}/bin/qmcloc ${STAGING}/${FBSDICON}

pkgprep-linux-clean::
	rm -f ${STAGING}${PREFIXDIR}/bin/qmcloc ${STAGING}/${LINUXICON}

pkgprep-darwin-clean::
	rm -f ${STAGING}/Applications/${MACOSAPP}

