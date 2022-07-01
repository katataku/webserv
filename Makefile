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

.PHONY: test
test:
	make -C tests
	
.PHONY: lint
lint:
	cpplint --filter=-legal/copyright srcs/**/*.hpp srcs/**/*.cpp

.PHONY: tidy
tidy:
	clang-tidy srcs/Webserv/sample.hpp -fix -header-filter=.* -checks=-clang-analyzer-security.insecureAPI.bzero,readability-convert-member-functions-to-static,readability-const-return-type,readability-identifier-naming,readability-make-member-function-const,readability-non-const-parameter,readability-qualified-auto,readability-redundant-control-flow,readability-redundant-member-init,readability-else-after-return,readability-implicit-bool-conversion,readability-simplify-boolean-expr,readability-static-accessed-through-instance,cppcoreguidelines-init-variables,cppcoreguidelines-virtual-class-destructor,cppcoreguidelines-prefer-member-initializer,llvm-include-order,misc-definitions-in-headers,misc-unused-parameters,modernize-redundant-void-arg,modernize-use-bool-literals,bugprone-copy-constructor-init,bugprone-virtual-near-miss,portability-restrict-system-includes,performance-for-range-copy,performance-inefficient-vector-operation,performance-type-promotion-in-math-fn,


.PHONY: setup
setup:
	cp ./.githooks/pre-commit ./.git/hooks/pre-commit
	chmod +x ./.git/hooks/pre-commit


CONTAINER = webserv
DOCKER_COMPOSE_FILE = ./docker/$(CONTAINER)/docker-compose.yml

.PHONY: build
build:
	docker compose -f $(DOCKER_COMPOSE_FILE) build 

.PHONY: up
up:
	docker compose -f $(DOCKER_COMPOSE_FILE) up -d

.PHONY: login
login:
	docker exec -it $(CONTAINER) /bin/bash

.PHONY: down
down:
	docker compose -f $(DOCKER_COMPOSE_FILE) down --timeout 1

.PHONY: docker_re
docker_re: down build up
	
.PHONY:prune
prune:
	docker system  prune
