EXE_FLAG = valgrind -s --leak-check=yes --show-leak-kinds=all --log-file="./logs/valgrind_log.txt"

CC = g++

DIR_GRAPHVIZ = ./graphviz
DIR_LANG = ./language
DIR_CPU = ./CPU


all:  Make_l Make_c
# all:  Make_l 

clean_files: clean_df clean_gf 

clean_obj: clean_l clean_g clean_c

clean_gf:
	cd $(DIR_GRAPHVIZ) && make clean_files

clean_g:
	cd $(DIR_GRAPHVIZ) && make clean

clean_l:
	cd $(DIR_LANG) && make clean

clean_c:
	cd $(DIR_CPU) && make clean_obj

Make_g: 
	cd $(DIR_GRAPHVIZ) && make

Make_l: 
	cd $(DIR_LANG) && make

Make_c: 
	cd $(DIR_CPU) && make