NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -MMD -MP
SRCS = srcs/**/webserv.cpp
OBJS = $(SRCS:%.cpp=%.o)
DEPS = $(OBJS:%.o=%.d)
INCS = -Isrcs/**/**.hpp

# -------------------------- Rules For Build ------------------------------

$(NAME): $(OBJS) ## Build webserver
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ -c $<

all: $(NAME) ## Build webserver

fclean: clean ## Delete executable webserver
	$(RM) $(NAME)

clean: ## Delete webserver object files
	$(RM) $(OBJS) $(DEPS)

re: fclean all ## Rebuild webserver

.PHONY: all fclean clean re

-include $(DEPS)

# -------------------------- Rules For Test -------------------------------

.PHONY: test
test: ## Exec unit tests for webserver
	make -C tests

# -------------------- Rules For Static Analyser --------------------------

.PHONY: lint
lint: ## Lint webserver source files
	cpplint --filter=-legal/copyright srcs/**/*.hpp srcs/**/*.cpp

.PHONY: tidy
tidy: ## Tidy webserver source files
	clang-tidy srcs/**/*.hpp srcs/**/*.cpp -fix

# ------------------------ Rules For Developer ----------------------------

.PHONY: setup
setup: ## Set up hooks for commit
	cp ./.githooks/pre-commit ./.git/hooks/pre-commit
	chmod +x ./.git/hooks/pre-commit

# ------------------------- Rules For Docker ------------------------------

CONTAINER = webserv
DOCKER_COMPOSE_FILE = ./docker/$(CONTAINER)/docker-compose.yml

.PHONY: dc-build
dc-build: ## Build docker container
	docker compose -f $(DOCKER_COMPOSE_FILE) build 

.PHONY: dc-up
dc-up: ## Run docker container
	docker compose -f $(DOCKER_COMPOSE_FILE) up -d

.PHONY: dc-login
dc-login: ## Login docker container
	docker exec -it $(CONTAINER) /bin/bash

.PHONY: dc-down
dc-down: ## Down docker container
	docker compose -f $(DOCKER_COMPOSE_FILE) down --timeout 1

.PHONY: dc-re
dc-re: dc-build dc-build dc-up ## Rebuild docker image and run container
	

# ----------------------- Rules For Nginx Container -----------------------

.PHONY: dc-nginx-build
dc-nginx-build: ## Build nginx container
	make dc-build CONTAINER=nginx

.PHONY: dc-nginx-up
dc-nginx-up: ## Run nginx container
	make dc-up CONTAINER=nginx

.PHONY: dc-nginx-login
dc-nginx-login: ## Login nginx container
	make dc-login CONTAINER=nginx

.PHONY: dc-nginx-down
dc-nginx-down: ## Down nginx container
	make dc-down CONTAINER=nginx

.PHONY: dc-nginx-re 
dc-nginx-re: dc-nginx-down dc-nginx-build dc-nginx-up ## Rebuild nginx image and run container

.PHONY: dc-prune
dc-prune: ## Delete unused docker object (images, containers, networks)
	docker system  prune

# ---------------------------- Rules For Help -----------------------------

.PHONY: help
help: ## Display this help screen
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' Makefile | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: h
h: help ## shorthand 'help' command
