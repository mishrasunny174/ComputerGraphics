Solution
========

Note: The crackme was tested in DOSBox and Bochs+FreeDOS.

1. Run the crackme, see what's up. The dragon is red, we must make it green.
2. Look around the code, get to know it.
3. Since this is all about color, you'll eventually run into this function:

sub_100E4 proc near

arg_0= byte ptr  4
arg_2= byte ptr  6
arg_4= byte ptr  8
arg_6= byte ptr  0Ah

push    bp
mov     bp, sp
xor     ax, ax
call    @__StackCheck$q4Word ; Stack overflow check (AX)
mov     al, [bp+arg_6]
mov     dx, 3C8h
out     dx, al
mov     al, [bp+arg_4]
mov     dx, 3C9h
out     dx, al
mov     al, [bp+arg_2]
mov     dx, 3C9h
out     dx, al
mov     al, [bp+arg_0]
mov     dx, 3C9h
out     dx, al
pop     bp
retn    8
sub_100E4 endp

4. Analyze it and you'll know that it sets RGB palette for text mode.
5. Check x-refs - most calls come from one function, but there are also two calls from sub_104C5.
6. Notice that these calls are conditional calls. Depending on <something> either red or green colors are set in the palette.
7. The <something> is arg_0 of the sub_104C5 function, which is byte_16C33 (a global variable).
8. byte_16C33 is set in only one place (main function), to whatever sub_1067D returns.
9. sub_1067D looks (and is) a password checking function. Analyze it.
10. During analysis you will notice that password length must be 19.
11. You'll also quickly spot that first character is obviously 'D'.
12. And you'll spot a series of very simple SUB and XOR equasions that all must be met.
13. You should also spot that knowing the first letter means you can quickly XOR it with each constant and you'll get the flag.
14. Enter the flag, watch the logo turn green.
