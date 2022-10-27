'''
Checks if the output file contains numbers actually sorted in increasing order
'''
NAME_PREFIX = "./files/f"   # prefix name of the file

def check():

    # OPENING FILE
    array = []
    with open(NAME_PREFIX + "out.bin", "rb") as f:
        n = f.read(4)
        total = int.from_bytes(n, byteorder="little")

        for _ in range(total):
            n = f.read(4)
            array.append(int.from_bytes(n, byteorder="little", signed=True))
    f.close()

    if (all(array[i] <= array[i+1] for i in range(len(array) - 1))):
        print("[!] OK! Output is sorted correctly")
    else:
        print("[!] mmm... something does not work")
    

if __name__ == "__main__":
    try:
        check()
    except:
        print("[ERROR] check() failed execution")