clc;
clear;
close all;
imgTrack = imread('track_layout.png');
%json_string = fileread("llc_data_0.json" ); %Normal driving (or fast)
json_string = fileread("llc_data_2.json" ); %Normal driving (or fast)
%json_string = fileread("llc_data_2.json" ); %slow driving
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

carServoCurrent = [data.i_ss]; % data.i_etc

carServoTemperature = [data.t_ss];
carESCTemperature = [data.t_esc_1];
carBMTemperature = [data.t_bm_1];
carBPTemperature = [ data.t_bp ];

timestamp = [ data.timestamp ]/1000;

    
idxValid = find( positionLat > 4 & positionLong > 4 );


wheelSpeedsOK = [ wheelRPM1(idxValid);wheelRPM2(idxValid);wheelRPM3(idxValid);wheelRPM4(idxValid) ]';

positionLongOK = smooth(positionLong(idxValid));
positionLatOK = smooth(positionLat(idxValid));
velocityCarOK = velocityCar(idxValid);
orientationYawOK = orientationYaw( idxValid );
orientationPitchOK= orientationPitch( idxValid );
orientationRollOK = orientationRoll( idxValid );

carServoCurrentOK = carServoCurrent(idxValid);

carServoTemperatureOK = carServoTemperature(idxValid);
carESCTemperatureOK = carESCTemperature(idxValid);
carBMTemperatureOK = carBMTemperature(idxValid);
carBPTemperatureOK = carBPTemperature(idxValid);

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

figure();
hold on;
plot((timestampOK(1:(end-1))-timestampOK(1)), velocityCarOK(1:end-1));
plot((timestampOK(1:(end-1))-timestampOK(1)), v);
legend('gps determined velocity', 'calculated velocity');
%title('GPS determined velocity VS lat & lon velocity');
xlabel('timestamp (s)');
ylabel('velicity (m/s)');
hold off;
 %%
close all;
% x = 0:1:length(timestampOK)-1; %let it start from 0
% v = orientationPitchOK;
% xq = 0:0.01:length(timestampOK);
% figure
% vq1 = interp1(x,v,xq,'spline');
% stem(x,v,':.');
% %plot(xq,vq1,':.');
% title('Linear Interpolated Pitch')

% Experiment 1 wheel speed differences
rangeWheelSpeeds = smooth(range( wheelSpeedsOK' ));
rangeWheelSpeeds = rangeWheelSpeeds+abs(min(rangeWheelSpeeds));
rangeWheelSpeeds = rangeWheelSpeeds./max(rangeWheelSpeeds);
rangeWheelSpeeds = rangeWheelSpeeds-0.5;
%rangeWheelSpeeds = rangeWheelSpeeds*mean(rangeWheelSpeeds)*2-mean(rangeWheelSpeeds);
%%
figure; plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, rangeWheelSpeeds )
%title('Exp 1: "Slow Driving", Difference in overall wheel speed');
%%
% Experiment 1 GPS track comparison.

plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, 0)
% title('Exp 1: "Slow Driving", GPS overlay of three laps')
legend("Path between gps coordinate points")

% Experiment 1 GPS track in combination with pitch angel
figure;
plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, orientationPitchOK)
%title('Exp 1: "Slow Driving", pitch angle of three laps');

% Some fancy 3d scatter
% X = positionLongOK(1:int32(end/3));
% Y = positionLatOK(1:int32(end/3));
% Z = orientationPitchOK(1:int32(end/3)).*-1;
% x = [X(:); X(:); X(:)];
% y = [Y(:); Y(:); Y(:)];
% z = [Z(:); Z(:); Z(:)];
% s = scatter3(x,y,z, 100,z,'filled','MarkerEdgeColor','k');
% xlabel('longitude (°)');
% ylabel('latitude (°)');
% zlabel('pitch angle (°)');
% colormap(jet);
% colorbar;

%%
% Experiment 1 Temperature ESC, servo, Motor, and battery pack
figure;
hold on;
%title('Exp 1: "Slow Driving", temperature in function of time')
plot(timestampOK, carServoTemperatureOK, LineWidth=2.5);
plot(timestampOK, carESCTemperatureOK,LineWidth=2.5);
plot(timestampOK, carBMTemperatureOK,LineWidth=2.5);
plot(timestampOK, carBPTemperatureOK,LineWidth=2.5);
legend('t_{ss}', 't_{esc1}', 't_{m1}', 't_{bp}');
xlabel('time (s)');
ylabel('temperature (°C)');
grid on;
hold off;