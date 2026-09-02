function plotImageOnTrack( imgTrack, positionLatOK, positionLongOK, valueToPlot)
   
    if(mean(valueToPlot) ~= 0)

        minValue = min(valueToPlot);
        maxValue = max(valueToPlot);
        normalizedVel = (valueToPlot - minValue) / (maxValue - minValue);
        cmap = jet(256);
        colorIdx = round(1 + normalizedVel * (size(cmap, 1) - 1));
        colorIdx = min( colorIdx, 255);
        colorsOK = cmap(colorIdx, :); 
    end
    % imgTrack = imread('WhatsApp Image 2025-04-16 at 17.22.39.jpeg');
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
    
    % Bounding box corners (clockwise: BL, BR, TR, TL)
    minY = min(coordsOnTrack(:,1));
    maxY = max(coordsOnTrack(:,1));
    minX = min(coordsOnTrack(:,2));
    maxX = max(coordsOnTrack(:,2));
    
    originalBox = [
        minY, minX+200;  % Bottom-Left
        maxY minX;  % Bottom-Right
        minY, maxX;  % Top-Right
        maxY, maxX-200;  % Top-Left
    ];
    
    % Show image and initial plot
 
    imshow(imgTrack); hold on;



    % scatter(coordsOnTrack(:,2), coordsOnTrack(:,1), 10, 'filled', colorsOK); % static reference
    % livePlot = scatter(coordsOnTrack(:,2), coordsOnTrack(:,1), 10, colorsOK,'filled'); % live-updating
    size(valueToPlot)
    size(coordsOnTrack)
    if(mean(valueToPlot) ~= 0)
        livePlot = scatter(coordsOnTrack(:,2), coordsOnTrack(:,1), 10, valueToPlot, 'filled'); % live-updating
        colormap(jet(256));        % apply colormap for the scatter
        c = colorbar;                  % show colorbar
        c.Location = "southoutside";
    else
        livePlot = plot(coordsOnTrack(:,2), coordsOnTrack(:,1), '-.', 'Color','red', 'LineWidth',3);
    end

    % caxis([minValue maxValue]); % match colorbar scale to your data range
    
    axis manual;             % fix axes limits
    set(gca, 'Clipping', 'off');  % allow annotations (drawpoints, etc) outside image 
    
    hPts = gobjects(4,1);
    for i = 1:4
        hPts(i) = drawpoint('Position', fliplr(originalBox(i,:)), ...
                            'Color','r','MarkerSize',10);
    end
    
    % Attach listeners to red points
    for i = 1:4
        addlistener(hPts(i), 'MovingROI', @(src, evt) updateLiveTrack(hPts,coordsOnTrack,originalBox,livePlot));
    end
    
    % Initial update
    updateLiveTrack(hPts,coordsOnTrack,originalBox,livePlot);

end

% === Nested function AFTER hPts is created ===
    function updateLiveTrack(hPts,coordsOnTrack,originalBox,livePlot)
        movedBox = zeros(4,2);
        for j = 1:4
            movedBox(j,:) = fliplr(hPts(j).Position);  % image = [x,y], coords = [row,col]
        end
        tform = fitgeotrans(originalBox, movedBox, 'projective');
        [xNew, yNew] = transformPointsForward(tform, coordsOnTrack(:,1), coordsOnTrack(:,2));
        set(livePlot, 'XData', yNew, 'YData', xNew);
    end