include ../Makefile.tests_common

# define the driver to be used for selected boards
ifneq (,$(filter cc2650stk,$(BOARD)))
  DRIVER := bmp280
endif

DRIVER ?= bme280

USEMODULE += $(DRIVER)
USEMODULE += xtimer

include $(RIOTBASE)/Makefile.include
