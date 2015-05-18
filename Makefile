CXX = g++
CXXFLAGS = 
CPPFLAGS = 
LDFLAGS = -s

compile: SE-TreeHUG 

SE-TreeHUG: SE-TreeHUG.o Items.o Dataset.o Itemset.o ItemsetTable.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o SE-TreeHUG 

