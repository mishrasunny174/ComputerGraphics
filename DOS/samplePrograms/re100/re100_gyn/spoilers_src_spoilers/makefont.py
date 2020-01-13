with open("gfx.raw", "rb") as f:
  d = f.read()

o = ""

for j in xrange(7):
  for i in xrange(17):
    
    for y in xrange(16):
      b = 0
      for x in xrange(8):
        pos_x = i * 9 + x
        pos_y = j * 16 + y
        pos = pos_y * 153 + pos_x
        
        if d[pos] == "\xff":
          b |= 1 << (7 - x)

      o += chr(b)
    o += "\0" * 16

with open("logo.fnt", "wb") as f:
  f.write(o)


