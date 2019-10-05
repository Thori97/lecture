%% initialize

close all;
clear;
clc;

s = tf('s');

%% plant parameters

m = 1;
b = 5;
k = 10; % please try k = 0; case as well!

P = 1/(m*s^2 + b*s + k);

%% PID design

figure;
bode(P);
legend;

Kp = 10;
Ki = 5;
Kd = 1;
Tf = 0.01;

C = pid(Kp,Ki,Kd,Tf)

%% plot nyquist

Gol = P*C;

figure;
NyquistOption = nyquistoptions;
NyquistOption.ShowFullContour = 'off'; 
NyquistOption.Xlim = [-3 1];
NyquistOption.Ylim = [-2 2];
nyquist(Gol,NyquistOption);

figure; margin(Gol)
%% Plot Step

Gcl = feedback(P*C,1);

figure;
step(Gcl);
StepInformation = stepinfo(Gcl)

sim('sim_cart.slx');
