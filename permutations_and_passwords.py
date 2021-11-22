import numpy as np
import string
import random

digits = [d for d in string.digits]
lower_case_letters = [l for l in string.ascii_lowercase]
upper_case_letters = [u for u in string.ascii_uppercase]
symbols = ['!', '?', '$', '#', '&', '*']

num_passwords = 1000
max_character = 10

# N represents the number of passwords
# M represents the number of characters
def generate_passwords(N, M, char_bank):
  passwords = []
  for j in range(N):
    passwords.append(''.join([random.choice(char_bank) for _ in range(M)]))
  return np.array(passwords, dtype=object)

# Numeric Characters
numeric_chars = digits
numeric_passwords = generate_passwords(num_passwords, max_character, numeric_chars)

# Lowercase Alphanumeric Characters
lc_alphanum_chars = lower_case_letters + digits
lc_alphanum_passwords = generate_passwords(num_passwords, max_character, lc_alphanum_chars)

# Alphanumeric (lowercase & uppercase) Characters
alphanum_chars = lower_case_letters + upper_case_letters + digits
alphanum_passwords = generate_passwords(num_passwords, max_character, alphanum_chars)

# Alphanumeric Characters and Symbols
all_chars = lower_case_letters + upper_case_letters + digits + symbols
all_passwords = generate_passwords(num_passwords, max_character, all_chars)

file_name = './passwords/digits_passwords_' + str(max_character) + '.txt'
f = open(file_name, 'w')
f.write(str(len(numeric_passwords)) + '\n')
for p in numeric_passwords:
  f.write(p + '\n')
f.close()

file_name = './passwords/lc_alphanum_passwords_' + str(max_character) + '.txt'
f = open(file_name, 'w')
f.write(str(len(lc_alphanum_passwords)) + '\n')
for p in lc_alphanum_passwords:
  f.write(p + '\n')
f.close()

file_name = './passwords/alphanum_passwords_' + str(max_character) + '.txt'
f = open(file_name, 'w')
f.write(str(len(alphanum_passwords)) + '\n')
for p in alphanum_passwords:
  f.write(p + '\n')
f.close()

file_name = './passwords/all_passwords_' + str(max_character) + '.txt'
f = open(file_name, 'w')
f.write(str(len(all_passwords)) + '\n')
for p in all_passwords:
  f.write(p + '\n')
f.close()