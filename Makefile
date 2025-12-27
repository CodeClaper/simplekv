SUBDIRS := src 

all: 
	$(foreach dir, $(SUBDIRS), $(MAKE) -C $(dir);)

stat:
	cloc src include 

clean: 
	@$(foreach dir, $(SUBDIRS), $(MAKE) -C $(dir) clean;)

.PHONY: all stat check
