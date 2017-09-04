all:
%:
	@find -maxdepth 1 -type d -name "build*" -exec make --no-print-directory -C {} $@ \;
