TARGET	= aidl

OBJS = \
	aidl.o \
	aidl_language.o \
	options.o \
	search_path.o \
	AST.o \
	Type.o \
	generate_java.o \
	generate_java_binder.o \
	generate_java_rpc.o

CFLAGS = \
	-O2 \
	-Wall \
	-DFAKE_LOG_DEVICE=1 \
	-DHAVE_PTHREADS \
	-DHAVE_OFF64_T \
	-DHAVE_ENDIAN_H \
	-DHAVE_SYS_UIO_H \
	-DOS_PATH_SEPARATOR=\'/\'

ifeq (arm,$(CPU))
#CFLAGS += -march=armv6
CFLAGS += -march=armv5te
endif

CXXFLAGS = $(CFLAGS)

ALL: $(TARGET)

aidl_language_l.cpp: aidl_language_l.l
	lex -o $@ $<

aidl_language_y.h: aidl_language_y.y
	yacc -o $@ $<

$(TARGET): aidl_language_l.cpp aidl_language_y.h $(OBJS)
	$(CXX) -o $@ $^

#aidl_language_l.o: aidl_language_l.cpp
#aidl_language_y.o: aidl_language_y.cpp

%.cxx: %.l
	lex -o $@ $<

%.cxx: %.y
	yacc -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET) aidl_language_l.cpp aidl_language_y.h *.o
