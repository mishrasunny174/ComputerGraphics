import subprocess

FLAG = "DrgnS{Dune?CounterStrike?}"

# Create MIRAGE.PAL.
with open("MIRAGE.PAL.IN", "r") as f:
  pal = [x for x in f.read().splitlines() if x != "0 0 0"]

pal_keys = []
for p in pal[3:]:
  x = [int(y) for y in p.split(" ")]
  k = (x[0] >> 2) ^ ((x[1] >> 2) << 1) ^ ((x[2] >> 2) << 2)
  pal_keys.append(k)

print pal_keys[0:26]

i = 0
while i < len(FLAG):
  ch0 = ord(FLAG[i + 0]) ^ pal_keys[i + 0]
  ch1 = ord(FLAG[i + 1]) ^ pal_keys[i + 1]
  v = ch0 | (ch1 << 8) | (0b11 << 16)

  b = (((v >>  0) & 0b111111) << 2) | 0b11
  g = (((v >>  6) & 0b111111) << 2) | 0b10
  r = (((v >> 12) & 0b111111) << 2) | 0b01
  
  pal.append("%u %u %u" % (r, g, b))
  i += 2

while len(pal) < 256 + 3:
  pal.append("0 0 0")

with open("MIRAGE.PAL", "w") as f:
  for p in pal:
    f.write("%s\n" % p)

# Compile CHECKER.ASM.
subprocess.check_output("nasm checker.asm", shell=True)

# Create CHECKER.CPP.
with open("checker", "rb") as f:
  d = f.read()

KEY = "CONFidence CTF 2016"

with open("checker.cpp", "w") as f:
  i = 0
  j = 0
  f.write("unsigned char checker_code[%u] = {\n" % len(d));

  for ch in d:
    if i == 0:
      f.write("  ")

    ch = ord(ch) ^ ord(KEY[j % len(KEY)])
    f.write("0x%.2x" % ch)

    j += 1
    if j != len(d):
      f.write(", ")

    i += 1
    if i == 8:
      f.write("\n")
      i = 0

  if i != 0:
    f.write("\n")

  f.write("};\n");



