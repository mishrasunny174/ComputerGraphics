{tpc -V -$D+ -$L+ -$N+ hi}
uses dos, crt;

{ Flaga: DrgnS[TextModeFTW!] z klamrami zamiast [ }

const
  Texts : array [1..30] of string = (
    'Our dragon must be green again! Please help!',
    'Enter a secret password that will help our dragon!',
    'You can press Esc to exit at any time.',
    'Why are you still here? You should be hacking!',
    'But since you are still here, here''s a hint for you:',
    'Just kidding :) No hints!',
    'Isn''t text mode cool? Why did we ever switch to graphical mode...',
    'I mean, you can do graphics here as well...',
    'Not saying that it''s simple or anything...',
    '...and you lose some characters, but who needs them anyway.',
    'Did you play any good DOS games lately?',
    'You did? That''s great!',
    'You didn''t? What''s that? You don''t know what DOS is?',
    'No no no, it''s not "Denial of Service", that''s "DoS".',
    'Well, DOS is a legacy OS that some old companies still use.',
    'Seriously.',
    'Yes, I know what year it is.',
    'Anyway, DOS was great! If you wanted to play a game, you just had to:',
    '- configure the IRQ and DMA of your sound card',
    '(if you had one that is)',
    '- load a mouse driver',
    '(if you had one that is)',
    '- sometimes load a CD-ROM driver',
    '(if you had one that is)',    
    '- stay up until 4am trying to get just 16 KB more free memory',
    'Anyway, in case you are wondering, the answer is "No."',
    'As in: No, these texts are random and not related to the crackme.',
    'I just thought it would be boring without them.',
    'You agree? That''s great!',
    'Anyway, good luck with the challenge. Rewinding this text feed now.'
    );
  textsdelay = 5.0;
  textscount = 30;

var
  theend : boolean;
  victory : boolean;
  ch : char;
  s : string;

  txtid : word;
  nextchg : single;

procedure MapFont;
begin
  portw[$3ce] := 5;
  portw[$3ce] := $406;
  portw[$3c4] := $402;
  portw[$3c4] := $604;
end;

procedure UnmapFont;
begin
  portw[$3c4] := $302;
  portw[$3c4] := $204;  
  portw[$3ce] := $1005;
  portw[$3ce] := $e06;
end;

procedure ReadFont;
var
  f : file of byte;
  b : byte;
  i : word;
begin
  assign(f, 'logo.fnt');
  reset(f);
  i := 128 * (16 + 16);
  while not eof(f) do
  begin
    read(f, b);
    mem[$a000:i] := b;
    {write(ord(mem[$a000:i]));}
    inc(i);
  end;
  close(f);
end;

Procedure SetRGB(c, r, g, b : Byte);
Begin
  Port [$3c8] := c;
  Port [$3c9] := r;
  Port [$3c9] := g;
  Port [$3c9] := b;
End;

procedure ShowLogo;
var
  i, j: integer;
  x, y: integer;
  p, n: word;
begin

  x := 48; { 31 }
  y := 3;
  n := 128;
  for j := 0 to (7 - 1) do
  begin
    for i := 0 to (17 - 1) do
    begin
      p := (y + j) * 80 + (x + i);
      mem[$B800:p * 2 + 0] := n;
      mem[$B800:p * 2 + 1] := 8 + j;
      inc(n);
    end;
  end;

  x := 12;

  TextColor(6);
  gotoxy(x, 3);  writeln('          ______         __      ');
  gotoxy(x, 4);  writeln('         /_  __/_ ______/ /  ___ ');
  gotoxy(x, 5);  writeln('          / / / // / __/ _ \/ _ \');
  gotoxy(x, 6);  writeln('         /_/  \_,_/_/ /_.__/\___/');
  gotoxy(x, 7);  writeln('');
  TextColor(7);  
  gotoxy(x, 8);  writeln('           CONFidence CTF 2015');
  TextColor(6);  
  gotoxy(x, 9);  writeln('    __   __        __              ___');
  gotoxy(x, 10); writeln('   /  ` |__)  /\  /  ` |__/  |\/| |__');
  gotoxy(x, 11); writeln('   \__, |  \ /~~\ \__, |  \  |  | |___');  
end;


function GetT:single;
var
  h, m, s, hs: word;
begin
  GetTime(h, m, s, hs);
  GetT := (hs + s * 100 + m * 60 * 100 + h * 60 * 60 * 100) / 100.0;
end;


procedure AnimateOnIdle(v:boolean);
var
  gr : single;
  i, st, l, y, x : word;
  t : single;

begin  

  repeat
    t := GetT;    
    for i := 0 to (7 - 1) do
    begin
      gr := Sin(t * 3.0 + i / 4.0) * 25 + 31;
      if v then SetRGB(56 + i, 0, round(gr), 0) else SetRGB(56 + i, round(gr), 0, 0);
    end;

    if t > nextchg then
    begin
      inc(txtid);
      if txtid > textscount then txtid := 1;      
      nextchg := t + textsdelay;

      y := 23;

      for i := y * 80 * 2 to (y+1) * 80 * 2 - 1 do
      begin
        mem[$b800:i] := 0;
      end;

      l := length(Texts[txtid]);
      st := (80 - l) div 2;

      for i := 0 to l - 1 do
      begin

        x := y * 80 * 2 + (st + i) * 2;
        mem[$b800:x + 0] := ord(Texts[txtid][i+1]);
        mem[$b800:x + 1] := 6;
      end;

      gotoxy(22+12+length(s), 18);
    end;
  until KeyPressed;  
end;

function CheckWin(s:string):boolean;
var 
  correct : word;
begin
  correct := 0;
  CheckWin := false;

  if length(s) <> 19 then exit;
  if ord(s[1]) xor ord('D') = 0 then inc(correct);
  if ord(s[1]) xor ord(s[2]) = 54 then inc(correct);
  if ord(s[1]) - ord(s[2]) = -46 then inc(correct);
  if ord(s[2]) xor ord(s[3]) = 21 then inc(correct);
  if ord(s[2]) - ord(s[3]) = 11 then inc(correct);
  if ord(s[3]) xor ord(s[4]) = 9 then inc(correct);
  if ord(s[3]) - ord(s[4]) = -7 then inc(correct);
  if ord(s[4]) xor ord(s[5]) = 61 then inc(correct);
  if ord(s[4]) - ord(s[5]) = 27 then inc(correct);
  if ord(s[5]) xor ord(s[6]) = 40 then inc(correct);
  if ord(s[5]) - ord(s[6]) = -40 then inc(correct);
  if ord(s[6]) xor ord(s[7]) = 47 then inc(correct);
  if ord(s[6]) - ord(s[7]) = 39 then inc(correct);
  if ord(s[7]) xor ord(s[8]) = 49 then inc(correct);
  if ord(s[7]) - ord(s[8]) = -17 then inc(correct);
  if ord(s[8]) xor ord(s[9]) = 29 then inc(correct);
  if ord(s[8]) - ord(s[9]) = -19 then inc(correct);
  if ord(s[9]) xor ord(s[10]) = 12 then inc(correct);
  if ord(s[9]) - ord(s[10]) = 4 then inc(correct);
  if ord(s[10]) xor ord(s[11]) = 57 then inc(correct);
  if ord(s[10]) - ord(s[11]) = 39 then inc(correct);
  if ord(s[11]) xor ord(s[12]) = 34 then inc(correct);
  if ord(s[11]) - ord(s[12]) = -34 then inc(correct);
  if ord(s[12]) xor ord(s[13]) = 11 then inc(correct);
  if ord(s[12]) - ord(s[13]) = 11 then inc(correct);
  if ord(s[13]) xor ord(s[14]) = 1 then inc(correct);
  if ord(s[13]) - ord(s[14]) = -1 then inc(correct);
  if ord(s[14]) xor ord(s[15]) = 35 then inc(correct);
  if ord(s[14]) - ord(s[15]) = 31 then inc(correct);
  if ord(s[15]) xor ord(s[16]) = 18 then inc(correct);
  if ord(s[15]) - ord(s[16]) = -14 then inc(correct);
  if ord(s[16]) xor ord(s[17]) = 3 then inc(correct);
  if ord(s[16]) - ord(s[17]) = -3 then inc(correct);
  if ord(s[17]) xor ord(s[18]) = 118 then inc(correct);
  if ord(s[17]) - ord(s[18]) = 54 then inc(correct);
  if ord(s[18]) xor ord(s[19]) = 92 then inc(correct);
  if ord(s[18]) - ord(s[19]) = -92 then inc(correct);

  if correct = 37 then CheckWin := true;

end;

procedure DrawEverythingInitially;
begin
  SetRGB(4,      25,  25,  63);  
  
  SetRGB(5,       0,  0,  10);  

  SetRGB(1,       0,  0,  31);
  SetRGB(2,       0,  0,  47);
  SetRGB(3,       0,  0,  63);  

  
  SetRGB({6} 20, 63,  0,  0);
  SetRGB(7,      63, 63, 63);

  ClrScr;
  MapFont;
  ReadFont;
  UnmapFont;
  ShowLogo;

  TextColor(7);
  TextBackground(1); gotoxy(22, 17); write('                                    ');
  TextBackground(2); gotoxy(22, 18); write('  Password:                         ');
  TextBackground(3); gotoxy(22, 19); write('                                    ');

  
end;

begin
  theend := false;
  s := '';
  victory := false;
  txtid := 0;
  nextchg := 0;

  DrawEverythingInitially;
  repeat
    TextBackground(5); gotoxy(22+12, 18); write('                       ');
    TextColor(4); gotoxy(22+12, 18); write(s);

    { ustaw kursor na koniec }
    gotoxy(22+12+length(s), 18);

    AnimateOnIdle(victory);
    ch := ReadKey;
    if ch = #27 then break;

    if ch = #0 then
    begin
      ch := ReadKey;
      { don't care about special cases }      
      continue;
    end;

    if (ord(ch) >= $20) and (ord(ch) <= $7e) and (length(s) < 22) then
    begin
      s := s + ch;
    end;

    if (ch = #8) and (length(s) > 0) then
    begin
      Delete(s, length(s), 1);
    end;

    victory := CheckWin(s);

  until false;
end.


