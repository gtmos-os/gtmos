all:
	cd gnu-efi && make && make bootloader
	cd kernel && make setup && make kernel && make buildimg && make run
run:
	cd kernel && make run

clean:
	cd kernel && make clean
	cd gnu-efi && make clean