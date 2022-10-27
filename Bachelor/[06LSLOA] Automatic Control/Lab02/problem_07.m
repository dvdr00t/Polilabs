% PROBLEM 02
% Computing equilibrium input corresponding to a particular equilibrium
% state of a given pendulum system and study its stability
% ------------------------------------------------------------------------
clear all
close all
clc

% Data given by the problem exercise
% ------------------------------------------------------------------------
m = 0.1;    % Mass of the pendulum
l = 0.5;    % Length of the pendulum
b = 0.1;    % Hinge friction coefficient
g = 9.81;   % Gravity coefficient
x_eq = [pi/2; 0];   % Equilibrium state
x0 = x_eq;  % Initial conditions

% By-hand computations of the equilibrium input leads to the following
% result
% ------------------------------------------------------------------------
u_eq = m*g*l;

% By-hand computation of the linearized model leads to the following result
% ------------------------------------------------------------------------
A = [0 1; 0 -b/(m*l^2)];
B = [0; 1/(m*l^2)];
e = eig(A);

% Showing results
% ------------------------------------------------------------------------
fprintf(1, 'The equilibrium input is u_eq = mgl = %f\n', u_eq);
disp('The eigenvalues of the linearized model are: ');
disp(e);
disp('Therefore, since not every eigenvalue has a strictly negative real part, no conclusion can be drawn on the stability of the system.');
