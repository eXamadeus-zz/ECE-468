CC=gcc
CFLAGS=-Wall -ggdb
IMGDIR=../images
HDIR=../headers
FDIR=../files
HEADERS=$(HDIR)/ex_rle.c $(HDIR)/ex_lzw.c $(HDIR)/ex_dict.c
CLEANUP=$(wildcard *.rle) $(wildcard *.lzw) $(wildcard *.ppm) $(wildcard *dec*)
CLEANUPIMG=$(wildcard $(IMGDIR)/*.rle) $(wildcard $(IMGDIR)/*.lzw) $(wildcard $(IMGDIR)/*dec*)

all:
	@echo
	@echo "Usage:"
	@echo "	make diffr"
	@echo "		Runs through RLE tests"
	@echo
	@echo "	make diffl"
	@echo "		Runs through LZW tests"
	@echo
	@echo "	make [rl][te][d]"
	@echo "		Compression:"
	@echo "			r - use RLE encryption"
	@echo "			l - use LZW encryption"
	@echo "		Input (default - ../images/golfcore.ppm):"
	@echo "			t - use passwd for input"
	@echo "			e - use echo for input"
	@echo "		Decompression (default - does not decompress):"
	@echo "			d - decompress file after compressing"
	@echo
	@echo "	make clean"
	@echo "		Cleans up the directory"
	@echo

test: clean test.c
	@echo "+++++++++++++++++++++++++++++++++++++++++++++++++" > error.log
	@$(CC) $(CFLAGS) $(HEADERS) test.c -o test.bin 2> error.log

lab2: lab2.c
	@echo "+++++++++++++++++++++++++++++++++++++++++++++++++" > error.log
	@printf "\033[1;32mCompiling \033[0m\033[32mlab2.c...\033[0m"
	@$(CC) $(CFLAGS) $(HEADERS) lab2.c -o lab2.bin 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;32mdone.\033[0m"

clean:
	@printf "\033[1;34mCleaning \033[0m\033[34mup...\033[0m"
	@rm -rf *.o *.bin $(CLEANUP) $(CLEANUPIMG)
	@ls > /dev/null
	@echo "\033[1;34mdone.\033[0m"
	
r: lab2
	@printf "\033[1;36mRunning \033[0m\033[36mcompression RLE...\033[0m"
	@./lab2.bin $(IMGDIR)/golfcore.ppm 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;36mdone.\033[0m"
	@printf "\033[1;33mOriginal file \033[0m\033[33msize: "
	@ls -lrt $(IMGDIR)/golfcore.ppm | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed file \033[0m\033[33msize: "
	@ls -lrt $(IMGDIR)/*.rle | nawk '{print $$5}'
	@printf "\033[0m"

@r: lab2
	@printf "\033[1;36mRunning \033[0m\033[36mcompression/decompression RLE ($(IMGDIR)/golfcore.ppm)...\033[0m"
	@./lab2.bin $(IMGDIR)/golfcore.ppm

rd: @r
	@./lab2.bin $(IMGDIR)/golfcore.ppm.rle -d 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;36mdone.\033[0m"
	@printf "\033[1;33mOriginal file \033[0m\033[33msize: "
	@ls -lrt $(IMGDIR)/golfcore.ppm | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed file \033[0m\033[33msize: "
	@ls -lrt $(IMGDIR)/golfcore*.rle | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mDecompressed file \033[0m\033[33msize: "
	@ls -lrt $(IMGDIR)/golfcore*dec* | nawk '{print $$5}'
	@printf "\033[0m"

l: lab2
	@printf "\033[1;36mRunning \033[0m\033[36mcompression LZW...\033[0m"
	@./lab2.bin $(IMGDIR)/golfcore.ppm -l 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;36mdone.\033[0m"
	@printf "\033[1;33mOriginal file \033[0m\033[33msize: "
	@ls -lrt $(IMGDIR)/golfcore.ppm | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed file \033[0m\033[33msize: "
	@ls -lrt $(IMGDIR)/*.lzw | nawk '{print $$5}'
	@printf "\033[0m"

@l: lab2
	@printf "\033[1;36mRunning \033[0m\033[36mcompression/decompression LZW ($(IMGDIR)/golfcore.ppm)...\033[0m"
	@./lab2.bin $(IMGDIR)/golfcore.ppm -l

ld: @l
	@./lab2.bin $(IMGDIR)/golfcore.ppm.lzw -l -d 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;36mdone.\033[0m"
	@printf "\033[1;33mOriginal file \033[0m\033[33msize: "
	@ls -lrt $(IMGDIR)/golfcore.ppm | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed file \033[0m\033[33msize: "
	@ls -lrt $(IMGDIR)/golfcore*.lzw | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mDecompressed file \033[0m\033[33msize: "
	@ls -lrt $(IMGDIR)/golfcore*dec* | nawk '{print $$5}'
	@printf "\033[0m"

rt: lab2
	@printf "\033[1;36mRunning \033[0m\033[36mcompression RLE (passwd)...\033[0m"
	-@./lab2.bin $(FDIR)/passwd 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;36mdone.\033[0m"
	@printf "\033[1;33mOriginal file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/passwd | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/*.rle | nawk '{print $$5}'
	@printf "\033[0m"

@rt: lab2
	@printf "\033[1;36mRunning \033[0m\033[36m compression/decompression RLE (passwd)...\033[0m"
	@./lab2.bin $(FDIR)/passwd

rtd: @rt
	@./lab2.bin $(FDIR)/passwd.rle -d 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;36mdone.\033[0m"
	@printf "\033[1;33mOriginal file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/passwd | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/passwd*.rle | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mDecompressed file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/passwd*dec* | nawk '{print $$5}'
	@printf "\033[0m"

re: lab2
	@printf "\033[1;36mRunning \033[0m\033[36mcompression RLE (echo)...\033[0m"
	-@./lab2.bin $(FDIR)/echo 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;36mdone.\033[0m"
	@printf "\033[1;33mOriginal file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/echo | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/echo*.rle | nawk '{print $$5}'
	@printf "\033[0m"

@re: lab2
	@printf "\033[1;36mRunning \033[0m\033[36m compression/decompression RLE (echo)...\033[0m"
	@./lab2.bin $(FDIR)/echo

red: @re
	@./lab2.bin $(FDIR)/echo.rle -d 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;36mdone.\033[0m"
	@printf "\033[1;33mOriginal file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/echo | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/echo*.rle | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mDecompressed file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/echo*dec* | nawk '{print $$5}'
	@printf "\033[0m"

lt: lab2
	@printf "\033[1;36mRunning \033[0m\033[36mcompression LZW (passwd)...\033[0m"
	-@./lab2.bin $(FDIR)/passwd -l 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;36mdone.\033[0m"
	@printf "\033[1;33mOriginal file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/passwd | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/*.lzw | nawk '{print $$5}'
	@printf "\033[0m"

@lt: lab2
	@printf "\033[1;36mRunning \033[0m\033[36mcompression/decompression LZW (passwd)...\033[0m"
	@./lab2.bin $(FDIR)/passwd -l

ltd: @lt
	@./lab2.bin $(FDIR)/passwd.lzw -l -d 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;36mdone.\033[0m"
	@printf "\033[1;33mOriginal file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/passwd | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/passwd*.lzw | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mDecompressed file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/passwd*dec* | nawk '{print $$5}'
	@printf "\033[0m"

le: lab2
	@printf "\033[1;36mRunning \033[0m\033[36mcompression LZW (echo)...\033[0m"
	-@./lab2.bin $(FDIR)/echo -l 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;36mdone.\033[0m"
	@printf "\033[1;33mOriginal file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/echo | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/*.lzw | nawk '{print $$5}'
	@printf "\033[0m"

@le: lab2
	@printf "\033[1;36mRunning \033[0m\033[36m compression/decompression LZW (echo)...\033[0m"
	@./lab2.bin $(FDIR)/echo -l

led: @le
	@./lab2.bin $(FDIR)/echo.lzw -d -l 2> error.log || (echo "\033[1;31mfailed.\033[0m" && false) && echo "\033[1;36mdone.\033[0m"
	@printf "\033[1;33mOriginal file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/echo | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/echo*.lzw | nawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mDecompressed file \033[0m\033[33msize: "
	@ls -lrt $(FDIR)/echo*dec* | nawk '{print $$5}'
	@printf "\033[0m"

diffr: clean rd rtd red
	@printf "\033[35m"
	diff $(IMGDIR)/golfcore.ppm $(IMGDIR)/golfcoredec.ppm
	diff $(FDIR)/passwd $(FDIR)/passwddec
	diff $(FDIR)/echo $(FDIR)/echodec
	@printf "\033[0m"

diffl: clean ld ltd led
	@printf "\033[35m"
	diff $(IMGDIR)/golfcore.ppm $(IMGDIR)/golfcoredec.ppm
	diff $(FDIR)/passwd $(FDIR)/passwddec
	diff $(FDIR)/echo $(FDIR)/echodec
	@printf "\033[0m"
