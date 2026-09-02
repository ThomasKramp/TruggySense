imgTrack = imread('track_picture.jpeg');
sizeImageTrack = size(imgTrack);

scaleFactors = [-1  1 1] * 0.8;
offsetFactors = [0 0 0];

% Prepare coords
coordsOnTrack = [positionLatOK positionLongOK zeros(size(positionLatOK))];
coordsOnTrack = coordsOnTrack - mean(coordsOnTrack);
coordsOnTrack(:,1:2) = coordsOnTrack(:,1:2) ./ max(coordsOnTrack(:,1:2));
coordsOnTrack = coordsOnTrack * rotz(68);
coordsOnTrack = coordsOnTrack .* (sizeImageTrack/2) .* scaleFactors;
coordsOnTrack = coordsOnTrack + (sizeImageTrack/2) + offsetFactors;

% --- Step 1: Manual definition of bounding quad ---
figure(11); clf;
imshow(imgTrack); hold on;
axis manual;
set(gca, 'Clipping', 'off');

title('Move the red points to define the bounding quadrilateral');

% Initial guess in the middle of the image (you can tune)
center = sizeImageTrack([2 1]) / 2;
offset = 200;
defaultBox = [
    center + [-offset,  offset];  % Top-Left
    center + [ offset,  offset];  % Top-Right
    center + [ offset, -offset];  % Bottom-Right
    center + [-offset, -offset];  % Bottom-Left
];

% Draw red control points
hPts = gobjects(4,1);
for i = 1:4
    hPts(i) = drawpoint('Position', defaultBox(i,:), ...
                        'Color','r','MarkerSize',10);
end

% Draw connecting polygon
hPoly = plot([defaultBox(:,1); defaultBox(1,1)], ...
             [defaultBox(:,2); defaultBox(1,2)], ...
             'r-', 'LineWidth', 2);

% Live update of polygon edges as points move
for i = 1:4
    addlistener(hPts(i), 'MovingROI', @(src, evt) updatePolygon(hPts, hPoly));
end

% === Wait for user to finish editing ===
uiwait(msgbox('Adjust the red points to define the bounding quad, then click OK to transform.'));

% --- Step 2: Apply transformation ---
% Build movedBox from red points
movedBox = zeros(4,2);
for i = 1:4
    movedBox(i,:) = hPts(i).Position;
end

% Define original quad in data space (square box before transform)
originalBox = [
   -1,  1;  % TL
    1,  1;  % TR
    1, -1;  % BR
   -1, -1   % BL
];
originalBox = originalBox * 0.5 * max(sizeImageTrack);  % scale to image-like size
originalBox = originalBox + center;  % center in image

% Compute and apply projective transform
tform = fitgeotrans(originalBox, movedBox, 'projective');
[xNew, yNew] = transformPointsForward(tform, coordsOnTrack(:,1), coordsOnTrack(:,2));

% Draw result
scatter(xNew, yNew, 10, 'filled', 'MarkerFaceColor', 'g');
title('Green = transformed track points');



% === Helper: update polygon when dragging points ===
function updatePolygon(hPts, hPoly)
    quad = zeros(4,2);
    for k = 1:4
        quad(k,:) = hPts(k).Position;
    end
    % Close the loop for polygon
    set(hPoly, 'XData', [quad(:,1); quad(1,1)], ...
               'YData', [quad(:,2); quad(1,2)]);
end
