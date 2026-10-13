% 连续化
% dsys=tf([0.000999500166625008],[1 -0.999000499833375],0.0001)%离散传递函数表达式
% scsys=d2c(dsys,'zoh');
% [num,den]=tfdata(scsys,'v')
% H=tf([num],[den])%连续传递函数表达式

% %离散化
format longG
sys = tf(0.1,[1 0.1]);
zh = c2d(sys,0.0001,'zoh');
[num,den] = tfdata(zh,'v');
