hash_value_sha256 = "94afc306ec79b7a87ba30e38634dd9979443ace982f69d9ae5272976135f41ec"
hash_value_sha512 = "76cd4410dc53bca770ff72ad40cd84a8df543e083618cc69319365e039d9e1cc5f90c31911cc5e410e6938cc96058239bd6307775ec2261eba127a5f2b2f5448"

result = []
for i in range(len(hash_value_sha256)):
    result.append(hex(int(hash_value_sha256[i], 16) ^ (int(hash_value_sha512[i], 16) & int(hash_value_sha512[i+len(hash_value_sha256)], 16)))[2:])

print(''.join(result))