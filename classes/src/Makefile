include ../../Makefile.vars

OBJFILES	= DwmMclocAdaFileScanner.o \
		  DwmMclocClojureFileScanner.o \
		  DwmMclocCmakeFileScanner.o \
		  DwmMclocConfig.o \
		  DwmMclocConfigLex.o \
		  DwmMclocConfigParse.o \
		  DwmMclocCppFileScanner.o \
		  DwmMclocCssFileScanner.o \
		  DwmMclocCodeUtils.o \
		  DwmMclocDFileScanner.o \
		  DwmMclocErlangFileScanner.o \
		  DwmMclocFlexFileScanner.o \
		  DwmMclocFortranFileScanner.o \
		  DwmMclocHaskellFileScanner.o \
		  DwmMclocHtmlFileScanner.o \
		  DwmMclocJuliaFileScanner.o \
		  DwmMclocKotlinFileScanner.o \
		  DwmMclocLanguage.o \
		  DwmMclocLispFileScanner.o \
		  DwmMclocLuaFileScanner.o \
		  DwmMclocM4FileScanner.o \
		  DwmMclocManpageFileScanner.o \
		  DwmMclocNoCommentsFileScanner.o \
		  DwmMclocPerlFileScanner.o \
		  DwmMclocPrologFileScanner.o \
		  DwmMclocPythonFileScanner.o \
		  DwmMclocRecentsLexer.o \
		  DwmMclocRecentsParser.o \
		  DwmMclocRubyFileScanner.o \
		  DwmMclocRustFileScanner.o \
		  DwmMclocScanWorker.o \
		  DwmMclocShellFileScanner.o \
		  DwmMclocSourceCollection.o \
		  DwmMclocSqlFileScanner.o \
		  DwmMclocTerminalTricks.o \
		  DwmMclocTexFileScanner.o
OBJS		= $(OBJFILES:%.o=../objs/%.o)
OBJDEPS		= $(OBJFILES:%.o=deps/%_deps)
INCS		= -I../include

all: ../lib/libMcloc.a

../lib/libMcloc.a: ${OBJS}
	ar rv $@ $^
	-ranlib $@

pkgprep: ${STAGING}${PREFIXDIR}/lib/libMcloc.a

${STAGING}${PREFIXDIR}/lib/libMcloc.a: ../lib/libMcloc.a
	../../install-sh -c -m 444 $< $@

DwmMclocConfigLex.cc: DwmMclocConfigLex.lex DwmMclocConfigParse.hh
	flex -o$@ $<

DwmMclocConfigParse.hh: DwmMclocConfigParse.cc

DwmMclocConfigParse.cc: DwmMclocConfigParse.y
	bison -d -o$@ $<

DwmMclocRecentsLexer.cc: DwmMclocRecentsLexer.lex DwmMclocRecentsParser.hh
	flex -o$@ $<

DwmMclocRecentsParser.hh: DwmMclocRecentsParser.cc

DwmMclocRecentsParser.cc: DwmMclocRecentsParser.y
	bison -d -o$@ $<

%FileScanner.cc: %FileScanner.lex
	flex $<

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

../objs/%.o: %.cc deps/%_deps
	${CXX} ${CXXFLAGS} ${INCS} -c $< -o $@

clean::
	rm -f ${OBJS} ${OBJDEPS} ../lib/libMcloc.a
	rm -f *FileScanner.cc DwmMclocConfigLex.cc DwmMclocConfigParse.hh DwmMclocConfigParse.cc DwmMclocRecentsParser.cc DwmMclocRecentsParser.hh DwmMclocRecentsLexer.cc

distclean:: clean pkgprep-clean
	rm -f *~

pkgprep-clean::
	rm ${STAGING}${PREFIXDIR}/lib/libMcloc.a

