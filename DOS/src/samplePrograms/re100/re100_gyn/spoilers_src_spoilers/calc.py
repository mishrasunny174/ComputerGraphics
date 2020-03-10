f = "DrgnS{TextModeFTW!}"
for i in xrange(len(f) - 1):
  x = ord(f[i]) ^ ord(f[i+1])
  print "  if ord(s[%i]) xor ord(s[%i]) = %i then inc(correct);" % (i+1, i+2, x)

  x = ord(f[i]) - ord(f[i+1])
  print "  if ord(s[%i]) - ord(s[%i]) = %i then inc(correct);" % (i+1, i+2, x)  

