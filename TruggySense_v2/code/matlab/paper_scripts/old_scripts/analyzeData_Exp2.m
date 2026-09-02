clc;
clear;
% close all;
imgTrack = imread('track_layout.png');
json_string_fast = fileread("llc_data_16.json" ); %Normal driving (or fast)
json_string_slow = fileread("llc_data_2.json" ); %slow driving
data_fast = jsondecode(json_string_fast);
data_slow = jsondecode(json_string_slow);

positionLat = [data_fast.latitude];
positionLong = [data_fast.longitude];

positionLat_slow = [data_slow.latitude];
positionLong_slow = [data_slow.longitude];

velocityCar = [ data_fast.velocity ];

wheelRPM1 = [data_fast.rpm_wheel_1];
wheelRPM2 = [data_fast.rpm_wheel_2 ];
wheelRPM3 = [data_fast.rpm_wheel_3 ];
wheelRPM4 = [data_fast.rpm_wheel_4 ];

orientationYaw = [ data_fast.yaw ];
orientationPitch = [ data_fast.pitch ];
orientationRoll = [ data_fast.roll ];

carServoCurrent = [data_fast.i_ss]; % data.i_etc

carServoTemperature = [data_fast.t_ss];
carESCTemperature = [data_fast.t_esc_1];
carBMTemperature = [data_fast.t_bm_1];
carBPTemperature = [ data_fast.t_bp ];

carServoTemperature_slow = [data_slow.t_ss];
carESCTemperature_slow = [data_slow.t_esc_1];
carBMTemperature_slow = [data_slow.t_bm_1];
carBPTemperature_slow = [ data_slow.t_bp ];

timestamp = [ data_fast.timestamp ]/1000;
timestamp_slow = [ data_slow.timestamp ]/1000;

    
idxValid = find( positionLat > 4 & positionLong > 4 );
idxValid_slow = find( positionLat_slow > 4 & positionLong_slow > 4 );


wheelSpeedsOK = [ wheelRPM1(idxValid);wheelRPM2(idxValid);wheelRPM3(idxValid);wheelRPM4(idxValid) ]';

positionLongOK = smooth(positionLong(idxValid));
positionLatOK = smooth(positionLat(idxValid));
velocityCarOK = velocityCar(idxValid);
orientationYawOK = orientationYaw( idxValid );
orientationPitchOK= orientationPitch( idxValid );
orientationRollOK = orientationRoll( idxValid );

carServoCurrentOK = carServoCurrent(idxValid);

carServoTemperatureOK_slow = carServoTemperature_slow(idxValid_slow);
carESCTemperatureOK_slow = carESCTemperature_slow(idxValid_slow);
carBMTemperatureOK_slow = carBMTemperature_slow(idxValid_slow);
carBPTemperatureOK_slow = carBPTemperature_slow(idxValid_slow);

carServoTemperatureOK = carServoTemperature(idxValid);
carESCTemperatureOK = carESCTemperature(idxValid);
carBMTemperatureOK = carBMTemperature(idxValid);
carBPTemperatureOK = carBPTemperature(idxValid);

timestampOK = timestamp(idxValid);
timestampOK_slow = timestamp_slow(idxValid_slow);

% Experiment 1 wheel speed differences
rangeWheelSpeeds = range( wheelSpeedsOK' );
rangeWheelSpeeds = rangeWheelSpeeds+abs(min(rangeWheelSpeeds));
rangeWheelSpeeds = rangeWheelSpeeds./max(rangeWheelSpeeds);
rangeWheelSpeeds = rangeWheelSpeeds*200-100;

meanWheelspeedFront =  mean( wheelSpeedsOK(:,[1 4])' )';
meanWheelspeedBack =  mean( wheelSpeedsOK(:,[2 3])' )';

meanWheelspeedLeft =  mean( wheelSpeedsOK(:,[1 2])' )';
meanWheelspeedRight =  mean( wheelSpeedsOK(:,[3 4])' )';

meanWheelspeedLeftNorm = meanWheelspeedLeft - mean(meanWheelspeedLeft);
meanWheelspeedRightNorm = meanWheelspeedRight - mean(meanWheelspeedRight);

ratioFrontBack = meanWheelspeedFront ./ meanWheelspeedBack;
ratioLeftRight = meanWheelspeedLeftNorm ./ meanWheelspeedRightNorm;

diffLeftRight = meanWheelspeedLeft - meanWheelspeedRight;
diffLeftRight = diffLeftRight./max(diffLeftRight);

diffFrontBack = meanWheelspeedFront - meanWheelspeedBack;

diffFrontBack = diffFrontBack+abs(min(diffFrontBack));
diffFrontBack = diffFrontBack./max(diffFrontBack);
diffFrontBack = diffFrontBack*200-100;

% Experiment 2 wheel speed difference front and back
figure; plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, diffFrontBack )
% title('Exp 2: "Fast Driving", Difference in front and rear wheel speed normalized between -100 and 100');


% Experiment 2 Temperature ESC, battery and motor
figure;
hold on;
maximal_length = min([length(carESCTemperatureOK) length(carESCTemperatureOK_slow)]);
plot(carESCTemperatureOK(1:maximal_length) - carESCTemperatureOK_slow(1:maximal_length), LineWidth=2.5);
plot(carBMTemperatureOK(1:maximal_length) - carBMTemperatureOK_slow(1:maximal_length), LineWidth=2.5);
plot(carBPTemperatureOK(1:maximal_length) - carBPTemperatureOK_slow(1:maximal_length), LineWidth=2.5);
legend('t_{esc1} - t_{esc1_s}', 't_{m1} - t_{m1_s}', 't_{bp}-t_{bp_s}');
xlabel('sample');
ylabel('temperature (°C)');
% title('Exp 2: "Fast Driving", Temperature difference between fast and slow track')
grid on;
hold off;
