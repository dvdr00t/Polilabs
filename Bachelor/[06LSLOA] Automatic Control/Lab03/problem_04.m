%% PROBLEM 04
% Plotting transfer functions
% ------------------------------------------------------------------------
clear all
close all
clc

%% GIVEN DATA
% ------------------------------------------------------------------------
s = tf('s');
H1 = 5 / s^3;
H2 = 0.25 / (s^2 * (1 - 0.5*s)^2);
H3 = (s-1) / (s*(s^2 - 9));
H4 = (1 + 0.5*s) / ((1 + s) * (1 - s)^2);
H5 = (s^2 + 1) / ((s^2 - 4) * (s + 4));

%% PLOTTING DATA
% ------------------------------------------------------------------------
figure(1)
bode(H5, 'r')
title('Bode diagram')
grid on

figure(2)
nyquist(H5, 'r')
title('Nyquist diagram')
grid on

figure(3)
nichols(H5, 'r')
title('Nichols diagram')
grid on

