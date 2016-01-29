C       = g++
LIB     =  -I/usr/csshare/pkgs/csim_cpp-19.0/lib
CSIM    = /usr/csshare/pkgs/csim_cpp-19.0/lib/csim.cpp.a
RM      = /bin/rm -f
CFLAG   = -DCPP -DGPP
 
INPUT = hertz2.csim.c
INPUT1 = hertz3.csim.c
INPUT2 = oneworld.c
INPUT3 = twoworld.c
INPUT4 = hiho.c
INPUT5 = hiho2.c
INPUT6 = part1.c
INPUT7 = part2.cpp

TARGET = hertz2.csim
TARGET1 = hertz3.csim
TARGET2 = oneworld.csim
TARGET3 = twoworld.csim
TARGET4 = hiho.csim
TARGET5 = hiho2.csim
TARGET6 = part1.csim
TARGET7 = part2.csim

$(TARGET): $(INPUT)
	$(C) $(CFLAG) $(LIB) $(INPUT) $(CSIM) -lm -o $(TARGET)

$(TARGET1): $(INPUT1)
	$(C) $(CFLAG) $(LIB) $(INPUT1) $(CSIM) -lm -o $(TARGET1)

$(TARGET2): $(INPUT2)
	$(C) $(CFLAG) $(LIB) $(INPUT2) $(CSIM) -lm -o $(TARGET2)

$(TARGET3): $(INPUT3)
	$(C) $(CFLAG) $(LIB) $(INPUT3) $(CSIM) -lm -o $(TARGET3)

$(TARGET4): $(INPUT4)
	$(C) $(CFLAG) $(LIB) $(INPUT4) $(CSIM) -lm -o $(TARGET4)
 
$(TARGET5): $(INPUT5)
	$(C) $(CFLAG) $(LIB) $(INPUT5) $(CSIM) -lm -o $(TARGET5)

$(TARGET6): $(INPUT6)
	$(C) $(CFLAG) $(LIB) $(INPUT6) $(CSIM) -lm -o $(TARGET6)

$(TARGET7): $(INPUT7)
	$(C) $(CFLAG) $(LIB) $(INPUT7) $(CSIM) -lm -o $(TARGET7)
 
 
clean:
	$(RM) *.o *.output core *~ *# $(TARGET) *.csim

