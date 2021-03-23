% PROBLEM 01
% Given the LTI system compute y(t) and x(t)
% ------------------------------------------------------------------------

clear all
close all
clc

% Data given by the problem exercise
% ------------------------------------------------------------------------
s = tf('s');
U = 2/s;
x0 = [2; 2];
A = [1 2; 4 3];
B = [5; 8];
C = [-1 3];
D = 8;
tol = 1e-2;

% Computing state response and output response in the Laplace domain
% ------------------------------------------------------------------------
X = zpk(minreal(inv(s*eye(2)-A)*(B*U+x0),tol));
Y = zpk(minreal(((C*inv(s*eye(2)-A))*(x0+U*B))+U*D, tol));

% X1 residues and poles after Partial Fraction Expansion
% ------------------------------------------------------------------------
[num_X1, den_X1] = tfdata(X(1), 'v');
[r1, p1] = residue(num_X1, den_X1)

% X2 residues and poles after Partial Fraction Expansion
% ------------------------------------------------------------------------
[num_X2, den_X2] = tfdata(X(2), 'v');
[r2, p2] = residue(num_X2, den_X2)

% Y residue and poles after Partial Fraction Expansion
% ------------------------------------------------------------------------
[num_Y, den_Y] = tfdata(Y, 'v');
[ry, py] = residue(num_Y, den_Y)

