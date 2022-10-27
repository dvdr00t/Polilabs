# (1) Print the list of the files in the tree with root (/) that have extension “.exe”, dimension greater than 1024 bytes, 
# and that they are executable
find / -name "*.app" -perm +111 -size +1k

# (2) Print the path of all the files with extension “.c”, and with a dimension of 100 bytes.
find / -name "*.c" -size 100c

# (3) Print all the files of your user, of regular type, that contain in the name at least one a and at least 
# one A in an undefined order (i.e., a...A or A...a). Of these files print the last 3 lines.
find / -type f -regex "(.*A.*a.*)|(.*a.*A.*)" -exec tail -n 3 '{}' \;

# (4) Print all the files of your user from the level of depth 3 to the level of depth 5, with a dimension of at least 250 bytes, 
# and with the name of exactly 4 characters. Of these files count the number of characters. Repeat the same operation counting the 
# number of rows
find / -type f -mindepth 3 -maxdepth 5 -user username -size +249c -name "????" -exec wc '{}' \;

# (5) Print all the files in the system with an extension of 3 characters, with the name beginning with an A and with 3 consecutive z. 
# Of these files create a compressed version (see the manual, man, of the command gzip).
find / -name "A*zzz*.???" -exec tar -zcvf '{}'.tar.gz '{}' \;

# (6) Print the content of all the directories, starting from the root of the file system, and with name bin.
find / -type d -name "bin" -exec ls '{}' \;

# (7) Find all the files with the extension .c of the other users (excluding your user!), and belonging to your group.
find / -name "*.c" ! -user username

# (8) Modify the permissions of all your files in the first level of the sub-tree of the root of the file system, so that 
# all users other than the owner have no permission.
find / -maxdepth 1 -user username -exec chmod go-xrw '{}' \; 