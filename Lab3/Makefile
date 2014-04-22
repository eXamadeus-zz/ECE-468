CC          = gcc
CFLAGS      = -Wall -ggdb -lm
TESTFILE    = $(FILEDIR)/sample_iTunes.mov
GOLFFILE    = $(FILEDIR)/golfcore.ppm
ECHOFILE    = $(FILEDIR)/echo
PASSFILE    = $(FILEDIR)/passwd
FILEDIR     = ../files
HDIR        = ../headers
HEADERS     = $(HDIR)/ex_bintree.c $(HDIR)/ex_huffenc.c $(HDIR)/ex_huffdec.c
CLEANUP     = $(wildcard *.huf) $(wildcard *.ppm) $(wildcard *dec*) $(wildcard *.bin)
CLEANUPFILE = $(wildcard $(FILEDIR)/*.huf) $(wildcard $(FILEDIR)/*dec*)
RUN         = ./lab3.bin
SCRIPTDIR   = ./scripts
DIFF        = $(SCRIPTDIR)/difflab
TIMERUN     = $(SCRIPTDIR)/timelab $(RUN)

display:
	@echo
	@echo "Usage:"
	@echo "	make huff"
	@echo "		Runs through all file type Huffman tests"
	@echo
	@echo "	make golf"
	@echo "		Runs through Huffman with golfcore.ppm image"
	@echo
	@echo "	make pass"
	@echo "		Runs through Huffman with passwd text"
	@echo
	@echo "	make echo"
	@echo "		Runs through Huffman with echo binary"
	@echo
	@echo "	make [golf][pass][echo][huff]t"
	@echo "		Runs through [<command>] with a timer script"
	@echo
	@echo "	make clean"
	@echo "		Cleans up the directory"
	@echo

clean:
	@printf "\033[1;34mCleaning \033[0m\033[34mup...\033[0m"
	@rm -rf *.o $(CLEANUP) $(CLEANUPFILE)
	@printf "\033[1;34mdone.\033[0m\n"

huff: clean golf pass echo

hufft: clean golft passt echot

logmark:
	@echo "+++++++++++++++++++++++++++++++++++++++++++++++++" 1>error.log
	@printf "\033[1;32mCompiling \033[0m\033[32mlab3.c...\033[0m"

build: lab3.c logmark
	@$(CC) lab3.c $(HEADERS) $(CFLAGS) -o lab3.bin 2> error.log || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;32mdone.\033[0m\n"

test: clean build
	@printf "\033[1;36mRunning \033[0m\033[36mcompression Huffman ($(TESTFILE))...\033[0m"
	-@$(RUN) $(TESTFILE) || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"
	@printf "\033[1;36mRunning \033[0m\033[36mdecompression Huffman ($(TESTFILE))...\033[0m"
	-@$(RUN) $(TESTFILE).huf -d || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"

gruninfo:
	@printf "\033[1;36mRunning \033[0m\033[36mcompression Huffman ($(GOLFFILE))...\033[0m"
gruninfod:
	@printf "\033[1;36mRunning \033[0m\033[36mdecompression Huffman ($(GOLFFILE).huf)...\033[0m"

pruninfo:
	@printf "\033[1;36mRunning \033[0m\033[36mcompression Huffman ($(PASSFILE))...\033[0m"
pruninfod:
	@printf "\033[1;36mRunning \033[0m\033[36mdecompression Huffman ($(PASSFILE).huf)...\033[0m"

eruninfo:
	@printf "\033[1;36mRunning \033[0m\033[36mcompression Huffman ($(ECHOFILE))...\033[0m"
eruninfod:
	@printf "\033[1;36mRunning \033[0m\033[36mdecompression Huffman ($(ECHOFILE).huf)...\033[0m"

ginfo:
	@printf "\033[1;33mOriginal \033[0m\033[33mfile size: "
	@ls -lrt $(GOLFFILE) | gawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed \033[0m\033[33mfile size: "
	@ls -lrt $(GOLFFILE).huf | gawk '{print $$5}'
	@printf "\033[0m"
ginfod: 
	@printf "\033[1;33mCompressed \033[0m\033[33mfile size: "
	@ls -lrt $(GOLFFILE).huf | gawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mDecompressed \033[0m\033[33mfile size: "
	@ls -lrt $(GOLFFILE).hufdec | gawk '{print $$5}'
	@printf "\033[0m"

golf: build grun ginfo grund ginfod
	@printf "\033[1;35mDiff\033[0m "
	@printf "\033[35m$(GOLFFILE) $(GOLFFILE)dec"
	-@$(DIFF) $(GOLFFILE) $(GOLFFILE).hufdec || (printf "\033[1;31m failed.\033[0m\n" && false) && printf "\033[1;35m done.\033[0m\n" 
golft: build grunt ginfo grundt ginfod
	@printf "\033[1;35mDiff\033[0m "
	@printf "\033[35m$(GOLFFILE) $(GOLFFILE)dec"
	-@$(DIFF) $(GOLFFILE) $(GOLFFILE).hufdec || (printf "\033[1;31m failed.\033[0m\n" && false) && printf "\033[1;35m done.\033[0m\n"

grun: build gruninfo
	-@$(RUN) $(GOLFFILE) || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"
grund: build gruninfod
	-@$(RUN) $(GOLFFILE).huf -d || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"
grunt: build gruninfo
	-@$(TIMERUN) $(GOLFFILE) || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"
grundt: build gruninfod
	-@$(TIMERUN) $(GOLFFILE).huf -d || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"

einfo:
	@printf "\033[1;33mOriginal \033[0m\033[33mfile size: "
	@ls -lrt $(ECHOFILE) | gawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed \033[0m\033[33mfile size: "
	@ls -lrt $(ECHOFILE).huf | gawk '{print $$5}'
	@printf "\033[0m"
einfod: 
	@printf "\033[1;33mCompressed \033[0m\033[33mfile size: "
	@ls -lrt $(ECHOFILE).huf | gawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mDecompressed \033[0m\033[33mfile size: "
	@ls -lrt $(ECHOFILE).hufdec | gawk '{print $$5}'
	@printf "\033[0m"

echo: build erun einfo erund einfod
	@printf "\033[1;35mDiff\033[0m "
	@printf "\033[35m$(ECHOFILE) $(ECHOFILE)dec"
	-@$(DIFF) $(ECHOFILE) $(ECHOFILE).hufdec || (printf "\033[1;31m failed.\033[0m\n" && false) && printf "\033[1;35m done.\033[0m\n" 
echot: build erunt einfo erundt einfod
	@printf "\033[1;35mDiff\033[0m "
	@printf "\033[35m$(ECHOFILE) $(ECHOFILE)dec"
	-@$(DIFF) $(ECHOFILE) $(ECHOFILE).hufdec || (printf "\033[1;31m failed.\033[0m\n" && false) && printf "\033[1;35m done.\033[0m\n"

erun: build eruninfo
	-@$(RUN) $(ECHOFILE) || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"
erund: build eruninfod
	-@$(RUN) $(ECHOFILE).huf -d || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"
erunt: build eruninfo
	-@$(TIMERUN) $(ECHOFILE) || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"
erundt: build eruninfod
	-@$(TIMERUN) $(ECHOFILE).huf -d || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"

pinfo:
	@printf "\033[1;33mOriginal \033[0m\033[33mfile size: "
	@ls -lrt $(PASSFILE) | gawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mCompressed \033[0m\033[33mfile size: "
	@ls -lrt $(PASSFILE).huf | gawk '{print $$5}'
	@printf "\033[0m"
pinfod: 
	@printf "\033[1;33mCompressed \033[0m\033[33mfile size: "
	@ls -lrt $(PASSFILE).huf | gawk '{print $$5}'
	@printf "\033[0m"
	@printf "\033[1;33mDecompressed \033[0m\033[33mfile size: "
	@ls -lrt $(PASSFILE).hufdec | gawk '{print $$5}'
	@printf "\033[0m"

pass: prun pinfo prund pinfod
	@printf "\033[1;35mDiff\033[0m "
	@printf "\033[35m$(PASSFILE) $(PASSFILE)dec"
	-@$(DIFF) $(PASSFILE) $(PASSFILE).hufdec || (printf "\033[1;31m failed.\033[0m\n" && false) && printf "\033[1;35m done.\033[0m\n" 
passt: prunt pinfo prundt pinfod
	@printf "\033[1;35mDiff\033[0m "
	@printf "\033[35m$(PASSFILE) $(PASSFILE)dec"
	-@$(DIFF) $(PASSFILE) $(PASSFILE).hufdec || (printf "\033[1;31m failed.\033[0m\n" && false) && printf "\033[1;35m done.\033[0m\n"

prun: build pruninfo
	-@$(RUN) $(PASSFILE) || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"
prund: build pruninfod
	-@$(RUN) $(PASSFILE).huf -d || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"
prunt: build pruninfo
	-@$(TIMERUN) $(PASSFILE) || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"
prundt: build pruninfod
	-@$(TIMERUN) $(PASSFILE).huf -d || (printf "\033[1;31mfailed.\033[0m\n" && false) && printf "\033[1;36mdone.\033[0m\n"
