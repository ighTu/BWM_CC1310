# invoke SourceDir generated makefile for uartecho.pem3
uartecho.pem3: .libraries,uartecho.pem3
.libraries,uartecho.pem3: package/cfg/uartecho_pem3.xdl
	$(MAKE) -f C:\Workspace\Github\BWM_CC1310\bwm_CC1310_LAUNCHXL_TI_1.1/src/makefile.libs

clean::
	$(MAKE) -f C:\Workspace\Github\BWM_CC1310\bwm_CC1310_LAUNCHXL_TI_1.1/src/makefile.libs clean

