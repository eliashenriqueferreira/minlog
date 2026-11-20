CROSS   := no
CC      := gcc
CXX     := g++
DESTDIR := $(abspath bin)
FLAVOR	:= $(shell cat flavor.txt)
