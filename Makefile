CUR_ROOT = ./
GCC = g++
C          = g++
SHELL       = /bin/bash
WORKROOT=$(CUR_ROOT)

ifeq ($(__PERF), 1)
   CPPFLAGS = -g -pg -static -pipe -fPIC -W -Wwrite-strings -Wpointer-arith -Wreorder -Wswitch -Wsign-promo \
		-Wredundant-decls -Wformat -Wall -Wconversion -D_GNU_SOURCE  -D_USE_DOUBLE_POINT_
   LDFLAGS     =  -pg 
else
   CPPFLAGS = -g  -O2 -pipe -static -fPIC -W -Wwrite-strings -Wpointer-arith -Wreorder -Wswitch -Wsign-promo \
		-Wredundant-decls -Wformat -Wall -Wconversion -D_GNU_SOURCE  -D_USE_DOUBLE_POINT_  
   LDFLAGS     = 
endif


INCLUDE_PATH = -I./ -I/usr/include -I/usr/local/include
				
LIB_PATH= -lpthread

LIB = liblog.a
OUTPUT_OBJ=./outputobj
OUTPUT=./output


##################################################################################
OBJ := $(patsubst %.cpp,$(OUTPUT_OBJ)/%.o,$(wildcard *.cpp))
OBJ += $(patsubst %.c,$(OUTPUT_OBJ)/%.o,$(wildcard *.c))
OBJ := $(filter-out $(OUTPUT_OBJ)/$(EXENAME).o, $(OBJ))

##################################################################################
.PHONY: all clean 

#OBJNAME:
all: mk_output_dir $(LIB)
	@echo "**************************************************************"
	@echo "$(LIB)  pass the build, you can use it!GO, GO, GO!"
	@echo "**************************************************************"
	
mk_obj_dir:
	mkdir $(OUTPUT_OBJ) 2>/dev/null || echo	
	@echo "OBJ: $(OBJ)"	

mk_output_dir: mk_obj_dir
	mkdir -p $(OUTPUT)/lib 2>/dev/null || echo
	mkdir -p $(OUTPUT)/include 2>/dev/null || echo
	@echo "OBJ: $(OBJ)"	
	
clean:
	rm -rf *.o $(EXENAME) $(LIB) $(OBJ) $(OUTPUT_OBJ)	$(OUTPUT)

$(OUTPUT_OBJ)/%.o	: %.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDE_PATH) 	

$(OUTPUT_OBJ)/%.o	: %.c
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDE_PATH) 	

$(LIB) : $(OBJ)
	rm -f $@ || echo
	ar cr $@ $<
	ranlib $@	
	cp $@ $(OUTPUT)/lib
	cp *.h $(OUTPUT)/include

