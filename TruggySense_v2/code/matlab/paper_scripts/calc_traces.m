clc;
clear;
close all;

%% Constants

k = 0.048;          % External PCB layer constant
dT = 60;            % Temperature rise [°C]
t_mm = 0.035;       % Copper thickness [mm]
t_mil = 1.378;      % Copper thickness [mil]
I = [0.1, 2, 200];  % Current values [A]

%% Track width calculation

% Equation: W = (1/H) * ( I / (k * dT^0.44) )^(1/0.725)

W_mil = (1 ./ t_mil) .* ...
    (I ./ (k .* dT.^0.44)).^(1/0.725);
W_mm = W_mil * 0.0254; % Convert mil to mm

%% Via diameter calculation

% Equation: D = t + (1/(pi*t)) * ( I / (k*dT^0.44) )^(1/0.725)

D_mil = t_mil + ...
    (1 ./ (pi .* t_mil)) .* ...
    (I ./ (k .* dT.^0.44)).^(1/0.725);

D_mm = D_mil * 0.0254; % Convert mil to mm