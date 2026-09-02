clc;
clear;
close all;


%% Load data:
imgTrack = imread('track_images/track_layout.png');
json_string_dogb = fileread("data/v1/llc_data_12.json");
json_string_slow = fileread("data/v1/llc_data_2.json");
data_dogb = jsondecode(json_string_dogb); % Dogbone removed
data_slow = jsondecode(json_string_slow);

positionLat  = [data_dogb.latitude];
positionLong = [data_dogb.longitude];

positionLat_slow  = [data_slow.latitude];
positionLong_slow = [data_slow.longitude];

wheelRPM1 = [data_dogb.rpm_wheel_1];
wheelRPM2 = [data_dogb.rpm_wheel_2];
wheelRPM3 = [data_dogb.rpm_wheel_3];
wheelRPM4 = [data_dogb.rpm_wheel_4];

wheelRPM1s = [data_slow.rpm_wheel_1];
wheelRPM2s = [data_slow.rpm_wheel_2];
wheelRPM3s = [data_slow.rpm_wheel_3];
wheelRPM4s = [data_slow.rpm_wheel_4];


%% Check data
idxValid = find( positionLat > 4 & positionLong > 4 );
idxValid_slow = find( positionLat_slow > 4 & positionLong_slow > 4 );

positionLat = smooth(positionLat(idxValid));
positionLong = smooth(positionLong(idxValid));

positionLat_slow = smooth(positionLat_slow(idxValid_slow));
positionLong_slow = smooth(positionLong_slow(idxValid_slow));

wheelRPM1 = wheelRPM1(idxValid);
wheelRPM2 = wheelRPM2(idxValid);
wheelRPM3 = wheelRPM3(idxValid);
wheelRPM4 = wheelRPM4(idxValid);

wheelRPM1s = wheelRPM1s(idxValid_slow);
wheelRPM2s = wheelRPM2s(idxValid_slow);
wheelRPM3s = wheelRPM3s(idxValid_slow);
wheelRPM4s = wheelRPM4s(idxValid_slow);

wheelDiff    = wheelRPM1 - wheelRPM2;
wheelDiff    = wheelDiff(200:end);
positionLat  = positionLat(200:end);
positionLong = positionLong(200:end);

wheelDiffs = wheelRPM1s - wheelRPM2s;

f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
plotImageOnTrack(imgTrack, positionLat, positionLong, wheelDiff, "Wheel speed difference (RPM)")
% title('Exp3: "Physical Failure", dogbone removed and rear differential locked')


f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
plotImageOnTrack(imgTrack, positionLat_slow, positionLong_slow, wheelDiffs, "Wheel speed difference (RPM)")
% title('Exp3: "Physical Failure", slow driving experiment reference')