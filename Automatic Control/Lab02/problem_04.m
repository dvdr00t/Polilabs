% PROBLEM 04
% Analyze internal stability and BIBO stability of the LTI system proposed
% ------------------------------------------------------------------------
clear all
close all
clc

% Data given by the problem exercise
% ------------------------------------------------------------------------
A = [-1 2; 1 0];
B = [2; 0];
C = [1/2 -1/2];
D = 0;
tol = 1e-2;
s = tf('s');

% Computing eigenvalues of the system
% ------------------------------------------------------------------------
e = eig(A);

% Computing the transfer function of the LTI system in the Laplace domain
% in order to verify that all the poles have real part less than zero
% ------------------------------------------------------------------------
sys = ss(A, B, C, D);
disp('The transfer functions is: ');
H = zpk(tf(sys))
[num_H, den_H] = tfdata(H, 'v');
[res_H, pol_H] = residue(num_H, den_H);

% Showing results
% ------------------------------------------------------------------------
disp('The eigenvalues of the system are: ');
disp(e);
disp('The system is internally unstable since it exists one eigenvalue whose real part is greater than zero.');
disp('The transfer functions residues are: ');
disp(res_H);
disp('And the transfer functions poles are: ');
disp(pol_H);
disp('The system is BIBO stable since the only pole of the transfer function is -2 (due to residue equal to zero that cancels out the pole equal to 1)');