CC = mpicc
OBJS = prefix_scan2.o
TARGET = prefix_scan2

$(TARGET) : $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

clean :
	rm *.o prefix_scan2
