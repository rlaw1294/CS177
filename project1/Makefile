C       = g++
LIB     =  -I/usr/csshare/pkgs/csim_cpp-19.0/lib
CSIM    = /usr/csshare/pkgs/csim_cpp-19.0/lib/csim.cpp.a
RM      = /bin/rm -f
CFLAG   = -DCPP -DGPP -g
 
INPUT = RachelLawProject1.cpp
TARGET = csim
  
$(TARGET): $(INPUT)
	$(C) $(CFLAG) $(LIB) $(INPUT) $(CSIM) -lm -o $(TARGET)
 
clean:
	$(RM) *.o *.output core *~ *# $(TARGET)

