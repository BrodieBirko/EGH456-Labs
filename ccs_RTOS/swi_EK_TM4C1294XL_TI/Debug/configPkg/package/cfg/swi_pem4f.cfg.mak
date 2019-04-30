# invoke SourceDir generated makefile for swi.pem4f
swi.pem4f: .libraries,swi.pem4f
.libraries,swi.pem4f: package/cfg/swi_pem4f.xdl
	$(MAKE) -f D:\ccs_RTOS\swi_EK_TM4C1294XL_TI/src/makefile.libs

clean::
	$(MAKE) -f D:\ccs_RTOS\swi_EK_TM4C1294XL_TI/src/makefile.libs clean

