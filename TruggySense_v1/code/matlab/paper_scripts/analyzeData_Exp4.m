clc;
clear;
imgTrack = imread('track_picture.jpeg');
%json_string_fast = fileread("llc_data_14.json" ); %crash
%json_string_fast = fileread("D300425_T12575890_llc_data_23.json");
json_string_fast = fileread("D300425_T13012060_llc_data_24.json");

data_fast = jsondecode(json_string_fast);

positionLat = [data_fast.latitude];
positionLong = [data_fast.longitude];

velocityCar = [ data_fast.velocity ];

orientationYaw = [ data_fast.yaw ];
orientationPitch = [ data_fast.pitch ];
orientationRoll = [ data_fast.roll ];

timestamp = [ data_fast.timestamp ]/1000;
  
idxValid = find( positionLat > 4 & positionLong > 4 );

velocityCarOK = velocityCar(idxValid);
orientationYawOK = orientationYaw( idxValid );
orientationPitchOK= orientationPitch( idxValid );
orientationRollOK = orientationRoll( idxValid );

timestampOK = timestamp(idxValid);

figure; plot(orientationPitchOK(1:end), LineWidth=2.5);
title("Crash");
xlabel("sample point");
ylabel('yaw angle (°)');