include Makefile.vars

PKGTARGETS = ${STAGING}${PREFIXDIR}/etc/mcloc.cfg \
             ${STAGING}${PREFIXDIR}/etc/mcloc_cocomo1_intermediate.cfg

all::
	${MAKE} -C classes
	${MAKE} -C apps

pkgprep:: ${PKGTARGETS}
	${MAKE} -C classes $@
	${MAKE} -C apps $@
	${MAKE} -C docs $@

package: ${OSNAME}-pkg

darwin-pkg: pkgprep
	pkgbuild --root staging --component-plist mcloc.plist --identifier net.mcplex.mcloc --version ${VERSION} mcloc-${VERSION}.pkg

freebsd-pkg: pkgprep
	mkfbsdmnfst -r packaging/fbsd_manifest -s staging > staging/+MANIFEST
	pkg create -o . -r staging -m staging

linux-pkg: pkgprep
	if [ ! -d staging/DEBIAN ]; then mkdir staging/DEBIAN; fi
	mkdebcontrol -r packaging/debcontrol -s staging > staging/DEBIAN/control
	dpkg-deb -b --root-owner-group staging
	dpkg-name -o staging.deb

${STAGING}${PREFIXDIR}/etc/mcloc.cfg: etc/mcloc.cfg
	./install-sh -c -m 644 $< $@

${STAGING}${PREFIXDIR}/etc/mcloc_cocomo1_intermediate.cfg: etc/mcloc_cocomo1_intermediate.cfg
	./install-sh -c -m 644 $< $@

clean::
	${MAKE} -C classes $@
	${MAKE} -C apps $@
	${MAKE} -C docs $@

distclean:: clean
	${MAKE} -C docs $@
	rm -Rf ${STAGING}/*
	rm -f Makefile.vars packaging/fbsd_manifest
	rm -f mcloc-${VERSION}.pkg mcloc-*\.txz mcloc_*.deb mcloc.list
	rm -Rf autom4te.cache config.log config.status

pkgprep-clean::
	${MAKE} -C classes $@
	${MAKE} -C apps $@
	${MAKE} -C docs $@

