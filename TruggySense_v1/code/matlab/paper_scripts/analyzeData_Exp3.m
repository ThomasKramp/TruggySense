clc;
clear;
imgTrack = imread('track_layout.jpg');
json_string_fast = fileread("llc_data_12.json" ); %dogbone removed
json_string_slow = fileread("llc_data_2.json" ); % slow driving
data_fast = jsondecode(json_string_fast);
data_slow = jsondecode(json_string_slow);

positionLat = [data_fast.latitude];
positionLong = [data_fast.longitude];

pla_s = [data_slow.latitude];
plo_s = [data_slow.longitude];

velocityCar = [ data_fast.velocity ];

wheelRPM1 = [data_fast.rpm_wheel_1];
wheelRPM2 = [data_fast.rpm_wheel_2 ];
wheelRPM3 = [data_fast.rpm_wheel_3];
wheelRPM4 = [data_fast.rpm_wheel_4 ];

wheelRPM1s = [data_slow.rpm_wheel_1];
wheelRPM2s = [data_slow.rpm_wheel_2 ];
wheelRPM3s = [data_slow.rpm_wheel_3];
wheelRPM4s = [data_slow.rpm_wheel_4 ];

idxValid = find( positionLat > 4 & positionLong > 4 );
idxValid_slow = find( pla_s > 4 & plo_s > 4 );

positionLat = smooth(positionLat(idxValid));
positionLong = smooth(positionLong(idxValid));

pla_s = smooth(pla_s(idxValid_slow));
plo_s = smooth(plo_s(idxValid_slow));

% wheelRPM1 = wheelRPM1(idxValid);
% wheelRPM2 = wheelRPM2(idxValid);
% 
% wheelRPM1s = wheelRPM1s(idxValid_slow);
% wheelRPM2s = wheelRPM2s(idxValid_slow);

wheelRPM1 = mean([wheelRPM1(idxValid); wheelRPM4(idxValid)]);
wheelRPM2 = mean([wheelRPM2(idxValid); wheelRPM3(idxValid)]);

wheelRPM1s = mean([wheelRPM1s(idxValid_slow); wheelRPM4s(idxValid_slow)]);
wheelRPM2s = mean([wheelRPM2s(idxValid_slow); wheelRPM3s(idxValid_slow)]);

wheelDiff = wheelRPM1 - wheelRPM2;
wheelDiff = wheelDiff(200:end);
positionLat = positionLat(200:end);
positionLong = positionLong(200:end);
% wheelDiffMean = mean(wheelDiff);
% wheelDiff = wheelDiff+ abs(min(wheelDiff));
% wheelDiff = wheelDiff/max(wheelDiff);
% wheelDiff = wheelDiff - 0.5;
% wheelDiff = smooth(wheelDiff);




wheelDiffs = wheelRPM1s - wheelRPM2s;
% wheelDiffMean = mean(wheelDiff);
% wheelDiffs = wheelDiffs+ abs(min(wheelDiffs));
% wheelDiffs = wheelDiffs/max(wheelDiffs);
% wheelDiffs = wheelDiffs - 0.5;
% wheelDiffs = smooth(wheelDiffs);


%find highest and lowest values
%this to scale both in the same way

% minV = min([wheelDiff, wheelDiffs]);
% wheelDiff = wheelDiff + abs(minV);
% wheelDiffs = wheelDiffs + abs(minV);
% center_point = abs(minV);
% center_point_s =  abs(minV);

% center_point = abs(min(wheelDiff));
% center_point_s =  abs(min(wheelDiffs));
% wheelDiff = wheelDiff + abs(min(wheelDiff));
% wheelDiffs = wheelDiffs + abs(min(wheelDiffs));

% maxV = max([wheelDiff, wheelDiffs]);
% wheelDiff = wheelDiff/maxV;
% wheelDiffs = wheelDiffs/maxV;
% center_point = center_point/maxV;
% center_point_s = center_point_s/maxV;

% center_point = center_point/max(wheelDiff);
% center_point_s = center_point_s/max(wheelDiffs);
% wheelDiff = wheelDiff/max(wheelDiff);
% wheelDiffs = wheelDiffs/max(wheelDiffs);

% wheelDiff = smooth(wheelDiff - center_point);
% wheelDiffs = smooth(wheelDiffs - center_point_s);


% close all;
% figure;
% hold on;
% plot(wheelDiff);
% plot(wheelDiffs);
% legend('fast', 'slow');
% hold off;

% wheelDiff = wheelDiff*100;
% wheelDiffs = wheelDiffs*100;
%     size(positionLat)
%     size(positionLong)
%     size(wheelDiff)

figure;

plotImageOnTrack( imgTrack, positionLat, positionLong, wheelDiff )
title('Exp3: "Physical Failure", dogbone removed and rear differential locked')


figure;
plotImageOnTrack( imgTrack, pla_s, plo_s, wheelDiffs )
title('Exp3: "Physical Failure", slow driving experiment reference')