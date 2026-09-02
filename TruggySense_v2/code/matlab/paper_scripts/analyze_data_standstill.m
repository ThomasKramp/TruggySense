clc;
clear;
close all;

%% Load data:
% Data standstill:
json_string = fileread("data/v2/llc_data_32.json");
data = jsondecode(json_string);

posLat_standstill = [data.latitude];
posLng_standstill = [data.longitude];

% Center the data around Campus Groenenborger, for privacy purposes.
lat_offset = posLat_standstill(1) - 51.1779524;  % 51.17° N → latitude
lng_offset = posLng_standstill(1) - 4.4179314;   % 4.41° E → longitude

posLat_standstill = posLat_standstill(:) + lat_offset;
posLng_standstill = posLng_standstill(:) + lng_offset;

ESC_standstill = [data.ch_3];
wheelSpeeds_standstill = [ data.rpm_wheel_1; data.rpm_wheel_2; data.rpm_wheel_3; data.rpm_wheel_4 ]';

IMU_standstill = [ data.roll; data.pitch; data.yaw ]';

% Data slow drive:
json_string = fileread("data/v2/llc_data_33.json");
data = jsondecode(json_string);

posLat_slow_drive = [data.latitude] + lat_offset;
posLng_slow_drive = [data.longitude] + lng_offset;

ESC_slow_drive = [data.ch_3];
wheelSpeeds_slow_drive = [ data.rpm_wheel_1; data.rpm_wheel_2; data.rpm_wheel_3; data.rpm_wheel_4 ]';

IMU_slow_drive = [ data.roll; data.pitch; data.yaw ]';

% Data fast drive:
json_string = fileread("data/v2/llc_data_34.json");
data = jsondecode(json_string);

posLat_fast_drive = [data.latitude] + lat_offset;
posLng_fast_drive = [data.longitude] + lng_offset;

ESC_fast_drive = [data.ch_3];
wheelSpeeds_fast_drive = [ data.rpm_wheel_1; data.rpm_wheel_2; data.rpm_wheel_3; data.rpm_wheel_4 ]';

IMU_fast_drive = [ data.roll; data.pitch; data.yaw ]';

%% Show data GPS:
% GPS distance:
datasets = {'Inactive', 'Slow Drive', 'Fast Drive'};
lats = {posLat_standstill, posLat_slow_drive, posLat_fast_drive};
lngs = {posLng_standstill, posLng_slow_drive, posLng_fast_drive};

% Precompute centroids:
centLat = cellfun(@mean, lats);
centLng = cellfun(@mean, lngs);

for d = 1:3
    lat = lats{d}(:);
    lng = lngs{d}(:);
    n = numel(lat);
    maxDist = 0;
    for i = 1:n
        % Haversine distance in meters
        dlat = deg2rad(lat(i) - centLat(d));
        dlng = deg2rad(lng(i) - centLng(d));
        a = sin(dlat/2)^2 + cos(deg2rad(centLat(d))) * cos(deg2rad(lat(i))) * sin(dlng/2)^2;
        dist = 6371000 * 2 * atan2(sqrt(a), sqrt(1-a));
        if dist > maxDist
            maxDist = dist;
        end
    end

    % Centroid distance vs standstill centroid:
    dlat = deg2rad(centLat(d) - centLat(1));
    dlng = deg2rad(centLng(d) - centLng(1));
    a = sin(dlat/2)^2 + cos(deg2rad(centLat(1))) * cos(deg2rad(centLat(d))) * sin(dlng/2)^2;
    centDist = 6371000 * 2 * atan2(sqrt(a), sqrt(1-a));

    fprintf('Max dist from centroid [%s]: %.2f m | Centroid offset from Inactive: %.2f m\n', ...
        datasets{d}, maxDist, centDist);
end

% Scatter plot - GPS locations:
f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
hold on;
% Scatter plots:
scatter(posLng_standstill, posLat_standstill, 20, 'r', 'filled', 'DisplayName', 'Inactive');
scatter(posLng_slow_drive, posLat_slow_drive, 20, 'b', 'filled', 'DisplayName', 'Slow Drive');
scatter(posLng_fast_drive, posLat_fast_drive, 20, 'k', 'filled', 'DisplayName', 'Fast Drive');
% Centroids:
scatter(mean(posLng_standstill), mean(posLat_standstill), 100, 'r', 'filled', 'Marker', 'pentagram', 'HandleVisibility', 'off');
scatter(mean(posLng_slow_drive), mean(posLat_slow_drive), 100, 'b', 'filled', 'Marker', 'pentagram', 'HandleVisibility', 'off');
scatter(mean(posLng_fast_drive), mean(posLat_fast_drive), 100, 'k', 'filled', 'Marker', 'pentagram', 'HandleVisibility', 'off');
hold off;
xlabel('Longitude (°)');
ylabel('Latitude (°)');
title('GPS Location Data');
grid on;
axis equal;
l = legend('Inactive', 'Slow Drive', 'Fast Drive', ...
    'Location', 'northoutside', 'FontSize', 16, 'Orientation', 'horizontal');
ax = gca; axPos = ax.Position;
ax.Position = [axPos(1), axPos(2), axPos(3), axPos(4)-0.08];
l.Position(2) = ax.Position(2) + ax.Position(4) + 0.05;
drawnow;

%% Wheel speed:
% Boxplot - Wheel speeds:
f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
datasets = {'Inactive', 'Slow Drive', 'Fast Drive'};
speeds = {wheelSpeeds_standstill, wheelSpeeds_slow_drive, wheelSpeeds_fast_drive};

left  = 0.08;
width = 0.88;
gap   = 0.05;
totalHeight = 0.82;
unitH = totalHeight / 5;
heights = [unitH, unitH*2, unitH*2];
bottoms(3) = 0.06;
bottoms(2) = bottoms(3) + heights(3) + gap;
bottoms(1) = bottoms(2) + heights(2) + gap;

for i = 1:3
    % Wheel speeds boxplot (left)
    ax_left = axes('Position', [left, bottoms(i), width*0.75, heights(i)]);
    boxplot(speeds{i}, 'Labels', {'FR wheel', 'FL wheel', 'RR wheel', 'RL wheel'});
    set(findobj(ax_left, 'Type', 'Line'), 'LineWidth', 2);
    ylabel('Wheel Speed (RPM)');
    title(datasets{i}, ...
        'FontSize', 16);
    grid on;

    % Combined boxplot (right)
    ax_right = axes('Position', [left + width*0.75 + 0.06, bottoms(i), width*0.17, heights(i)]);
    combined = speeds{i}(:);
    boxplot(combined, 'Labels', {'Combined'});
    set(findobj(ax_right, 'Type', 'Line'), 'LineWidth', 2);
    grid on;
end
drawnow;

% Notes:
% - Blue box (= IQR):       Contains 50% of the data points. Those ranging
%                           between 25% and 75%.
% - Red line (= median):    The 50% point / the center point of the data.
%                           It shows to what side the data is skewed.
% - Black lines:            Show all data within the range of 1.5 * IRQ.
% - Orange crosses:         Outliers.

% % Histogram - Wheel speeds:
% f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
% speeds = {wheelSpeeds_standstill, wheelSpeeds_slow_drive, wheelSpeeds_fast_drive};
% wheelColors = {'r', 'b', 'g', 'm'};
% 
% % Layout parameters
% left  = 0.08;
% width = 0.88;
% gap   = 0.05;
% totalHeight = 0.72;  % Leave room at top for legend + sgtitle
% unitH = totalHeight / 5;
% heights = [unitH, unitH*2, unitH*2];
% bottoms(3) = 0.06;
% bottoms(2) = bottoms(3) + heights(3) + gap;
% bottoms(1) = bottoms(2) + heights(2) + gap;
% 
% for i = 1:3
%     ax = axes('Position', [left, bottoms(i), width, heights(i)]);
%     hold on;
%     for w = 1:4
%         histogram(speeds{i}(:, w), 'FaceColor', wheelColors{w}, ...
%             'FaceAlpha', 0.5, 'DisplayName', sprintf('Wheel %d', w));
%     end
%     hold off;
%     xlabel('Wheel Speed (RPM)');
%     ylabel('Count');
%     title(datasets{i});
%     grid on;
% end
% 
% % Single legend above all subplots, below sgtitle
% l = legend('FR wheel', 'FL wheel', 'RR wheel', 'RL wheel', ...
%     'FontSize', 14, 'Orientation', 'horizontal');
% l.Position = [0.5 - l.Position(3)/2, 0.88, l.Position(3), l.Position(4)];
% 
% annotation('textbox', [0, 0.95, 1, 0.05], ...
%     'HorizontalAlignment', 'center', 'VerticalAlignment', 'middle', ...
%     'FontSize', 16, 'FontWeight', 'bold', 'EdgeColor', 'none');
% drawnow;

%% IMU:
IMU_data   = {IMU_standstill, IMU_slow_drive, IMU_fast_drive};
datasets   = {'Inactive', 'Slow Drive', 'Fast Drive'};

IMU_labels = {'Roll', 'Pitch', 'Yaw'};
dataset_colors = {'r', 'b', 'y'};

% IMU Boxplots
f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";

for w = 1:3 % Roll / Pitch / Yaw
    subplot(3,1,w);
    hold on;
    h = gobjects(1,3);
    for i = 1:3 % datasets
        h(i) = histogram(IMU_data{i}(:,w), ...
            'FaceColor', dataset_colors{i}, ...
            'FaceAlpha', 0.5, ...
            'DisplayName', datasets{i});
    end
    hold off;
    xlabel('Orientation angle (°)');
    ylabel('Count');
    title(IMU_labels{w}, ...
        'FontSize', 16);
    grid on;
end
legend(h, datasets, ...
    'Orientation', 'horizontal', ...
    'Position', [0.45 0.96 0.15 0.03], ...
    'FontSize', 16);
drawnow;

% % IMU Histograms
% f = figure('Position', [0 0 2300 1150]); f.Theme.BaseColorStyle = "light";
% 
% for w = 1:3   % Roll / Pitch / Yaw
%     subplot(3,1,w);
%     hold on;
%     for i = 1:3   % datasets
%         histogram(IMU_data{i}(:,w), ...
%             'FaceColor', dataset_colors{i}, ...
%             'FaceAlpha', 0.5, ...
%             'DisplayName', datasets{i});
%     end
%     hold off;
%     xlabel('Orientation angle (°)');
%     ylabel('Count');
%     title(IMU_labels{w});
%     grid on;
%     legend('Location', 'northoutside', ...
%            'Orientation', 'horizontal');
% end
% sgtitle('IMU Histograms');
% drawnow;