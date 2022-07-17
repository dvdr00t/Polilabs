'''
Implementing Diffie-Hellman protocol with HazMar library
'''

from cryptography.hazmat.primitives.hashes import SHA256
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric import dh
from cryptography.hazmat.primitives.kdf.hkdf import HKDF

# + ----------------------- +
# |         ALICE           |
# + ----------------------- +

# Generating parameters (fixed and long term)
parameters = dh.generate_parameters(
    generator = 2,
    key_size = 1024,
    backend = default_backend()
)

# Generating keys
KEY_priA = parameters.generate_private_key()
KEY_pubA = KEY_priA.public_key()

# + ----------------------- +
# |           BOB           |
# + ----------------------- +
KEY_priB = parameters.generate_private_key()
KEY_pubB = KEY_priB.public_key()

shared_secret = KEY_priA.exchange(KEY_pubB)
derived_key = HKDF(
    algorithm = SHA256(),
    length = 32,
    salt = None,
    info = b'agreed data',
    backend = default_backend()
).derive(shared_secret)

