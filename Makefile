CXXFLAGS=--std=c++11


bvh.o: bvh_parser.cpp bvh_parser.h
	$(CXX) $(CXXFLAGS) -g -c bvh_parser.cpp

json.o:
	@[ -f ./json.o ]

base: json.o bvh_parser.o
	$(CXX) $(CXXFLAGS) -g -o base json.o bvh_parser.o

bonus: bvh_parser.o json.cpp bvh_parser.h
	@[ -f ./json.o ] && rm json.o || true
	$(CXX) $(CXXFLAGS) -g -c json.cpp
	$(CXX) $(CXXFLAGS) -g -o bonus json.o bvh_parser.o

.PHONY: clean
clean:
	@[ -f ./bvh.o ] && rm bvh_parser.o || true
	@[ -f bonus ] && rm bonus || true
	@[ -f base ] && rm base || true