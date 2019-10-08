CROSS   := no
CC      := gcc-7
CXX     := g++-7
DESTDIR := $(abspath ../bin)
FLAVOR	:= $(shell cat flavor.txt)
