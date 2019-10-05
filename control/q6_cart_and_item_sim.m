%% initialize

close all;
clear;
clc;

s = tf('s');

%% plant parameters

mc = 1;
mi = 1;
b = 10;
k = 40000;

Pcart = (mi*s^2+b*s+k)/(mc*mi*s^4+(mc+mi)*b*s^3+(mc+mi)*k*s^2);
Pitem = (b*s+k)/(mc*mi*s^4+(mc+mi)*b*s^3+(mc+mi)*k*s^2);
flag_Pitem = true; % control object is Pitem
if flag_Pitem
    P = Pitem;
else
    P = Pcart;
end

%% PID by Hand Tuning

Kp = 1;
Ki = 0;
Kd = 0;
Tf = 0.01;

C = pid(Kp,Ki,Kd,Tf)

Pn = 1/((mc+mi)*s^2);

figure;
bode(Pcart,Pitem,Pn);
legend;

%% plot nyquist

Gol = P*C;

figure;
NyquistOption = nyquistoptions;
NyquistOption.ShowFullContour = 'off'; 
NyquistOption.Xlim = [-3 1];
NyquistOption.Ylim = [-2 2];
nyquist(Gol,NyquistOption);

figure; margin(Gol);

%% Plot Step

Gcl = feedback(P*C,1);

figure;
step(Gcl);
StepInformation = stepinfo(Gcl)

sim('sim_cart_and_item.slx');
