PYTHON23=-I/System/Library/Frameworks/Python.framework/Versions/2.3/include/python2.3
PYTHON24=-I/Library/Frameworks/Python.framework/Versions/2.4/Headers
PYTHON25=-I/Library/Frameworks/Python.framework/Versions/2.5/Headers

PYTHONDEP23=/System/Library/Frameworks/Python.framework/Versions/Current/lib/python2.3/site-packages/pylink
PYTHONDEP24=/Library/Frameworks/Python.framework/Versions/2.4/lib/python2.4/site-packages/pylink
PYTHONDEP25=/Library/Frameworks/Python.framework/Versions/2.5/lib/python2.5/site-packages/pylink

PYTHON23X=python2.3
PYTHON24X=/usr/local/bin/python2.4
PYTHON25X=python2.5


ifeq ($(DEPPYVERSION),"")
        DEPPYVERSION=2.4
endif

ifeq ($(DEPPYVERSION),2.3)
	PYTHONX=$(PYTHON23X)
	PYTHONDEP=$(PYTHONDEP23)
	PYTHON=$(PYTHON23) 
endif

ifeq ($(DEPPYVERSION),2.4)
	PYTHONX=$(PYTHON24X)
	PYTHONDEP=$(PYTHONDEP24)
	PYTHON=$(PYTHON24) 
endif

ifeq ($(DEPPYVERSION),2.5)
	PYTHONX=$(PYTHON25X)
	PYTHONDEP=$(PYTHONDEP25)
	PYTHON=$(PYTHON25) 
endif

CFLAGS= -DMACOSX -DUNIX -DNDEBUG  -F$(HOME)/Library/Frameworks  $(PYTHON) -I/Library/Frameworks/eyelink_core.framework/Versions/Current/Headers/ -I/Library/Frameworks/eyelink_core_graphics.framework/Versions/Current/Headers/  
all: package

build: 
	rm -rf $(PYTHONDEP)
	-rm -rf csrc/eyelinkmodule_processed*
	echo "#define MAPFCN(x,y)  { x,y, 0x0001,x}" >csrc/eyelinkmodule_processed1.c
	echo "#define MAPFCNEX(x,y,z)  { x,y, 0x0001,z}" >>csrc/eyelinkmodule_processed1.c
	-gcc  $(CFLAGS) -E csrc/eyelinkmodule.c 2> t.txt 1>>csrc/eyelinkmodule_processed1.c
	-gcc  $(CFLAGS) -E csrc/eyelinkmodule_processed1.c 2> t.txt 1>>csrc/eyelinkmodule_processed.c
	
	$(PYTHONX) setupmac.py install
	cp -R $(PYTHONDEP) pylinktogo/pylink$(DEPPYVERSION)
	find  pylinktogo/pylink$(DEPPYVERSION) -name "*.py" -depth -exec rm -rf {} \;

clean:
	
	rm -rf pylinktogo

package: clean
	mkdir pylinktogo
	cp -R examples pylinktogo
	cp  "Pylink api userguide.pdf" pylinktogo
	cp readmemac.txt pylinktogo/readme.txt

	rm -rf build
	make build DEPPYVERSION=2.3
	rm -rf build
	make build DEPPYVERSION=2.4
	rm -rf build
	make build DEPPYVERSION=2.5		
	
	find  pylinktogo -name "CVS" -depth -exec rm -rf {} \;
	
