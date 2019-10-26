PHP ?= php7.4
all: composer.lock
	$(MAKE) run
run:
	@echo -n executing '"'$(PHP) console random'"' :' '
	@$(PHP) console random

composer.lock: composer.json
	composer dump-a
