'''
Coppersmith's Short Pad Attack

Given a message m, padded with two random values r1 and r2, composed of at most p bits

    m1 = m || r1
    m2 = m || r2
    
and encrypted as:

    c1 = m1^e mod n
    c2 = m2^e mod n
    
an attacker accessing the public key (e, n) used to encrypt c1 and c2 can efficiently
recover m if:

    p < floor(n / e^2)
    
based on the Franklin-Reiter Attack on related messages.

'''