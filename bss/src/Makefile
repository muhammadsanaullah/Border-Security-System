include ../Makefile.inc

LIB_GEOJSON_DIR := ../libs/lib_geojson
GASON_SRC := ../libs/gason/src
GASON_DIR := ../libs/gason
INCLUDES := -I$(LIB_GEOJSON_DIR) -I$(GASON_SRC)
BSS := bss
GASON := ../build/gason.o
LIB_GEOJSON := ../build/lib_geojson.o

all: $(BSS)

$(LIB_GEOJSON):
	make -C $(LIB_GEOJSON_DIR)

$(GASON):
	make -C $(GASON_DIR)

$(BSS): $(LIB_GEOJSON) $(GASON)
	$(COMPILER) $(INCLUDES) $^ bss.cpp -o bss

clean:
	rm ../build/*
	rm bss
