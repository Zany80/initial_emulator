all:
%:
	@find builds/ -maxdepth 1 -type d -name build-* -exec make -C {} $@ \;
