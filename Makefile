CC = g++ -g
 
all: clean lab4

lab4: 
	$(CC) main.cpp -o lab4 

clean:
	@rm -f *.o lab4



	
	


