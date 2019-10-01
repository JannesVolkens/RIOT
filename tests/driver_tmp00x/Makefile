include ../Makefile.tests_common

# define the driver to be used for selected boards
ifneq (,$(filter cc2650stk,$(BOARD)))
  DRIVER := tmp007
endif

DRIVER ?= tmp006

USEMODULE += $(DRIVER)
USEMODULE += xtimer

include $(RIOTBASE)/Makefile.include
