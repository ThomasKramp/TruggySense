close all;

% a sample on how to review data acquired from the RCV
json_string = fileread("/home/robbe/Documents/GitHub/master_thesis/code/Data_Formatter/acquired_data/13-04-2025/llc_data_9.json");
data = jsondecode(json_string);

set(gca, "fontsize", 12)

%subplot(2,1,1);
hold on;
plot([data(:).timestamp], [data(:).pitch],'LineWidth',2);
plot([data(:).timestamp], [data(:).roll],'LineWidth',2);
plot([data(:).timestamp], [data(:).yaw],'LineWidth',2);
xlabel("timestamp (ms)");
ylabel("degree (°)");
title("Euler angles over time");
legend("pitch", "roll", "yaw")
grid('on');
hold off;
##
##subplot(2,1,2);
##hold on;
##plot([data(:).timestamp], [data(:).rpm_wheel_1],'LineWidth',3);
##plot([data(:).timestamp], [data(:).rpm_wheel_2],'LineWidth',3);
##plot([data(:).timestamp], [data(:).rpm_wheel_3],'LineWidth',3);
##plot([data(:).timestamp], [data(:).rpm_wheel_4],'LineWidth',3);
##xlabel("timestamp (ms)");
##ylabel("RPM");
##title("wheel rotation over time");
##legend("rpm_{wheel_1}", "rpm_{wheel_2}", "rpm_{wheel_3}", "rpm_{wheel_4}")
##grid('on');
##hold off;
