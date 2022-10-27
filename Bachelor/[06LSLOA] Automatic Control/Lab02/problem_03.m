% PROBLEM 03
% Perform modal analysis of an LTI system
% ------------------------------------------------------------------------
clear all
close all
clc

% Data given by the problem exercise
% ------------------------------------------------------------------------
A = [-2 0 0; 0 0 1; 0 0 0];
tol = 1e-2;
s = tf('s');

% Computing the output response in the Laplace domain
% NOTE: pay attention to the matrix dimension and take into account that
% matrices multiplication is not commutative!
% ------------------------------------------------------------------------
X = zpk(minreal(inv(s*eye(3)-A), tol))

% Computing eigenvalues of the system
% ------------------------------------------------------------------------
e = eig(A);

% Showing results
% ------------------------------------------------------------------------
disp('The eigenvalues of the system are: ');
disp(e);