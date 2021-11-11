import numpy as np
import string
import random
import itertools

digits = [d for d in string.digits]
lower_case_letters = [l for l in string.ascii_lowercase]
upper_case_letters = [u for u in string.ascii_uppercase]
symbols = ['!', '?', '$', '#', '&', '*']

num_passwords = 10000
max_characters_numeric = 6
max_characters_lc_alphanum = 4
max_characters_alphanum = 4
max_characters_all = 4

# N represents the number of passwords
# M represents the number of characters
def generate_passwords(N, M, char_bank):
  passwords = []
  for j in range(N):
    passwords.append(''.join([random.choice(char_bank) for _ in range(M)]))
  return np.array(passwords, dtype=object)

# M represents the number of characters
def generate_permutations(M, char_bank):
  permutations = []
  combinations_raw = list(itertools.product(char_bank, repeat = M))
  for c in combinations_raw:
    permutations.append(''.join(c)) # We store the hash since Numba doesn't support strings
  return np.array(permutations, dtype=object)

# Numeric Characters
numeric_chars = digits
numeric_passwords = generate_passwords(num_passwords, max_characters_numeric, numeric_chars)

# Lowercase Alphanumeric Characters
lc_alphanum_chars = lower_case_letters + digits
lc_alphanum_passwords = generate_passwords(num_passwords, max_characters_lc_alphanum, lc_alphanum_chars)

# Alphanumeric (lowercase & uppercase) Characters
alphanum_chars = lower_case_letters + upper_case_letters + digits
alphanum_passwords = generate_passwords(num_passwords, max_characters_alphanum, alphanum_chars)

# Alphanumeric Characters and Symbols
all_chars = lower_case_letters + upper_case_letters + digits + symbols
all_passwords = generate_passwords(num_passwords, max_characters_all, all_chars)

# We go with a toggle system to avoid overloading the RAM
flag = 3

# Numeric Characters
if flag == 0:
  permutations = generate_permutations(max_characters_numeric, numeric_chars)
  passwords = numeric_passwords
  max_char = max_characters_numeric
  perm_type = 'numeric_chars'

# Lowercase Alphanumeric Characters
if flag == 1:
  permutations = generate_permutations(max_characters_lc_alphanum, lc_alphanum_chars)
  passwords = lc_alphanum_passwords
  max_char = max_characters_lc_alphanum
  perm_type = 'lc_alphanum_chars'

# Alphanumeric (lowercase & uppercase) Characters
if flag == 2:
  permutations = generate_permutations(max_characters_alphanum, alphanum_chars)
  passwords = alphanum_passwords
  max_char = max_characters_alphanum
  perm_type = 'alphanum_chars'

# Alphanumeric Characters and Symbols
if flag == 3:
  permutations = generate_permutations(max_characters_all, all_chars)
  passwords = all_passwords
  max_char = max_characters_all
  perm_type = 'all_chars'

file_name = perm_type + '_' + str(max_char) + '.txt'
f = open(file_name, 'w')
f.write(str(len(permutations)) + '\n')
for p in permutations:
  f.write(p + '\n')
f.close()

file_name = 'passwords_' + str(max_char) + '.txt'
f = open(file_name, 'w')
f.write(str(len(passwords)) + '\n')
for p in passwords:
  f.write(p + '\n')
f.close()