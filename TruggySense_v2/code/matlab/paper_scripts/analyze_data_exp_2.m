clc;
clear;
close all;


%% Load data:
% Reference data:
imgTrack_ref = imread('track_images/track_layout_1.png');
data_fast_ref = jsondecode(fileread("data/v1/llc_data_16.json"));
data_slow_ref = jsondecode(fileread("data/v1/llc_data_2.json"));

% Base data:
imgTrack = imread('track_images/track_layout_2.png');
data_fast = jsondecode(fileread("data/v2/llc_data_56.json"));
data_slow = jsondecode(fileread("data/v2/llc_data_60.json"));

% Reference data:
positionLat_fast_ref = [data_fast_ref.latitude];   positionLng_fast_ref = [data_fast_ref.longitude];
positionLat_slow_ref = [data_slow_ref.latitude];   positionLng_slow_ref = [data_slow_ref.longitude];

idxValid_fast_ref = find(positionLat_fast_ref > 4 & positionLng_fast_ref > 4);
idxValid_slow_ref = find(positionLat_slow_ref > 4 & positionLng_slow_ref > 4);

positionLngOK_ref = smooth(positionLng_fast_ref(idxValid_fast_ref));
positionLatOK_ref = smooth(positionLat_fast_ref(idxValid_fast_ref));

% Base data:
positionLat_fast = [data_fast.latitude];   positionLng_fast = [data_fast.longitude];
positionLat_slow = [data_slow.latitude];   positionLng_slow = [data_slow.longitude];

idxValid_fast = find( positionLat_fast > 4 & positionLng_fast > 4 );
idxValid_slow = find( positionLat_slow > 4 & positionLng_slow > 4 );

positionLngOK = smooth(positionLng_fast(idxValid_fast));
positionLatOK = smooth(positionLat_fast(idxValid_fast));



% %% Experiment 2: Wheel speed differences
% normalize = @(x) (x + abs(min(x))) ./ max(x + abs(min(x)));
% 
% % Reference data:
% wheelFR = [data_fast_ref.rpm_wheel_4]; wheelFL = [data_fast_ref.rpm_wheel_1]; wheelRR = [data_fast_ref.rpm_wheel_3]; wheelRL = [data_fast_ref.rpm_wheel_2];
% wheelSpeedsOK_ref = [ wheelFR(idxValid_fast_ref); wheelFL(idxValid_fast_ref); wheelRR(idxValid_fast_ref); wheelRL(idxValid_fast_ref) ]';
% meanWheelspeedFront = mean(wheelSpeedsOK_ref(:,[1 2])')';
% meanWheelspeedBack  = mean(wheelSpeedsOK_ref(:,[3 4])')';
% 
% diffFrontBack = normalize(meanWheelspeedFront - meanWheelspeedBack);
% diffFrontBack = dataClamp(diffFrontBack);
% diffFrontBack = normalize(diffFrontBack) - 0.5;
% 
% % f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
% % plotImageOnTrack(imgTrack_ref, positionLatOK_ref, positionLngOK_ref, diffFrontBack, "Wheel speed difference normalized between -0.5 and 0.5")
% 
% % Base data:
% wheelFR = [data_fast.rpm_wheel_1]; wheelFL = [data_fast.rpm_wheel_2]; wheelRR = [data_fast.rpm_wheel_3]; wheelRL = [data_fast.rpm_wheel_4];
% wheelSpeedsOK = [ wheelFR(idxValid_fast); wheelFL(idxValid_fast); wheelRR(idxValid_fast); wheelRL(idxValid_fast) ]';
% meanWheelspeedFront = mean(wheelSpeedsOK(:,[1 2])')';
% meanWheelspeedBack  = mean(wheelSpeedsOK(:,[3 4])')';
% 
% diffFrontBack = normalize(meanWheelspeedFront - meanWheelspeedBack);
% diffFrontBack = dataClamp(diffFrontBack);
% diffFrontBack = normalize(diffFrontBack) - 0.5;
% 
% f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
% plotImageOnTrack(imgTrack, positionLatOK, positionLngOK, diffFrontBack, "Wheel speed difference normalized between -0.5 and 0.5")



%% Experiment 2: Temperature of the ESC, Motor, and battery pack
% Reference data
carESCTemperature_fast_ref = [data_fast_ref.t_esc_1];   carESCTemperatureOK_fast_ref = carESCTemperature_fast_ref(idxValid_fast);
carBMTemperature_fast_ref  = [data_fast_ref.t_bm_1];    carBMTemperatureOK_fast_ref  = carBMTemperature_fast_ref(idxValid_fast);
carBPTemperature_fast_ref  = [data_fast_ref.t_bp];      carBPTemperatureOK_fast_ref  = carBPTemperature_fast_ref(idxValid_fast);

carESCTemperature_slow_ref = [data_slow_ref.t_esc_1];   carESCTemperatureOK_slow_ref = carESCTemperature_slow_ref(idxValid_slow);
carBMTemperature_slow_ref  = [data_slow_ref.t_bm_1];    carBMTemperatureOK_slow_ref  = carBMTemperature_slow_ref(idxValid_slow);
carBPTemperature_slow_ref  = [data_slow_ref.t_bp];      carBPTemperatureOK_slow_ref  = carBPTemperature_slow_ref(idxValid_slow);

timestamp_fast_ref = [data_fast_ref.timestamp]/1000;   temp = timestamp_fast_ref(idxValid_fast);   timestampOK_fast_ref = temp - temp(1);
timestamp_slow_ref = [data_slow_ref.timestamp]/1000;   temp = timestamp_slow_ref(idxValid_slow);   timestampOK_slow_ref = temp - temp(1);

% Base data:
carESCTemperature_fast = [data_fast.t_esc_1];   carESCTemperatureOK_fast = carESCTemperature_fast(idxValid_fast);
carBMTemperature_fast  = [data_fast.t_bm_1];    carBMTemperatureOK_fast  = carBMTemperature_fast(idxValid_fast);
carBPTemperature_fast  = [data_fast.t_bp];      carBPTemperatureOK_fast  = carBPTemperature_fast(idxValid_fast);

carESCTemperature_slow = [data_slow.t_esc_1];   carESCTemperatureOK_slow = carESCTemperature_slow(idxValid_slow);
carBMTemperature_slow  = [data_slow.t_bm_1];    carBMTemperatureOK_slow  = carBMTemperature_slow(idxValid_slow);
carBPTemperature_slow  = [data_slow.t_bp];      carBPTemperatureOK_slow  = carBPTemperature_slow(idxValid_slow);

timestamp_fast = [data_fast.timestamp]/1000;    temp = timestamp_fast(idxValid_fast);   timestampOK_fast = temp - temp(1);
timestamp_slow = [data_slow.timestamp]/1000;    temp = timestamp_slow(idxValid_slow);   timestampOK_slow = temp - temp(1);


f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";

% Left: v1 temperatures
ax1 = subplot(1,2,1);
hold on;
maximal_length = min([length(carESCTemperatureOK_fast_ref) length(carESCTemperatureOK_slow_ref)]);
p1 = plot(carESCTemperatureOK_fast_ref(1:maximal_length) - carESCTemperatureOK_slow_ref(1:maximal_length), LineWidth=2.5);
p2 = plot(carBMTemperatureOK_fast_ref(1:maximal_length)  - carBMTemperatureOK_slow_ref(1:maximal_length),  LineWidth=2.5);
p3 = plot(carBPTemperatureOK_fast_ref(1:maximal_length)  - carBPTemperatureOK_slow_ref(1:maximal_length),  LineWidth=2.5);
xlabel('time (s)');
ylabel('temperature (°C)');
title('Version 1', FontSize=16);
grid on;
hold off;

% Right: v2 temperatures
ax2 = subplot(1,2,2);
hold on;
maximal_length = min([length(carESCTemperatureOK_fast) length(carESCTemperatureOK_slow)]);
p1 = plot(carESCTemperatureOK_fast(1:maximal_length) - carESCTemperatureOK_slow(1:maximal_length), LineWidth=2.5);
p2 = plot(carBMTemperatureOK_fast(1:maximal_length)  - carBMTemperatureOK_slow(1:maximal_length),  LineWidth=2.5);
p3 = plot(carBPTemperatureOK_fast(1:maximal_length)  - carBPTemperatureOK_slow(1:maximal_length),  LineWidth=2.5);
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

l = legend(ax1, [p1 p2 p3], ...
           't^{f}_{esc_{1}} - t^{s}_{esc_{1}}', 't^{f}_{m_{1}} - t^{s}_{m_{1}}', 't^{f}_{bp}-t^{s}_{bp}', ...
           'Location', 'northoutside', 'FontSize', 16, 'Orientation', 'horizontal');
drawnow;  % forces MATLAB to compute the legend size first
l.Position(1) = 0.45;  % centre horizontally
l.Position(2) = 0.92;


%% Experiment 2: Distribution of fast and slow steer PWM value
n = 33;

% Reference data
actSS_fast_ref = [data_fast_ref.ch_4];   actSSOK_fast_ref = actSS_fast_ref(idxValid_fast);
actSS_slow_ref = [data_slow_ref.ch_4];   actSSOK_slow_ref = actSS_slow_ref(idxValid_slow);

orientationYaw_fast_ref = [data_fast_ref.yaw];   orientationYawOK_fast_ref = orientationYaw_fast_ref(idxValid_fast);
orientationYaw_slow_ref = [data_slow_ref.yaw];   orientationYawOK_slow_ref = orientationYaw_slow_ref(idxValid_slow);

pointStart_fast = 1050;   pointsOfInterest_fast = pointStart_fast:pointStart_fast+n;
pointStart_slow = 1550;   pointsOfInterest_slow = pointStart_slow:pointStart_slow+n;

actSSOK_fast_ref = actSSOK_fast_ref(pointsOfInterest_fast);
actSSOK_slow_ref = actSSOK_slow_ref(pointsOfInterest_slow);

orientationYawOK_fast_ref = orientationYawOK_fast_ref(pointsOfInterest_fast);
orientationYawOK_fast_ref = orientationYawOK_fast_ref - min(orientationYawOK_fast_ref);
orientationYawOK_fast_ref = orientationYawOK_fast_ref - mean(orientationYawOK_fast_ref);

orientationYawOK_slow_ref = orientationYawOK_slow_ref(pointsOfInterest_slow);
orientationYawOK_slow_ref = orientationYawOK_slow_ref - min(orientationYawOK_slow_ref);
orientationYawOK_slow_ref = orientationYawOK_slow_ref - mean(orientationYawOK_slow_ref);

timestamp_fast_ref = [data_fast_ref.timestamp]/1000;   temp = timestamp_fast_ref(idxValid_fast);   timestampOK_fast_ref = temp - temp(1);
timestamp_slow_ref = [data_slow_ref.timestamp]/1000;   temp = timestamp_slow_ref(idxValid_slow);   timestampOK_slow_ref = temp - temp(1);
timestampOK_fast_ref = timestampOK_fast_ref(pointsOfInterest_fast);
timestampOK_slow_ref = timestampOK_slow_ref(pointsOfInterest_slow);

% Base data:
actSS_fast = [data_fast.ch_2];   actSSOK_fast = actSS_fast(idxValid_fast);
actSS_slow = [data_slow.ch_2];   actSSOK_slow = actSS_slow(idxValid_slow);

orientationYaw_fast = [data_fast.yaw];   orientationYawOK_fast = orientationYaw_fast(idxValid_fast);
orientationYaw_slow = [data_slow.yaw];   orientationYawOK_slow = orientationYaw_slow(idxValid_slow);

pointStart_fast = 680;   pointsOfInterest_fast = pointStart_fast:pointStart_fast+n;
pointStart_slow = 740;   pointsOfInterest_slow = pointStart_slow:pointStart_slow+n;

actSSOK_fast = actSSOK_fast(pointsOfInterest_fast);
actSSOK_slow = actSSOK_slow(pointsOfInterest_slow);

orientationYawOK_fast = orientationYawOK_fast(pointsOfInterest_fast);
orientationYawOK_fast = orientationYawOK_fast - min(orientationYawOK_fast);
orientationYawOK_fast = orientationYawOK_fast - mean(orientationYawOK_fast);

orientationYawOK_slow = orientationYawOK_slow(pointsOfInterest_slow);
orientationYawOK_slow = orientationYawOK_slow - min(orientationYawOK_slow);
orientationYawOK_slow = orientationYawOK_slow - mean(orientationYawOK_slow);

timestamp_fast = [data_fast.timestamp]/1000;    temp = timestamp_fast(idxValid_fast);   timestampOK_fast = temp - temp(1);
timestamp_slow = [data_slow.timestamp]/1000;    temp = timestamp_slow(idxValid_slow);   timestampOK_slow = temp - temp(1);
timestampOK_fast = timestampOK_fast(pointsOfInterest_fast);
timestampOK_slow = timestampOK_slow(pointsOfInterest_slow);

f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";

hold on;
histogram(actSSOK_fast_ref(1:n), 10, BinWidth=5, Normalization="percentage");
histogram(actSSOK_slow_ref(1:n), 10, BinWidth=5, Normalization="percentage");
histogram(actSSOK_fast(1:n),10, BinWidth=5, Normalization="percentage");
histogram(actSSOK_slow(1:n),10, BinWidth=5, Normalization="percentage");
hold off;

ylabel('probability (%)')
ylabel('Pulse Width Duration (ms)');
l = legend('PWM^{f}_{ss - v1}', 'PWM^{s}_{ss - v1}', 'PWM^{f}_{ss - v2}', 'PWM^{s}_{ss - v2}', ...
    'Location', 'northoutside', 'FontSize', 14, 'Orientation', 'horizontal');
% Shrink axes height to make room, then reposition colorbar below
ax = gca; axPos = ax.Position;
ax.Position = [axPos(1), axPos(2), axPos(3), axPos(4)-0.08];

% Place colorbar just below the axes with a small gap
l.Position(2) = ax.Position(2) + ax.Position(4) + 0.05;
drawnow;


%% Experiment 2: Raw PWM values of SS with yaw angles between fast and slow experiment
f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";

% Left: v1
ax1 = subplot(1,2,1);
colororder(ax1, {'k','k'})

yyaxis(ax1, 'left')
hold(ax1, 'on')
p1 = plot(ax1, actSSOK_fast_ref(1:n), 'Color', 'r', LineWidth=2.5, LineStyle='-');
p2 = plot(ax1, actSSOK_slow_ref(1:n), 'Color', 'b', LineWidth=2.5, LineStyle='-');
ylabel(ax1, 'Pulse Width Duration (ms)');
hold(ax1, 'off')

yyaxis(ax1, 'right')
hold(ax1, 'on')
p3 = plot(ax1, orientationYawOK_fast_ref(1:n), 'Color', 'k', LineWidth=2.5, LineStyle=':');
p4 = plot(ax1, orientationYawOK_slow_ref(1:n), 'Color', 'k', LineWidth=2.5, LineStyle='--');
hold(ax1, 'off')

ylabel(ax1, 'Relative Yaw angle (°)');
xlabel(ax1, 'sample point')
title(ax1, 'Version 1', FontSize=16);
grid(ax1, 'on');

% Right: v2
ax2 = subplot(1,2,2);
colororder(ax2, {'k','k'})

yyaxis(ax2, 'left')
hold(ax2, 'on')
plot(ax2, actSSOK_fast(1:n), 'Color', 'r', LineWidth=2.5, LineStyle='-');
plot(ax2, actSSOK_slow(1:n), 'Color', 'b', LineWidth=2.5, LineStyle='-');
ylabel(ax2, 'Pulse Width Duration (ms)');
hold(ax2, 'off')

yyaxis(ax2, 'right')
hold(ax2, 'on')
plot(ax2, orientationYawOK_fast(1:n), 'Color', 'k', LineWidth=2.5, LineStyle=':');
plot(ax2, orientationYawOK_slow(1:n), 'Color', 'k', LineWidth=2.5, LineStyle='--');
hold(ax2, 'off')

ylabel(ax2, 'Relative Yaw angle (°)');
xlabel(ax2, 'sample point')
title(ax2, 'Version 2', FontSize=16);
grid(ax2, 'on');

% Shrink both subplots and add shared legend
ax1.Position(2) = 0.08; ax1.Position(4) = 0.78;
ax2.Position(2) = 0.08; ax2.Position(4) = 0.78;

l = legend(ax1, [p1 p2 p3 p4], ...
    'PWM^{f}_{SS}', 'PWM^{s}_{SS}', 'yaw^{f}', 'yaw^{s}', ...
    'Location', 'northoutside', 'FontSize', 16, 'Orientation', 'horizontal');
drawnow;

l.Position(1) = 0.5 - l.Position(3)/2 + 0.02;
l.Position(2) = 0.92;