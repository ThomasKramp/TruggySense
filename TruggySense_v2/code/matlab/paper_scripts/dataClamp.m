function x = dataClamp(x)

    mask = x > 0.5;
    if sum(mask) < length(x) * 0.0378
        threshold = max(x(x <= 0.5));
        x(mask) = threshold;
    end

    mask = x < 0.5;
    if sum(mask) < length(x) * 0.0378
        threshold = min(x(x >= 0.5));
        x(mask) = threshold;
    end
    
end