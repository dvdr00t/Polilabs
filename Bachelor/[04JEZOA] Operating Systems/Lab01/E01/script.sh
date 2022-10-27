# Generating the tree of directories
mkdir osEx01
mkdir osEx01/src osEx01/bin osEx01/test
mkdir osEx01/src/include osEx01/src/lib
mkdir osEx01/test/script osEx01/test/results

# Downloading the file in the osEx01/src folder
wget https://www.skenz.it/listing/os/lab/lab01e01in.txt -P osEx01/src

# Copying the file in the target directories
cp osEx01/src/lab01e01in.txt osEx01/test
cp osEx01/src/lab01e01in.txt osEx01/test/script
cp osEx01/src/lab01e01in.txt osEx01/test/results

# Verifying the existence of the file and its rights
ls -l osEx01/src

# Using the command more, less and cat to see the content of the file
echo more command:
more osEx01/src/lab01e01in.txt

echo less command:
less osEx01/src/lab01e01in.txt

echo cat command:
cat osEx01/src/lab01e01in.txt

# Verifying that all the file have the same content
diff osEx01/src/lab01e01in.txt osEx01/test/lab01e01in.txt
diff osEx01/src/lab01e01in.txt osEx01/test/script/lab01e01in.txt
diff osEx01/src/lab01e01in.txt osEx01/test/results/lab01e01in.txt

# Deleting all the files previously created
rm osEx01/test/lab01e01in.txt
rm osEx01/test/script/lab01e01in.txt
rm osEx01/test/results/lab01e01in.txt
