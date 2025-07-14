CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -D COUNT_COMPARE=0
NAME = PmergeMe
INC = PmergeMe.hpp
SRC = main.cpp PmergeMe.cpp
OBJ = $(SRC:%.cpp=obj/%.o)

all: $(NAME)

$(NAME): $(OBJ) $(INC)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

obj/%.o: %.cpp $(INC)
	mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf obj

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re