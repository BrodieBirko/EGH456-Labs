# invoke SourceDir generated makefile for hello.pem4f
hello.pem4f: .libraries,hello.pem4f
.libraries,hello.pem4f: package/cfg/hello_pem4f.xdl
	$(MAKE) -f D:\ccs_RTOS\hello_EK_TM4C1294XL_TI/src/makefile.libs

clean::
	$(MAKE) -f D:\ccs_RTOS\hello_EK_TM4C1294XL_TI/src/makefile.libs clean

