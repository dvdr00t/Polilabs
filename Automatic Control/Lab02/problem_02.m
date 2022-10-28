% PROBLEM 02
% Perform modal analysis of an LTI system
% ------------------------------------------------------------------------
clear all
close all
clc

% Data given by the problem exercise
% ------------------------------------------------------------------------
A = [1 2; 4 3];
B = [5 8];
C = [-1 3];
D = 8;
tol = 1e-2;
s = tf('s');

% Computing eigenvalues of the system
% ------------------------------------------------------------------------
e = eig(A);

% Computing the output response in the Laplace domain
% NOTE: pay attention to the matrix dimension and take into account that
% matrices multiplication is not commutative!
% ------------------------------------------------------------------------
X = zpk(minreal(inv(s*eye(2)-A), tol));

% Y residue and poles after Partial Fraction Expansion
% ------------------------------------------------------------------------
[num_X11, den_X11] = tfdata(X(1, 1), 'v');
[num_X12, den_X12] = tfdata(X(1, 2), 'v');
[num_X21, den_X21] = tfdata(X(2, 1), 'v');
[num_X22, den_X22] = tfdata(X(2, 2), 'v');
[r_x11, p_x11] = residue(num_X11, den_X11);
[r_x12, p_x12] = residue(num_X12, den_X12);
[r_x21, p_x21] = residue(num_X21, den_X21);
[r_x22, p_x22] = residue(num_X22, den_X22);

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
psi_11 = angle(r_x11(1));   % input (1, 1)
two_theta_11 = 2*abs(r_x11(1));
sigma_11 = real(p_x11(1));
omega_11 = imag(p_x11(1));

psi_12 = angle(r_x12(1));   % input (1, 2)
two_theta_12 = 2*abs(r_x12(1));
sigma_12 = real(p_x12(1));
omega_12 = imag(p_x12(1));

psi_21 = angle(r_x21(1));   % input (2, 1)
two_theta_21 = 2*abs(r_x21(1));
sigma_21 = real(p_x21(1));
omega_21 = imag(p_x21(1));

psi_22 = angle(r_x22(1));   % input (2, 2)
two_theta_22 = 2*abs(r_x22(1));
sigma_22 = real(p_x22(1));
omega_22 = imag(p_x22(1));

% Showing results
% ------------------------------------------------------------------------
disp('The zero input state response is defined by the following values:');
disp('Magnitude (2*theta): ');
disp(two_theta_11);
disp(two_theta_12);
disp(two_theta_21);
disp(two_theta_22);
disp('Exponent coefficient (sigma): ');
disp(sigma_11);
disp(sigma_12);
disp(sigma_21);
disp(sigma_22);
disp('Time coefficient (omega): ');
disp(omega_11);
disp(omega_12);
disp(omega_21);
disp(omega_22);
disp('Phase shift: ');
disp(psi_11);
disp(psi_12);
disp(psi_21);
disp(psi_22);

disp('The eigenvalues of the system are: ');
disp(e);