NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -MMD -MP -g -O0
SRCS = $(wildcard srcs/*/*.cpp)
OBJS = $(SRCS:%.cpp=%.o)
DEPS = $(OBJS:%.o=%.d)
HEADERS = $(wildcard srcs/*/*.hpp)
INCS = $(addprefix -I,$(wildcard srcs/*)) 

ifdef DEBUG
	CXXFLAGS += -D DEBUG=true -g -fsanitize=address
endif

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

debug: fclean ## Build in debug mode
	make DEBUG=true

.PHONY: all fclean clean re

-include $(DEPS)

# -------------------------- Rules For Test -------------------------------

.PHONY: utest
utest: ## Exec unit tests for webserver
	make -C tests/unit_test

.PHONY: itest
itest: ## Exec unit tests for webserver
	make itest-ok && make itest-error

INTEGRATION_TEST_SHELL_OK = ./tests/integration_test/ok.sh
.PHONY: itest-ok
itest-ok: ## Exec unit tests for webserver
	@if [ ! -x $(INTEGRATION_TEST_SHELL_OK) ]; then\
		chmod +x $(INTEGRATION_TEST_SHELL_OK);\
	fi
	$(INTEGRATION_TEST_SHELL_OK)


INTEGRATION_TEST_SHELL_ERROR = ./tests/integration_test/error.sh
.PHONY: itest-error
itest-error: ## Exec unit tests for webserver
	@if [ ! -x $(INTEGRATION_TEST_SHELL_ERROR) ]; then\
		chmod +x $(INTEGRATION_TEST_SHELL_ERROR);\
	fi
	$(INTEGRATION_TEST_SHELL_ERROR)

VALGRIND_TEST_SHELL = ./tests/valgrind/valgrind.sh
.PHONY: valgrind
valgrind: ## Exec valgrind tests for webserver
	@if [ ! -x $(VALGRIND_TEST_SHELL) ]; then\
		chmod +x $(VALGRIND_TEST_SHELL);\
	fi
	$(VALGRIND_TEST_SHELL)

# -------------------- Rules For Static Analyser --------------------------

.PHONY: format
format: ## Lint webserver source files
	clang-format $(HEADERS) $(SRCS) --dry-run -Werror

.PHONY: lint
lint: ## Lint webserver source files
	cpplint --filter=-legal/copyright,-build/include_subdir $(HEADERS) $(SRCS)

.PHONY: tidy
tidy: ## Tidy webserver source files
	clang-tidy $(HEADERS) $(SRCS) -- $(CXXFLAGS) $(INCS)

.PHONY: mdformat
mdformat: ## Format Markdown files
	mdformat README.md docs/

# ------------------------ Rules For Developer ----------------------------

.PHONY: setup
setup: ## Set up hooks for commit
	cp ./.githooks/pre-commit ./.git/hooks/pre-commit
	chmod +x ./.git/hooks/pre-commit

.PHONY: submit
submit: ## Move files for submission
	sh tools/submit.sh

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
dc-re: dc-down dc-build dc-up ## Rebuild docker image and run container
	

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

# ----------------------- Rules For Nginx Container -----------------------

.PHONY: dc-siege-build
dc-siege-build: ## Build siege container
	make dc-build CONTAINER=siege

.PHONY: dc-siege-up
dc-siege-up: ## Run siege container
	make dc-up CONTAINER=siege

.PHONY: dc-siege-login
dc-siege-login: ## Login siege container
	make dc-login CONTAINER=siege

.PHONY: dc-siege-down
dc-siege-down: ## Down siege container
	make dc-down CONTAINER=siege

.PHONY: dc-siege-re 
dc-siege-re: dc-siege-down dc-siege-build dc-siege-up ## Rebuild siege image and run container


.PHONY: stress
stress: ## Do Stress Test on siege container
	@echo "start stress test"
	@echo "rebuild docker container"
	@make dc-siege-re
	@echo "start stress test for 30 sec."
	@docker compose -f ./docker/siege/docker-compose.yml exec -T siege bash start.sh
	@echo ""
	@echo "Finish Stress test. You can see ./log/siege.log and ./log/top.log"

# ---------------------------- Rules For Help -----------------------------

.PHONY: help
help: ## Display this help screen
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' Makefile | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: h
h: help ## shorthand 'help' command
