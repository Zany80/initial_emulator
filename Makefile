all:
%:
	@find builds/ -name 32 -type d -exec $(MAKE) -C {} $@ \;
	@find builds/ -name 64 -type d -exec $(MAKE) -C {} $@ \;
