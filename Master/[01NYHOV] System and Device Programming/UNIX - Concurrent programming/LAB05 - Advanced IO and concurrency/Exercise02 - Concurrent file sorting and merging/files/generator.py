'''
Generates A LOT of 32-bits numbers in A LOT of files, in order to obtain a computational
onerous dataset that will be fed to the two algorithms in order to evaluate their 
performances.
'''

import random

BITS = 32
MAX_FILES = 20      # maximum number of files to be generated
MAX_VALUE = 1000   # maximum number of values to be generated in a single file
NAME_PREFIX = "./files/f"   # prefix name of the file

def generate_files():

    # ITERATIONS OVER ALL FILES
    for i in range(MAX_FILES):

        # CREATING AND OPENING THE FILE
        filename = NAME_PREFIX + str(i+1) + ".bin"
        with open(filename, "wb") as f:

            # GENERATING RANDOM VALUES
            nvalues = random.randrange(1, MAX_VALUE)
            f.write(nvalues.to_bytes(4, byteorder="little"))
            for n in range(nvalues):
                f.write((random.getrandbits(BITS)).to_bytes(4, byteorder="big"))
        f.close()

    # CREATING OUTPUT FILE 
    f = open(NAME_PREFIX + "out.bin", "wb")
    f.close()


if __name__ == '__main__':
    try:
        generate_files()
    except:
        print("[ERROR] generation of files failed.")