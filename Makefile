GCC = arm-linux-gcc 
SUBDIRS = main camera lcd  char_font pthread uart rgb2bmp obj
#key rocker uart

BIN = debug
OBJS = camera_rgb.o framebuffer.o  show_font.o pthread_fun.o rgb2bmp.o serial.o main.o
OBJS_DIR = obj
BIN_DIR = bin
CFLAG = -I ../include 
RM = rm -rf

export GCC OBJS OBJS_DIR BIN_DIR BIN CFLAG 

all:CHECK_DIR $(SUBDIRS)
CHECK_DIR:
		mkdir -p $(BIN_DIR)
$(SUBDIRS):ECHO
	make -C $@ 
ECHO:       
	@echo $(SUBDIRS)
	@echo begin compile
clear:
	@$(RM) $(OBJS_DIR)/*.o
	@$(RM) $(BIN_DIR)
	@echo $(RM) $(BIN_DIR) $(OBJS_DIR)/*.o






