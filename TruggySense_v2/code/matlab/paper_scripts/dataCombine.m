clc;
clear;
close all;


%% Load data:
imgTrack = imread('track_images/track_layout_2.png');
files = { ...
    "data/data_slow_1/llc_data_50.json",  ...
    "data/data_slow_1/llc_data_51.json",  ...
    "data/data_slow_1/llc_data_52.json",  ...
    "data/data_slow_1/llc_data_53.json",  ...
    "data/data_slow_1/llc_data_54.json"  ...
};


% Load and append in a loop
data = [];
for i = 1:numel(files)
    json_string = fileread(files{i});
    data = [data; jsondecode(json_string)];
end

positionLat  = [data.latitude];
positionLong = [data.longitude];


%% Check data
idxValid = find( positionLat > 4 & positionLong > 4 );
idxValid = idxValid(250:end);

positionLongOK = smooth(positionLong(idxValid));
positionLatOK  = smooth(positionLat(idxValid));


%% Display data
f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
plotImageOnTrack(imgTrack, positionLatOK, positionLongOK, 0, "")
% title('Exp 1: "Slow Driving", GPS overlay of three laps')
l = legend("Path between gps coordinate points", 'Location', 'northoutside', 'FontSize', 14, 'Orientation', 'horizontal');
% l.Position(2) = l.Position(2) + 0.05;
% Shrink axes height to make room, then reposition colorbar below
ax = gca; axPos = ax.Position;
ax.Position = [axPos(1), axPos(2), axPos(3), axPos(4)-0.08];

% Place colorbar just below the axes with a small gap
l.Position(2) = ax.Position(2) + ax.Position(4) + 0.01;

%% Combine data
json_out = jsonencode(data);
fid = fopen("data/v2/llc_data_0.json", 'w');
fprintf(fid, '%s', json_out);
fclose(fid);