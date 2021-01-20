include ../../Makefile.vars

OBJFILES	= TestCocomoConfig.o \
		  TestCppScanner.o \
		  TestDScanner.o \
		  TestFlexScanner.o \
		  TestLuaScanner.o \
		  TestRustScanner.o
OBJDEPS		= $(OBJFILES:%.o=deps/%_deps)
OBJDEPS         += deps/DwmMclocSingleFileTests_deps \
		   deps/MclocUnitAssert_deps
TESTS		= $(OBJFILES:%.o=%)
INCS		= -I../include -I.
LIBS		= -L../lib -lMcloc -ltermcap

all: ${TESTS}

runtests: all
	@ for tp in ${TESTS} ; do \
		printf "%-36s " $$tp ; \
		out=`./$$tp` ; \
		if [ $$? -eq 0 ]; then \
			printf "%25s\n" "$$out" ; \
		else \
			printf "\n%s" "$$out" ; \
		fi ; \
	done

../lib/libMcloc.a::
	${MAKE} -C ../src

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

Test%: Test%.o DwmMclocSingleFileTests.o MclocUnitAssert.o ../lib/libMcloc.a
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $@ $^ ${LIBS}

%.o: %.cc deps/%_deps
	${CXX} ${CXXFLAGS} ${INCS} -c $< -o $@

clean::
	rm -f ${OBJFILES} ${OBJDEPS} ${TESTS}
	rm -f DwmMclocSingleFileTests.o MclocUnitAssert.o

distclean:: clean pkgprep-clean
	rm -f *~

pkgprep-clean::
	@echo "Nothing to be done for 'pkgprep-clean'"

