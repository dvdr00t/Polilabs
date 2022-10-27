% PROBLEM 02
% Computing equilibrium states corresponding to a particular equilibrium
% input of a given nonlinear dynamical system and study its stability
% ------------------------------------------------------------------------
clear all
close all
clc

% The solving equation for equilibrium computation can be trivially solved
% graphically through the following plot
% ------------------------------------------------------------------------
x1_start = -10;
x1_end = 10;
x1 = linspace(x1_start, x1_end, 10000);
f = @(x) -19.65*sin(x) - 4*x;
y = f(x1);

figure(1);
plot(x1, y, 'r', 'linewidth', 2)
grid on

x_eq1 = [fzero(f, -4); 0];
x_eq2 = [fzero(f, 0); 0];
x_eq3 = [fzero(f, 4); 0];

% Evaluating the stability of the equilibrium states of the linearized
% model through the Ai matrices. A1 corresponds to the linearized model of
% the equilibrium state x_eq1, and so on...
% ------------------------------------------------------------------------
A1 = [0 1; -19.62+cos(x_eq1(1)) -4];
A2 = [0 1; -19.62+cos(x_eq2(1)) -4];
A3 = [0 1; -19.62+cos(x_eq3(1)) -4];

% Computing the eigenvalues of the linearized system and study its
% stability
% ------------------------------------------------------------------------
e1 = eig(A1);
e2 = eig(A2);
e3 = eig(A3);

% Showing results
% ------------------------------------------------------------------------
disp('The equilibrium states of the system are: ')
disp(x_eq1);
disp(x_eq2);
disp(x_eq3);

disp('The eigevalues of the linearized system in the equilibrium state x_eq1 are:');
disp(e1);
disp('The eigevalues of the linearized system in the equilibrium state x_eq2 are:');
disp(e2);
disp('The eigevalues of the linearized system in the equilibrium state x_eq3 are:');
disp(e3);
disp('All the system are strable since all the eigenvalues have strictly negative real part.');