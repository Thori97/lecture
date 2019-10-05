clear; close all; clc;
load('data_frd_x');
bop = bodeoptions('cstprefs');
bop.PhaseWrappingBranch = -360;
bop.PhaseWrapping = 'on';
%%
mn = 4.2;
bn = 80;
kn = 2500;
s = tf('s');
Pmdl = 1/(mn*s^2 + bn*s + kn);

figure; bode(Pmsr,Pmdl,bop); legend;
xlim([1,1000]); grid;
