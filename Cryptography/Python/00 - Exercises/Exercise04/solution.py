'''
You have sniffed 100 digests of non-salted passwords encrypted with Salsa20 and a fixed nonce.

Plan an attack.

NB: the enc_dgst file contains the 100 sniffed digests.

----------------------------------------------------------------------------------------------------

1) Attacks based on statistical frequency are wrong, because these are digests and they are (at least
   they should) be completely random. 
2) A naive approach counts 

'''

from base64 import b64decode
from string import ascii_letters, printable
from numpy import zeros
from Crypto.Util.strxor import strxor

# Some constants
THRESHOLD = .95
HEX_DIGITS = ['a', 'b', 'c', 'd', 'e', 'f', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',]

if __name__ == '__main__':
    
    # Loading data from file
    encoded_digests = []
    with open('enc_dgst', 'r') as f_in:
        for line in f_in:
            encoded_digests.append(line[:-1])
    f_in.close()
    digests = [b64decode(encoded_digests[i]) for i in range(len(encoded_digests))]
    
    # Computing some statistics
    longest_digest = max(digests, key = len)
    max_len = len(longest_digest)
    shortest_digest = min(digests, key = len)
    min_len = len(shortest_digest)
    
    print('------- statistics -------')
    print('')
    print('> number of digest: ', len(digests))
    print('> maximum length:   ', max_len)
    print('> minimum length:   ', min_len)
    print('--------------------------')
    
    
    # Brute-force all bytes (based on statistical occurrences)
    candidates_list = []
    for byte in range(min_len):
        counters = zeros(256, dtype = int)
        
        # Trying all possible values
        for guess in range(256):
            for digest in digests:
                if chr(digest[byte] ^ guess) in HEX_DIGITS:
                    counters[guess] += 1
        
        # Just a bit of reordering
        max_matches = max(counters)
        match_list = sorted([(counters[i], i) for i in range(256)], reverse = True)
        
        # Filtering out poor candidates
        candidates = []
        for pair in match_list:
            if pair[0] < max_matches * THRESHOLD:
                break
            candidates.append(pair)
        
        # Updating candidates for this particular byte
        candidates_list.append(candidates)       
    
    # Attempting to get a keystream with the best candidates
    keystream = b''
    for c in candidates_list:
        keystream += c[0][1].to_bytes(1, 'big')
    
    # Printing result (hopefully)
    for d in digests:
        digest_recovered = strxor(d[:min_len], keystream)
        print(digest_recovered)
            
    
    