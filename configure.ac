AC_INIT([mcloc], [1.0.0], [], [], [http://www.mcplex.net])

dnl  ###  Read some macros.
builtin(include, ./mcloc.m4)  dnl

AC_PROG_CC
AC_PROG_CXX
AC_CANONICAL_SYSTEM

QMAKE=qmake

case $host_os in
     darwin[*])
	OSNAME=Darwin
	;;
     linux*)
	DEBARCH=`dpkg --print-architecture`
	AC_SUBST(DEBARCH)
	;;
     *)
        echo "host_os: " ${host_os}
	QMAKE=`which qmake`
	if [[ $? -ne 0 ]]; then
	  QMAKE=`which qmake-qt5`
	  if [[ $? -ne 0 ]]; then
	    echo qmake not found
	    exit 1
	  fi
	fi
     ;;
esac
AC_SUBST(OSNAME)
AC_SUBST(QMAKE)

MCLOC_NEED_LIBSTDCPPFS

DWM_SET_PKGVARS(mcloc,[mcloc-0.0.0])
DWM_GET_TAG([mcloc])

AC_CONFIG_FILES([Makefile.vars classes/include/DwmMclocPackageInfo.hh
                 packaging/fbsd_manifest packaging/debcontrol
		 apps/qmcloc/qmcloc.pro])
AC_OUTPUT
