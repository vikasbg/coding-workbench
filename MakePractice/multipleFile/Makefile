CC=gcc
INC=-I.
TAR_NAME=main
SRC_DIR=.
CUR_DIR=.
INSTALL_PATH=.
INSTALL_PATH=./build
# INSTALL_PATH=$(shell pwd)/build

CFLAGS=-c -g -O2
LDFLAGS= 

main: main.o add.o pr.o
	$(CC) $(SRC_DIR)/main.o $(SRC_DIR)/add.o $(SRC_DIR)/pr.o -o $(TAR_NAME)
	@echo "Application Generated"

main.o: main.c
	$(CC) $(INC) $(CFLAGS) $(SRC_DIR)/main.c
	@echo "Relocatable main.o generated"

add.o: add.c
	$(CC) $(INC) $(CFLAGS) $(SRC_DIR)/add.c
	@echo "Relocatable add.o generated"

pr.o: pr.c
	$(CC) $(INC) $(CFLAGS) $(SRC_DIR)/pr.c
	@echo "Relocatable pr.o generated"

install: $(TAR_NAME)
# 1. Create directory if missing (-p handles the check)
# 	mkdir -p $(INSTALL_PATH) 
	cp -f $(CUR_DIR)/$(TAR_NAME) $(INSTALL_PATH)/$(TAR_NAME)
	@echo "Application Installed/copied Successfully at $(INSTALL_PATH)"

clean:
	rm -rf $(CUR_DIR)/*.o $(CUR_DIR)/$(TAR_NAME) $(INSTALL_PATH)/$(TAR_NAME)
	@echo "Application Removed/deleted Successfully"

