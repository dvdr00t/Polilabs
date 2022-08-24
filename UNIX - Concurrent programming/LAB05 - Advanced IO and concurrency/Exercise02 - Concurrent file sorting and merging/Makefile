#   VARIABLES
# -------------
F1   := "files/f1.bin"
F2   := "files/f2.bin"
F3   := "files/f3.bin"
F4   := "files/f4.bin"
F5   := "files/f5.bin"
F6   := "files/f6.bin"
F7   := "files/f7.bin"
F8   := "files/f8.bin"
F9   := "files/f9.bin"
F10   := "files/f10.bin"
F11   := "files/f11.bin"
F12   := "files/f12.bin"
F13   := "files/f13.bin"
F14   := "files/f14.bin"
F15   := "files/f15.bin"
F16   := "files/f16.bin"
F17   := "files/f17.bin"
F18   := "files/f18.bin"
F19   := "files/f19.bin"
F20   := "files/f20.bin"
FOUT := "files/fout.bin"

# GENERATE SOME BIG FILES TO PERFORM THE TIME COMPARISON BETWEEN THE ALGORITHMS
generate_files:
	@ clear
	@ python3 files/generator.py

check_file:
	@ clear
	@ python3 files/checker.py

target: 
	@ rm -f algorithms/merge_notOPT
	@ rm -f files/out.bin
	@ rm -f lib/myLibrary.o
	@ gcc -c lib/myLibrary.c
	@ gcc -o merge_notOPT algorithms/merge_notOPT.c myLibrary.o -pthread
	@ echo "... executing program ..."	
	@ ./merge_notOPT ${F1} ${F2} ${F3} ${F4} ${F5} ${F6} ${F7} ${F8} ${F9} ${F10} ${F11} ${F12} ${F13} ${F14} ${F15} ${F16} ${F17} ${F18} ${F19} ${F20} ${FOUT}

target_OPT: 
	@ rm -f algorithms/merge_OPT
	@ rm -f files/out.bin
	@ rm -f lib/myLibrary.o
	@ gcc -c lib/myLibrary.c
	@ gcc -o merge_OPT algorithms/merge_OPT.c myLibrary.o -pthread
	@ echo "... executing program ..."	
	@ ./merge_OPT ${F1} ${F2} ${F3} ${F4} ${F5} ${F6} ${F7} ${F8} ${F9} ${F10} ${F11} ${F12} ${F13} ${F14} ${F15} ${F16} ${F17} ${F18} ${F19} ${F20} ${FOUT}

clean: 
	@ clear
	@ rm -f files/f*
	@ rm -f myLibrary.o
	@ rm -f merge_notOPT
	@ rm -f merge_OPT
	@ echo "[!] Removed all generated files"