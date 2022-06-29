NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -MMD -MP
SRCS = srcs/**/webserv.cpp
OBJS = $(SRCS:%.cpp=%.o)
DEPS = $(OBJS:%.o=%.d)
INCS = -Isrcs/**/**.hpp

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ -c $<

all: $(NAME)

fclean: clean
	$(RM) $(NAME)

clean:
	$(RM) $(OBJS) $(DEPS)

re: fclean all

.PHONY: all fclean clean re

-include $(DEPS)

.PHONY: lint
lint:
	cpplint --filter=-legal/copyright srcs/**/*.hpp srcs/**/*.cpp

.PHONY: setup
setup:
	cp ./.githooks/cpplint.sh ./.git/hooks/pre-commit
	chmod +x ./.git/hooks/pre-commit
