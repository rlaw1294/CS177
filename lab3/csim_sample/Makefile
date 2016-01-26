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

TARGET = csim
  
$(INPUT): $(INPUT)
	$(C) $(CFLAG) $(LIB) $(TARGET) $(CSIM) -lm -o $(TARGET)

$(INPUT1): $(INPUT1)
	$(C) $(CFLAG) $(LIB) $(INPUT1) $(CSIM) -lm -o $(TARGET)

$(INPUT2): $(INPUT2)
	$(C) $(CFLAG) $(LIB) $(INPUT2) $(CSIM) -lm -o $(TARGET)

$(INPUT3): $(INPUT3)
	$(C) $(CFLAG) $(LIB) $(INPUT3) $(CSIM) -lm -o $(TARGET)

$(INPUT4): $(INPUT4)
	$(C) $(CFLAG) $(LIB) $(INPUT4) $(CSIM) -lm -o $(TARGET)
 
$(INPUT5): $(INPUT5)
	$(C) $(CFLAG) $(LIB) $(INPUT5) $(CSIM) -lm -o $(TARGET)
 
clean:
	$(RM) *.o *.output core *~ *# $(TARGET)

