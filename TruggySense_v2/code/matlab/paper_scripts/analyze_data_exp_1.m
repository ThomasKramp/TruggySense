clc;
clear;
close all;



%% Load data:
% Reference data:
imgTrack_ref = imread('track_images/track_layout_1.png');
data_ref = jsondecode(fileread("data/v1/llc_data_2.json"));

% Base data:
imgTrack = imread('track_images/track_layout_2.png');
data = jsondecode(fileread("data/v2/llc_data_60.json"));



%% Experiment 1: GPS track reference
% Reference data:
positionLat_ref = [data_ref.latitude];
positionLng_ref = [data_ref.longitude];

idxValid_ref = find(positionLat_ref > 4 & positionLng_ref > 4);

positionLngOK_ref = smooth(positionLng_ref(idxValid_ref));
positionLatOK_ref = smooth(positionLat_ref(idxValid_ref));

% f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
% plotImageOnTrack(imgTrack_ref, positionLatOK_ref, positionLngOK_ref, 0, "")
% l = legend("Path between gps coordinate points", 'Location', 'northoutside', 'FontSize', 16, 'Orientation', 'horizontal');

% Base data:
positionLat = [data.latitude];
positionLng = [data.longitude];

idxValid = find(positionLat > 4 & positionLng > 4);

positionLngOK = smooth(positionLng(idxValid));
positionLatOK = smooth(positionLat(idxValid));

f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
plotImageOnTrack(imgTrack, positionLatOK, positionLngOK, 0, "")
l = legend("Path between gps coordinate points", 'Location', 'northoutside', 'FontSize', 14, 'Orientation', 'horizontal');
l.Position(2) = 0.94;


%% Experiment 1: Wheel speed differences
normalize = @(x) (x + abs(min(x))) ./ max(x + abs(min(x)));

% Reference data:
wheelFR = [data_ref.rpm_wheel_4]; wheelFL = [data_ref.rpm_wheel_1]; wheelRR = [data_ref.rpm_wheel_3]; wheelRL = [data_ref.rpm_wheel_2];
wheelSpeedsOK = [ wheelFR(idxValid_ref); wheelFL(idxValid_ref); wheelRR(idxValid_ref); wheelRL(idxValid_ref) ]';
meanWheelspeedRight = mean(wheelSpeedsOK(:,[1 3])')';
meanWheelspeedLeft  = mean(wheelSpeedsOK(:,[2 4])')';

% rangeWheelSpeeds = normalize(smooth(range(wheelSpeedsOK')));
% rangeWheelSpeeds = dataClamp(rangeWheelSpeeds);
% rangeWheelSpeeds = normalize(rangeWheelSpeeds) - 0.5;

rangeWheelSpeeds = normalize(meanWheelspeedRight - meanWheelspeedLeft);
rangeWheelSpeeds = dataClamp(rangeWheelSpeeds);
rangeWheelSpeeds = normalize(rangeWheelSpeeds) - 0.5;

f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
plotImageOnTrack(imgTrack_ref, positionLatOK_ref, positionLngOK_ref, rangeWheelSpeeds, "Wheel speed difference normalized between -0.5 and 0.5")

% Base data:
wheelFR = [data.rpm_wheel_1]; wheelFL = [data.rpm_wheel_2]; wheelRR = [data.rpm_wheel_3]; wheelRL = [data.rpm_wheel_4];
wheelSpeedsOK = [ wheelFR(idxValid); wheelFL(idxValid); wheelRR(idxValid); wheelRL(idxValid) ]';
% meanWheelspeedRight = mean(wheelSpeedsOK(:,[1 3])')';
% meanWheelspeedLeft  = mean(wheelSpeedsOK(:,[2 4])')';

rangeWheelSpeeds = normalize(smooth(range(wheelSpeedsOK')));
rangeWheelSpeeds = dataClamp(rangeWheelSpeeds);
rangeWheelSpeeds = normalize(rangeWheelSpeeds) - 0.5;

% rangeWheelSpeeds = normalize(meanWheelspeedRight - meanWheelspeedLeft);
% rangeWheelSpeeds = dataClamp(rangeWheelSpeeds);
% rangeWheelSpeeds = normalize(rangeWheelSpeeds) - 0.5;

f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
plotImageOnTrack(imgTrack, positionLatOK, positionLngOK, rangeWheelSpeeds, "Wheel speed difference normalized between -0.5 and 0.5")



%% Experiment 1: Pitch angel
% Reference data:
orientationPitch = [data_ref.pitch];
temp = orientationPitch(idxValid_ref);
orientationPitchOK = temp; %(mod(temp - temp(1) + 180, 360) - 180);

% f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
% plotImageOnTrack(imgTrack_ref, positionLatOK_ref, positionLngOK_ref, orientationPitchOK, "Pitch angle (°)")

% Base data:
orientationPitch = [data.roll];    % Due to the orientation of the IMU, the roll and pitch are switched
temp = orientationPitch(idxValid);
orientationPitchOK = -temp; %-(mod(temp - temp(1) + 180, 360) - 180);

f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
plotImageOnTrack(imgTrack, positionLatOK, positionLngOK, orientationPitchOK, "Pitch angle (°)")



%% Experiment 1: Temperature of the ESC, servo, Motor, and battery pack
% Reference data
carESCTemperature_ref   = [data_ref.t_esc_1];   carESCTemperatureOK_ref   = carESCTemperature_ref(idxValid_ref);
carBMTemperature_ref    = [data_ref.t_bm_1];    carBMTemperatureOK_ref    = carBMTemperature_ref(idxValid_ref);
carBPTemperature_ref    = [data_ref.t_bp];      carBPTemperatureOK_ref    = carBPTemperature_ref(idxValid_ref);
carServoTemperature_ref = [data_ref.t_ss];      carServoTemperatureOK_ref = carServoTemperature_ref(idxValid_ref);

timestamp_ref = [data_ref.timestamp]/1000;      temp = timestamp_ref(idxValid_ref);   timestampOK_ref = temp - temp(1);

% Base data:
carESCTemperature   = [data.t_esc_1];   carESCTemperatureOK   = carESCTemperature(idxValid);
carBMTemperature    = [data.t_bm_1];    carBMTemperatureOK    = carBMTemperature(idxValid);
carBPTemperature    = [data.t_bp];      carBPTemperatureOK    = carBPTemperature(idxValid);
carServoTemperature = [data.t_ss];      carServoTemperatureOK = carServoTemperature(idxValid);

timestamp = [data.timestamp]/1000;      temp = timestamp(idxValid);   timestampOK = temp - temp(1);


f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";

% Left: v1 temperatures
ax1 = subplot(1,2,1);
hold on;
p1 = plot(timestampOK_ref, carESCTemperatureOK_ref,   LineWidth=2.5);
p2 = plot(timestampOK_ref, carBMTemperatureOK_ref,    LineWidth=2.5);
p3 = plot(timestampOK_ref, carBPTemperatureOK_ref,    LineWidth=2.5);
p4 = plot(timestampOK_ref, carServoTemperatureOK_ref, LineWidth=2.5);
xlabel('time (s)');
ylabel('temperature (°C)');
title('Version 1', FontSize=16);
grid on;
hold off;

% Right: v2 temperatures
ax2 = subplot(1,2,2);
hold on;
plot(timestampOK, carESCTemperatureOK,   LineWidth=2.5);
plot(timestampOK, carBMTemperatureOK,    LineWidth=2.5);
plot(timestampOK, carBPTemperatureOK,    LineWidth=2.5);
plot(timestampOK, carServoTemperatureOK, LineWidth=2.5);
xlabel('time (s)');
ylabel('temperature (°C)');
title('Version 2', FontSize=16);
grid on;
hold off;

% Shrink both subplots to leave room at the top for the legend
ax1.Position(2) = 0.08;
ax1.Position(4) = 0.78;
ax2.Position(2) = 0.08;
ax2.Position(4) = 0.78;

% Single shared legend centred at the top inside the figure
l = legend(ax1, [p1 p2 p3 p4], 't_{esc_{1}}', 't_{m_{1}}', 't_{bp}', 't_{ss}', ...
           'Orientation', 'horizontal', 'FontSize', 16);

% Let MATLAB size it naturally, then centre it
drawnow;  % forces MATLAB to compute the legend size first
l.Position(1) = 0.45; % centre horizontally
l.Position(2) = 0.92;                  % top of figure