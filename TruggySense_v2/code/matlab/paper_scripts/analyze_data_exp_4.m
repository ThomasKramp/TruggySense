clc;
clear;
close all;


%% Load data:
imgTrack = imread('track_images/track_layout_2.png');
%json_string_crash_1 = fileread("data/v1/llc_data_14.json");
%json_string_crash_2 = fileread("data/v1/llc_data_23.json");
%json_string_crash_3 = fileread("data/v1/llc_data_24.json");
json_string_crash_1 = fileread("data/v2/llc_data_83.json");
json_string_crash_2 = fileread("data/v2/llc_data_83.json");
json_string_crash_3 = fileread("data/v2/llc_data_84.json");


%% Get data crash 1:
data = jsondecode(json_string_crash_1);
positionLat      = [data.latitude];
positionLong     = [data.longitude];
orientationRoll  = [data.pitch];
orientationPitch = [data.roll];
orientationYaw   = [data.yaw];
timestamp        = [data.timestamp]/1000;

idxValid = find( positionLat > 4 & positionLong > 4 );
orientationPitchOK_crash_1 = orientationPitch(idxValid);
timestampOK_crash_1 = timestamp(idxValid);

% Track reference
% positionLongOK = smooth(positionLong(idxValid));
% positionLatOK  = smooth(positionLat(idxValid));
% f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
% plotImageOnTrack(imgTrack, positionLatOK, positionLongOK, 0, "")
% dcm = datacursormode(gcf);
% set(dcm, 'UpdateFcn', @(src,event) myupdatefcn(event, positionLatOK, positionLongOK));


%% Get data crash 2:
data = jsondecode(json_string_crash_2);
positionLat      = [data.latitude];
positionLong     = [data.longitude];
orientationRoll  = [data.pitch];
orientationPitch = [data.roll];
orientationYaw   = [data.yaw];
timestamp        = [data.timestamp]/1000;

idxValid = find( positionLat > 4 & positionLong > 4 );
orientationPitchOK_crash_2 = orientationPitch(idxValid);
timestampOK_crash_2 = timestamp(idxValid);

% Track reference
% positionLongOK = smooth(positionLong(idxValid));
% positionLatOK  = smooth(positionLat(idxValid));
% f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
% plotImageOnTrack(imgTrack, positionLatOK, positionLongOK, 0, "")
% dcm = datacursormode(gcf);
% set(dcm, 'UpdateFcn', @(src,event) myupdatefcn(event, positionLatOK, positionLongOK));

%% Get data crash 3:
data = jsondecode(json_string_crash_3);
positionLat      = [data.latitude];
positionLong     = [data.longitude];
orientationRoll  = [data.pitch];
orientationPitch = [data.roll];
orientationYaw   = [data.yaw];
timestamp        = [data.timestamp]/1000;

idxValid = find( positionLat > 4 & positionLong > 4 );
temp = orientationRoll(idxValid);  orientationRollOK_crash_3  =  (mod(temp - temp(1) + 180, 360) - 180);
temp = orientationPitch(idxValid); orientationPitchOK_crash_3 = -(mod(temp - temp(1) + 180, 360) - 180);
temp = orientationYaw(idxValid);   orientationYawOK_crash_3   =  (mod(temp - temp(1) + 180, 360) - 180);
timestampOK_crash_3 = timestamp(idxValid);

% TracSk reference
positionLongOK = smooth(positionLong(idxValid));
positionLatOK  = smooth(positionLat(idxValid));
f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
plotImageOnTrack(imgTrack, positionLatOK(1:1300), positionLongOK(1:1300), 0, "")
dcm = datacursormode(gcf);
set(dcm, 'UpdateFcn', @(src,event) myupdatefcn(event, positionLatOK(1:1300), positionLongOK(1:1300)));

%% Plot full graphs:
f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
% 
% subplot(3, 1, 1);
% plot(orientationPitchOK_crash_1(1:end), LineWidth=2.5);
% title("Crash 1"); xlabel("sample point"); ylabel('pitch angle (°)');

% subplot(3, 1, 2);
% plot(orientationPitchOK_crash_2(1:end), LineWidth=2.5);
% title("Crash 2"); xlabel("sample point"); ylabel('pitch angle (°)');
% 
% subplot(3, 1, 3);
hold on
plot(orientationRollOK_crash_3(1:1300), LineWidth=2.5);
plot(orientationPitchOK_crash_3(1:1300), LineWidth=2.5);
plot(orientationYawOK_crash_3(1:1300), LineWidth=2.5);
hold off
title("Crash 3"); xlabel("sample point"); ylabel('pitch angle (°)');
l = legend('roll', 'pitch', 'yaw', 'Orientation', 'horizontal', 'FontSize', 14);

% Let MATLAB size it naturally, then centre it
drawnow;  % forces MATLAB to compute the legend size first
l.Position(1) = 0.5 - l.Position(3)/2;  % centre horizontally
l.Position(2) = 0.92;                    % top of figure


% % %% Plot crash data:
% f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
% 
% pointsOfInterest = 1100:1200-1;
% subplot(1, 3, 1);
% plot(orientationPitchOK_crash_1(pointsOfInterest), LineWidth=2.5);
% title("Crash 1"); xlabel("sample point"); ylabel('pitch angle (°)');
% 
% pointsOfInterest = 1620:1720-1;
% subplot(1, 3, 2);
% plot(orientationPitchOK_crash_2(pointsOfInterest), LineWidth=2.5);
% title("Crash 2"); xlabel("sample point"); ylabel('pitch angle (°)');
% 
% pointsOfInterest = 1100:1200-1;
% subplot(1, 3, 3);
% plot(orientationPitchOK_crash_3(pointsOfInterest), LineWidth=2.5);
% title("Crash 3"); xlabel("sample point"); ylabel('pitch angle (°)');