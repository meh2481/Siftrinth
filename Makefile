APP = siftrinth

include $(SDK_DIR)/Makefile.defs

OBJS = $(ASSETS).gen.o main.o Circle.o CubeBoard.o TextDraw.o
ASSETDEPS += *.png $(ASSETS).lua

include $(SDK_DIR)/Makefile.rules
