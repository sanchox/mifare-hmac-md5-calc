CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g
LDFLAGS=-g
LDLIBS=

SRCS=mifare-hmac-md5-calc.cpp hash-library/md5.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: mifare-hmac-md5-calc

mifare-hmac-md5-calc: $(OBJS)
	$(CXX) $(LDFLAGS) -o mifare-hmac-md5-calc $(OBJS) $(LDLIBS)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) mifare-hmac-md5-calc
