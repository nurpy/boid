
objects = main.o Entity.o Bird.o
libs = -Iinclude -Llib -lraylib

%.o: %.cpp
	g++ -c $< -o $@


BUILD: $(objects)
	g++ $(objects) $(libs) -o out.out
	./out.out
	$(MAKE) clean







clean: 
	rm *.out *.o
