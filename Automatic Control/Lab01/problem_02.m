% PROBLEM 02
% Given the LTI system compute y(t)
% ------------------------------------------------------------------------

clear all
close all
clc

% Data given by the problem exercise
% ------------------------------------------------------------------------
s = tf('s');
U = [0; 2];
x0 = [0; 0];
A = [0 1; -1 -1];
B = [4 0; 10 1];
C = [1 0];
D = [0 0];
tol = 1e-3;

% Computing the output response in the Laplace domain
% NOTE: pay attention to the matrix dimension and take into account that
% matrices multiplication is not commutative!
% ------------------------------------------------------------------------
Y = zpk(minreal(((C*inv(s*eye(2)-A))*(x0+B*U))+D*U, tol));

% Y residue and poles after Partial Fraction Expansion
% ------------------------------------------------------------------------
[num_Y, den_Y] = tfdata(Y, 'v');
[ry, py] = residue(num_Y, den_Y);

% Compute magnitude and phase
% NOTE: when poles are complex conjugate numbers, the Euler's formula can
% be used to simplify the final result. 
% In particular: 2*THETA * e^(SIGMA*t) * cos(OMEGA*t + PSI)*ramp_function(t)
% where:
% THETA = |R| (R is the residue which is complex conjugate)
% PSI = angle(R)
% SIGMA = real(P) (P is the pole which is complex conjugate)
% OMEGA = imag(P)
% ------------------------------------------------------------------------
theta = abs(ry(1));
two_theta = 2*theta
psi = angle(ry(1))
sigma = real(py(1))
omega = imag(py(1))
