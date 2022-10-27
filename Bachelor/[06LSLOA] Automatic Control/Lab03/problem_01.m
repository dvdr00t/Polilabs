%% PROBLEM 01
% Given the LTI system described by the given Transfer Function, compute
% the steady state output response in the presence of the given input and
% compute the maximum amplitude of a given sinusoidal input such that the
% maximum output amplitude is smaller than 1
% ------------------------------------------------------------------------
clear all
close all
clc


%% GIVEN DATA
% ------------------------------------------------------------------------
s = tf('s');
H = 1/(s^3 + 2*s^2 + 5.25*s + 4.25);

w0 = 0.1;       % Input frequency
u_step = 2;     % Input step amplitude
u_sinu = 3;     % Input sinusoidal amplitude
t_start = 0;
t_end = (1/w0)*16;
t = linspace(t_start, t_end, 100000);
u = @(x) u_sinu*sin(w0*x) + u_step;  % Input of the system


%% CHECK IF THE SYSTEM IS STABLE
% First: evaluate the stability by means of the eig() function
% Second: confirm the stability by means of the isstable() boolean function
% ------------------------------------------------------------------------
sys = ss(H);            % Computing the system object
eigs = eig(sys.A);      % Computing the system eigenvalues
for i = 1:length(eigs)
    if real(eigs(i)) > 0
        disp('The system is not stable!');
        return
    end
end

% Alternative verification
if ~isstable(sys)
    disp('The system is not stable!');
    return
end


%% DIRECTLY COMPUTING THE STEADY STATE OUTPUT RESPONSE
% The steady state output response of a given system can be directly
% computed through the function "lsim()" which requires as argument the
% system object
% ---------------------------------------------------------------------------------
y_dir = lsim(sys, u(t), t);


%% INDERICTLY COMPUTING THE STEADY STATE OUTPUT RESPONSE
% Computing amplitude and phase by using the BODE function
% NOTE: the magnitude value is NOT expressed in dB and the phase value in
% expressed in DEGREES
% ---------------------------------------------------------------------------------
[mgn, phs] = bode(H, w0);
K = dcgain(H);
y_indir = @(x) (u_sinu*mgn)*sin(w0*x+phs) + K*u_step;


%% COMPUTING THE MAXIMUM AMPLITUDE OF A SINUSOIDAL INPUT SUCH THAT Y_out < 1
% Since the output amplitude is given by the sinusoidal input amplitude
% times the value obtained through the bode() function
% ------------------------------------------------------------------------
w0_1 = 3;                           % Sinusoidal input frequency
[mgn_1, phs_1] = bode(H, w0_1);     
A_max = 1/mgn_1;

%% SHOWING RESULTS
% ------------------------------------------------------------------------
figure(1)   
plot(t, u(t), 'r')
title('Input signal to the system')
xlabel('Time [t]')
ylabel('Input signal u(t)')
grid on

figure(2)
plot(t, y_dir, 'r')
title('Steady State Output Response computed directly through lsim()')
xlabel('Time [t]')
ylabel('Steady State Output Response y_dir(t)')
grid on

figure(3)
plot(t, y_indir(t), 'r')
title('Steady State Output Response computed indirectly through bode() and dcgain()')
xlabel('Time [t]')
ylabel('Steady State Output Response y_indir(t)')
grid on

disp('The maximum amplitude of a sinusoidal input that satifies the condition |yss(t)| < 1 is ');
disp(A_max)

