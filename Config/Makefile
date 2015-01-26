NAME = prog
SRCS = $(wildcard *.cpp) $(wildcard */*.cpp)
DEPS = $(wildcard *.hpp) $(wildcard */*.hpp) $(wildcard *.h) $(wildcard */*.h)
OBJS = ${SRCS:.cpp=.o}
CXXFLAGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wunused -Wdelete-non-virtual-dtor -Wno-long-long -Wno-unused-parameter -pipe -fno-exceptions
LDFLAGS = -Wl,-O1 -Wl,--no-undefined
INCFLAGS = 
LNKFLAGS = 

.PHONY: all clean debug release

all: debug

debug: CXXFLAGS += -g
debug: $(NAME)

release: CXXFLAGS += -O2
release: $(NAME)

%.o: %.cpp $(DEPS)
	$(CXX) $(INCFLAGS) $(CXXFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(LNKFLAGS) $(LDFLAGS)

clean:
	@ rm $(OBJS) 2>/dev/null || true
	@ rm $(NAME) 2>/dev/null || true
