% PROBLEM 03
% Given the LTI system compute the exponential matrix e^(At)
% ------------------------------------------------------------------------

clear all
close all
clc

% Data given by the problem exercise
% ------------------------------------------------------------------------
s = tf('s');
A = [0 6; -1 -5];
B = [0; 1];

% HINT: the exponential matrix e^(At) is defined as the inverse Laplace
% transform of the inverse matrix (sI-A)^(-1)
% ------------------------------------------------------------------------
inv_mat = inv(s*eye(2)-A);

% Expliciting the numerator and denominator components
% ------------------------------------------------------------------------
inv_mat_explicit = zpk(inv_mat)

% Computing every component of the exponential matrix
% ------------------------------------------------------------------------
[num_11, den_11] = tfdata(inv_mat_explicit(1, 1), 'v');
[num_12, den_12] = tfdata(inv_mat_explicit(1, 2), 'v');
[num_21, den_21] = tfdata(inv_mat_explicit(2, 1), 'v');
[num_22, den_22] = tfdata(inv_mat_explicit(2, 2), 'v');
[res_11, pol_11] = residue(num_11, den_11);
[res_12, pol_12] = residue(num_12, den_12);
[res_21, pol_21] = residue(num_21, den_21);
[res_22, pol_22] = residue(num_22, den_22);