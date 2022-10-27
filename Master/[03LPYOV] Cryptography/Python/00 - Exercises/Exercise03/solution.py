'''
A smart cryptographer was asked to generate an RSA key for generating digital signatures.
To optimize the performance of the server only generating signatures, he inverted the public and private exponents:

    (e = the long number, d = F4), 
    
as they are commutative and F4 has only two bits in its binary representation.

1) Even assuming that attackers cannot guess the private exponent, what attacks can be mounted?
2) What is the additional side effect of this smart approach?
'''

# 1) Low (private) Exponent Attack
# 2) Computation effort is ALL on the verifier side (it is mandatory to design a system assuming the client has (almost) no computational power)