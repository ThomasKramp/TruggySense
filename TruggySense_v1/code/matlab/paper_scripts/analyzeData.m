clc;
clear;
imgTrack = imread('track_picture.jpeg');
json_string = fileread("llc_data_2.json" );
% json_string = fileread("llc_data_14.json" );
data = jsondecode(json_string);

positionLat = [data.latitude];
positionLong = [data.longitude];
velocityCar = [ data.velocity ];

wheelRPM1 = [data.rpm_wheel_1];
wheelRPM2 = [data.rpm_wheel_2 ];
wheelRPM3 = [data.rpm_wheel_3 ];
wheelRPM4 = [data.rpm_wheel_4 ];

orientationYaw = [ data.yaw ];
orientationPitch = [ data.pitch ];
orientationRoll = [ data.roll ];

timestamp = [ data.timestamp ]/1000;

    
%% 
idxValid = find( positionLat > 4 & positionLong > 4 );


wheelSpeeds = [ wheelRPM1(idxValid);wheelRPM2(idxValid);wheelRPM3(idxValid);wheelRPM4(idxValid) ]';

positionLongOK = smooth(positionLong(idxValid));
positionLatOK = smooth(positionLat(idxValid));
velocityCarOK = velocityCar(idxValid);
orientationYawOK = orientationYaw( idxValid );
orientationPitchOK= orientationPitch( idxValid );
orientationRollOK = orientationRoll( idxValid );

timestampOK = timestamp(idxValid);

%% Calculate velocity from locations:
% source: https://stackoverflow.com/questions/47028071/calculating-speed-from-set-of-longitude-and-latitudes-values-obtained-in-one-min

R = 6378000;
latRad = deg2rad(positionLatOK);  % times 2pi/360
lonRad = deg2rad(positionLongOK); % times 2pi/360
% dLat = diff(latRad);
% dLon = diff(lonRad);
% a = sin(dLat/2).^2 + cos(latRad(1:end-1)) .* cos(latRad(2:end)) .* sin(dLon/2).^2;
% c = 2 * atan2(sqrt(a), sqrt(1 - a));
% displacementMeters = [ 0 ; R * c  ]; 
%vx = ( (lonRad(2:end) - lonRad(1:(end-1))) .* R ) ./ (timestampOK(2:end) - timestampOK(1:(end-1)))

timestampOK(2:end) - timestampOK(1:(end-1));
vx = deg2rad(positionLongOK(2:end) - positionLongOK(1:(end-1))).*R ./ 0.1;
vy = deg2rad(positionLatOK(2:end) - positionLatOK(1:(end-1))).*R ./ 0.1;

v = sqrt(vx.^2 + vy.^2);

% figure();
% hold on;
% plot((timestampOK(1:(end-1))-timestampOK(1)), velocityCarOK(1:end-1));
% plot((timestampOK(1:(end-1))-timestampOK(1)), v);
% legend('gps determined velocity', 'calculated velocity');
% title('GPS determined velocity VS lat & lon velocity');
% xlabel('timestamp (s)');
% ylabel('velicity (m/s)');
% hold off;

%%
meanWheelSpeed = mean( wheelSpeeds' )';

% figure; plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, meanWheelSpeed )
% figure; plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, displacementMeters )

%% 
meanWheelspeedFront =  mean( wheelSpeeds(:,[1 4])' )';
meanWheelspeedBack =  mean( wheelSpeeds(:,[2 3])' )';

meanWheelspeedLeft =  mean( wheelSpeeds(:,[1 2])' )';
meanWheelspeedRight =  mean( wheelSpeeds(:,[3 4])' )';

meanWheelspeedLeftNorm = meanWheelspeedLeft - mean(meanWheelspeedLeft);
meanWheelspeedRightNorm = meanWheelspeedRight - mean(meanWheelspeedRight);

ratioFrontBack = meanWheelspeedFront ./ meanWheelspeedBack;
ratioLeftRight = meanWheelspeedLeftNorm ./ meanWheelspeedRightNorm;

diffLeftRight = meanWheelspeedLeft - meanWheelspeedRight;
diffLeftRight = (diffLeftRight./max(diffLeftRight))*200-100;

diffFrontBack = meanWheelspeedFront - meanWheelspeedBack;
% diffFrontBack = diffFrontBack+abs(min(diffFrontBack));
% diffFrontBack = diffFrontBack./max(diffFrontBack);
% diffFrontBack = diffFrontBack*200-100;
figure; plot(diffFrontBack);

rangeWheelSpeeds = range( wheelSpeeds' );
%% 
% x = 0:1:length(timestampOK)-1; %let it start from 0
% v = orientationPitchOK;
% xq = 0:0.01:length(timestampOK);
% figure
% vq1 = interp1(x,v,xq,'spline');
% stem(x,v,':.');
% %plot(xq,vq1,':.');
% title('Linear Interpolated Pitch')
% figure;
% plot(timestampOK, interp(1:0.001:timestampOK(end)-timestamp(1), orientationPitchOK,1:0.0001:timestampOK(end)-timestamp(1)));



% Experiment 1 GPS track comparison.
% plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, 0)
% title('Exp 1: "Slow Driving", GPS overlay of three laps')
% legend("Path between gps coordinate points")




% figure;
% plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, orientationPitchOK )
% title('Pitch Angle During a Crash');

% figure;
% plotImageOnTrack( imgTrack, positionLatOK(2:end), positionLongOK(2:end), v )
% title('Offline calculated speed on the track');

% 
figure;
plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, diffFrontBack )
title( 'Wheel Speed difference, Front vs Back')
% 
% figure; plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, diffLeftRight )
% 
% figure;
% plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, rangeWheelSpeeds )
% title( 'Wheel Speed')
% 
% figure;
% plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, orientationPitchOK )
% title( 'Pitch Angle (°)')
% 
% figure;
% plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, [ 0 diff(unwrap(deg2rad(orientationYawOK)))] )
% 
% figure;
% plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, orientationRollOK )
% 
% 

