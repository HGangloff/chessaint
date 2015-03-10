export CC=gcc
export DEBUGCFLAGS=-g
export CFLAGS=-Wall -Wextra -ansi -std=c99
export LDFLAGS=-lcunit

export TRUNK=$(shell pwd)
export PROJECTDIR=$(TRUNK)/chessaint
export TESTSDIR=$(TRUNK)/tests/unit
export BINDIR=$(TRUNK)/bin
export BUILDDIR=$(TRUNK)/build
export DEPENDIR=$(BUILDDIR)/dependencies
export DOCDIR=$(TRUNK)/docs

export EXECUTABLENAME="chessaint"
export TESTSNAME="runtests"

export MAKE=make -se

export BUILDTYPE=debug

default : debug tests
	echo "All built"
	echo ""
	#echo -e "\033[0;31mThanks for using ChessAint's makefiles !\033[0m"
.PHONY : default

all : release debug tests doc
.PHONY : all

.SILENT :

release :
	$(MAKE) -C $(PROJECTDIR) all BUILDTYPE=release
.PHONY : release

debug :
	$(MAKE) -C $(PROJECTDIR) all BUILDTYPE=debug
.PHONY : debug

tests :
	$(MAKE) -C $(TESTSDIR) BUILDTYPE=tests
.PHONY : tests

doc :
	doxygen $(DOCDIR)/doxyfile
.PHONY : doc

cleandoc :
	rm -rf $(DOCDIR)/html
	echo "Deleting doc"
.PHONY : cleandoc

nodoc:
.PHONY : nodoc

cleanall : clean cleanbin cleandoc
.PHONY : cleanall

clean :
	$(MAKE) -C $(PROJECTDIR) clean BUILDTYPE=release
	$(MAKE) -C $(PROJECTDIR) clean BUILDTYPE=debug
	$(MAKE) -C $(TESTSDIR) clean BUILDTYPE=tests
.PHONY : clean

cleanbin :
	$(MAKE) -C $(PROJECTDIR) cleanbin BUILDTYPE=release
	$(MAKE) -C $(PROJECTDIR) cleanbin BUILDTYPE=debug
	$(MAKE) -C $(TESTSDIR) cleanbin BUILDTYPE=tests
.PHONY : cleanbin

noclean :
.PHONY : noclean
