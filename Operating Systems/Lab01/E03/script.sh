# Downloading the file in a target folder
mkdir testdir
wget https://www.skenz.it/listing/os/lab/lab01e01in.txt -P testdir

# Creating the copy of the file, the hard link and the soft link
cp testdir/lab01e01in.txt testdir/lab01e01in.copy
ln testdir/lab01e01in.txt testdir/lab01e01in.hl
ln -s testdir/lab01e01in.txt testdir/lab01e01in.sl

# Vien info about the file
ls -l testdir

# Rename the file .txt and see the info again
mv lab01e01in.txt lab01e01in.backup
ls -l testdir


