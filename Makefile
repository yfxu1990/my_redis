TARGET = xyf_redis
PROJ_DIR = /home/redis/my_redis 
SRC_DIR = /home/redis/my_redis
CFALGE = -c -g
CXXFLAGE = 
LDFLAGE = -lpthread
INCDIR = 
LINKDIR = 
LINKLIBS = 
RM = rm -f
SOURCEFILES = $(wildcard $(SRC_DIR)/*.c)

$(warning $(SRC_DIR))
$(warning $(SOURCEFILES))
OBJS = $(patsubst %.c,%.o,$(SOURCEFILES))
$(warning $(OBJS))

# suffix rules
.SUFFIXS:.c
.c.o:
	$(CC) $(CFALGE) $<
	
$(TARGET):$(OBJS)
	$(CC) $(LDFLAGE)  -o $(TARGET) $(OBJS)
	

.PHONPY:
clean:
	$(RM) $(OBJS) $(TARGET)
	

