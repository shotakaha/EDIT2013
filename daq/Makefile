### target
TARGET1 = adc
SRC1    = $(TARGET1).cc
OBJ1    = $(TARGET1).o
DEP1    = $(TARGET1).d

TARGET2 = multiadc
SRC2    = $(TARGET2).cc
OBJ2    = $(TARGET2).o
DEP2    = $(TARGET2).d

### ROOT environment
ROOTCFLAGS = $(shell root-config --cflags)
ROOTGLIBS  = $(shell root-config --glibs)

### CAMAC environment
CAMLIB  = /opt/hep/kinoko/drv/camdrv/camlib.o

### Linux environment
CXX      = g++
CXXFLAGS = $(ROOTCFLAGS) -O2 -Wall
CXXLIBS  = $(ROOTGLIBS) $(CAMLIB)

all: $(TARGET1) $(TARGET2) dep

$(TARGET1) : $(OBJ1)
#multiadc: multiadc.o
	@echo "##__________make '$@' from '$<'__________***"
	$(CXX) -o $@ $< $(CXXLIBS)

$(TARGET2) : $(OBJ2)
#adc: adc.o
	@echo "##__________make '$@' from '$<'__________"
	$(CXX) -o $@ $< $(CXXLIBS)

dep:
	@echo "##__________log dependencies to '$(DEP1)'__________"
	$(CXX) -MM $(CXXFLAGS) $(SRC1) > $(DEP1)
	@echo "##__________log dependencies to '$(DEP2)'__________"
	$(CXX) -MM $(CXXFLAGS) $(SRC2) > $(DEP2)

### Suffix rule : 
.cc.o:
	@echo "##__________make '$@' from '$<'__________"
	$(CXX) $(CXXFLAGS) -c $<

.PHONY : clean
clean:
	@echo "##__________cleaning__________"
	$(RM) $(TARGET1) $(OBJ1) *~
	$(RM) $(TARGET2) $(OBJ2) *~
