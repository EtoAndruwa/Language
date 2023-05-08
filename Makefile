EXE_FLAG = valgrind -s --leak-check=yes --show-leak-kinds=all --log-file="./logs/valgrind_log.txt"

CC = g++

DIR_GRAPHVIZ = ./graphviz
DIR_LANG = ./language
DIR_CPU = ./CPU


all:  Make_l

clean_files: clean_df clean_gf

clean_gf:
	cd $(DIR_GRAPHVIZ) && make clean_files

clean_g:
	cd $(DIR_GRAPHVIZ) && make clean

clean_l:
	cd $(DIR_LANG) && make clean

Make_g: 
	cd $(DIR_GRAPHVIZ) && make

Make_l: 
	cd $(DIR_LANG) && make