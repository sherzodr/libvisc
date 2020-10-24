
include_HEADERS=libvisc.h
lib_LTLIBRARIES=libvisc.la
libvisc_la_SOURCES=libvisc.cpp libvisc.h

ACLOCAL_AMFLAGS=-I m4

example: example.cpp
	$(CXX) -oexample -L".libs/" -I. example.cpp -lvisc	

example.cpp: all
