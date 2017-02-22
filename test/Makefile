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


INCLUDE_PATH = -I./ -I/usr/include -I/usr/local/include -I../output/include
				
LIB_PATH= -L../output/lib -llog -lpthread

EXENAME=test
OUTPUT_OBJ=./outputobj


##################################################################################
OBJ := $(patsubst %.cpp,$(OUTPUT_OBJ)/%.o,$(wildcard *.cpp))
OBJ += $(patsubst %.c,$(OUTPUT_OBJ)/%.o,$(wildcard *.c))

##################################################################################
.PHONY: all clean 

#OBJNAME:
all: mk_obj_dir $(EXENAME)
	@echo "**************************************************************"
	@echo "$(LIB)  pass the build, you can use it!GO, GO, GO!"
	@echo "**************************************************************"
	
mk_obj_dir:
	mkdir $(OUTPUT_OBJ) 2>/dev/null || echo	
	@echo "OBJ: $(OBJ)"	

clean:
	rm -rf *.o $(EXENAME) $(OBJ) $(OUTPUT_OBJ) 

$(OUTPUT_OBJ)/%.o	: %.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDE_PATH) 	

$(OUTPUT_OBJ)/%.o	: %.c
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDE_PATH) 	

$(EXENAME) : $(OBJ)
	$(GCC) -o $(EXENAME) $(OBJ)  -Xlinker "-(" $(LIB_PATH) $(LDFLAGS) -Xlinker "-)" 	
	 
