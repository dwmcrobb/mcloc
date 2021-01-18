include ../../Makefile.vars

INCS		= -I../../classes/include -I.
OBJFILES	= mcloc.o
OBJDEPS         = $(OBJFILES:%.o=deps/%_deps)
PKGTARGETS 	= ${STAGING}${PREFIXDIR}/bin/mcloc

mcloc: ${OBJFILES} ../../classes/lib/libMcloc.a
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ $^ ${TERMLIB} ${OSLIBS}

#  dependency rule
deps/%_deps: %.cc
	@echo "making dependencies for $<"
	@set -e; \
	${CXX} -MM ${CXXFLAGS} ${INCS} -c $< | \
	 sed 's/\($*\)\.o[ :]*/\1.o $(@D)\/$(@F) : /g' > $@ ; [ -s $@ ] || \
	 rm -f $@

#  only include dependency makefiles if target is not 'clean' or 'distclean'
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include ${OBJDEPS}
endif
endif

mcloc.o: mcloc.cc
	${CXX} ${CXXFLAGS} ${INCS} -c $<

../../classes/lib/libMcloc.a::
	${MAKE} -C ../../classes/src

pkgprep:: ${PKGTARGETS}

${STAGING}${PREFIXDIR}/bin/mcloc: mcloc
	../../install-sh -s -c -m 555 $< $@

clean::
	rm -f mcloc ${OBJFILES} ${OBJDEPS}

distclean:: clean pkgprep-clean
	rm -f *~

pkgprep-clean::
	rm -f ${PKGTARGETS}
