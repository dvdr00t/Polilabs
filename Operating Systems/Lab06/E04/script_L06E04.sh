# # # # # # # # # # # # # # #
#                           #
# textfile: lab06e04in.txt  #
#                           #
# # # # # # # # # # # # # # #


# (1) The list of the same data ordered by reverse id number.
sort -b -n -r -k 1,1 lab06e04in.txt

# (2) The list of the same data ordered by surname
sort -b -k 2,2 lab06e04in.txt

# (3) The list of students who attended in 2011-2012
grep "2011/2012" lab06e04in.txt

# (4) The list of students in computer science (INF1T3) ordered by id number.
grep "INF1T3" lab06e04in.txt | sort -b -n -k 1,1 

# (5) The list of students with an id that terminates with 2 or with 4, and ordered by surname
grep -e '^[0-9]*[2|4]\> .' lab06e04in.txt | sort -b -k 2,2

# (6) The list of TCL students (TLC1T3), that they still have to attend (i.e., row with string “Da frequentare”),
# sorted in descending order by id number.
grep "TLC1T3" lab06e04in.txt | grep "Da frequentare" | sort -b -n -r -k 1,1

# (7) The list of students which name contains two A letters separated by any two characters
grep -e 'A..A' lab06e04in.txt

# (8) The list of students which name contains two A letters separated by any two characters, but with the “A” characters 
# that do not start or end the name itself.
grep -e 'A..A' lab06e04in.txt | grep -v '\<A' | grep -v 'A\>'

# (9) Take only the id numbers from the file, eliminate all the digit equal to 0, substitute all the digit 
# equal to 8 with 1, sort the resulting list and eliminate all the duplicates.
cut -d " " -f 1,1 lab06e04in.txt | tr -d 0 | tr 8 1 | sort -n -u

# (10) Perform all the operation of the previous point (point 9), but print only duplicate lines, counting them
cut -d " " -f 1,1 lab06e04in.txt | tr -d 0 | tr 8 1 | sort -n | uniq -d