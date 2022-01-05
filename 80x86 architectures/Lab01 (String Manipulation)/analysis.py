def main():

	first_line = "nel mezzo del cammin di nostra vita"
	second_line = "mi ritrovai per una selva oscura"
	third_line = "che la diritta via era smarrita"
	fourth_line = "ahi quanto a dir qual era e cosa dura"

	analysis(first_line)
	analysis(second_line)
	analysis(third_line)
	analysis(fourth_line)

	return

def analysis(line: str):
	mfv = search_mfc(line)
	search_mfc2(line, mfv//2)
	return

def search_mfc(line: str):
	freq_dict = {}
	for char in line:
		if char != " ":
			if char not in freq_dict:
				freq_dict[char] = 1
			else:
				freq_dict[char] += 1

	values_list = list(freq_dict.values())
	values_list.sort(reverse=True)
	mfv = values_list[0]
	for (key, value) in freq_dict.items():
		if value == mfv:
			mfc = key # Most Frequent Character
	print("The most frequent char is: " + str(mfc) + " with frequency: " + str(mfv))
	return mfv

def search_mfc2(line: str, max2: int):
	freq_dict = {}
	for char in line:
		if char != " ":
			if char not in freq_dict:
				freq_dict[char] = 1
			else:
				freq_dict[char] += 1

	print("Characters with frequency >= max2 are:")
	for (key, value) in freq_dict.items():
		if value > max2:
			print(key + " with frequency: " + str(value))
	return
main()