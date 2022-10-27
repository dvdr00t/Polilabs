%% PROBLEM 01
% (1) Compute the time response of the control input u(t) when r(t) is a unit
% step and the other inputs are set to zero
% ---------------------------------------------------------------------------
clear all
close all
clc

%% GIVEN DATA
% ---------------------------------------------------------------------------
s = tf('s');
tol = 1e-3;
G = 1 / (s+1)^2;
C = (s+1)^2 / (s * (1 + s/4));

%% SOLVING (1)
% From the theory: u(s) = R(s)r(s) + S(s)d_a(s) - R(s)d_y(s)
%   where d_a(s) and d_y(s) are set to zero by the exercise
% By computing u(s) as the product between r(s) = 1/s (step input) and R(s)
% it is possible to apply the Partial Fraction Expansion (PFE) to the
% result in order to get back to the time domain.
%
% NOTE: residue(num, den) finds the residues, poles and direct term of
%   a partial fraction expansion of the ratio of two polynomials B(s)/A(s).
%   If some pole has a multiplicity larger than 1, then the PFE includes
%   terms like 
%                R(j)        R(j+1)                R(j+m-1)
%              -------- + ------------   + ... + ------------
%              s - P(j)   (s - P(j))^2           (s - P(j))^m
%
%   And therefore the first pole that appears in the result vector is the
%   pole associated the denominator of smallest degree.
% ---------------------------------------------------------------------------
r = 1/s;
R = zpk(minreal(C/(1+G*C), tol));
u = r*R;            % In the Laplace domain

[num, den] = tfdata(u, 'v');
[res, pol] = residue(num, den); % Back in the time domain

%% SHOWING RESULTS
% ---------------------------------------------------------------------------
disp('Poles are:');
disp(pol);
disp('Residues are:');
disp(res);


%% SOLVING (2)
% From the theory: e(s) = S(s)r(s) - Q(s)d_a(s) - S(s)d_y(s)
%   where d_a(s) and r(s) are set to zero by the exercise
% By computing e(s) as the product between d_y(s) = L[0.5sin(t)](s) (sinusoidal input)
% and S(s) it is possible to apply the Partial Fraction Expansion (PFE) to the
% result in order to get back to the time domain.
% The PFE is: 
%
%                R(j)      -0.32      -0.4
%              -------- + ------- + ---------
%              s^2 + 1     (s+2)     (s+2)^2
%
%        Complex conjugate  
%
% Which leads to:
%
%  e(t) = 2*THETA*e^(SIGMA*t)*cos(OMEGA*t + PSI) - 0.32e^(-2t) - 0.4te(-2t)     
%                                                   (--> 0)       (--> 0)
% where:
% THETA = |R| (R is the residue which is complex conjugate)
% PSI = angle(R)
% SIGMA = real(P) (P is the pole which is complex conjugate)
% OMEGA = imag(P)
% ---------------------------------------------------------------------------
d_y = 0.5*(1/(1 + s^2));
S = zpk(minreal(1/(1+G*C), tol));
e = zpk(minreal(d_y*S, tol));

[num_e, den_e] = tfdata(e, 'v');
[res_e, pol_e] = residue(num_e, den_e);

%% SHOWING RESULTS
% ---------------------------------------------------------------------------
disp('Poles are:');
disp(pol);
disp('Residues are:');
disp(res);
disp('2*THETA is: ');
disp(2*abs(res_e(3)));
disp('PSI (shifted to the sine) is: ');
disp(angle(res_e(3)) - pi/2);
disp('SIGMA is: ');
disp(real(pol_e(3)));
disp('OMEGA is: ');
disp(imag(pol_e(3)));


%% SOLVING (3)
% From the theory: y(s) = T(s)r(s) + Q(s)d_a(s) + S(s)d_y(s)
%   where d_a(s) is set to zero by the exercise
% By computing y(s) as the product between d_y(t) = 2 (step output disturbance)
% and S(s) and between r(t) = 3 (step input reference) and T(s) it is possible 
% to apply the Partial Fraction Expansion (PFE) to the result in order to get back to the time domain.
% The PFE is: 
%
%               3      -1         -2
%              --- + ------- + ---------
%               s     (s+2)     (s+2)^2 
%
% Which leads to:
%
%  y(t) = 3 - e^(-2t) - 2te(-2t)     
%             (--> 0)    (--> 0)
% ---------------------------------------------------------------------------
r = 3/s;
d_y = 2/s;
T = zpk(minreal((G*C)/(1 + G*C), tol));
y = zpk(minreal(T*r + S*d_y, tol));

[num_y, den_y] = tfdata(y, 'v');
[res_y, pol_y] = residue(num_y, den_y);

