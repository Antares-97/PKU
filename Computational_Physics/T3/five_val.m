function f=five_val(v, A, b)
x = v(1);
y = v(2);
z = v(3);
p = v(4);
q = v(5);
f = (x-0.718).^2+(((y+0.718))./2).^2+(z-0.2).^2+(((p+2))./0.1).^2+q.^2+(y-z-1.5).^2;
s = fmincon(@five_val, v, A, b);
print(s);
