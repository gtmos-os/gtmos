all:
	cd gnu-efi && make
	cd kernel && make setup && make kernel && make buildimg && make run
run:
	cd kernel && make run