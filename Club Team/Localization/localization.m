
input_data = csvread('localization_input_data.csv', 1, 1);

% Remove leading zeroes and values afterward that do not change.
crop = min(find(input_data(:,2), 1, 'first'), find(input_data(:,3), 1, 'first'));
input_data = input_data(crop-1:end,:);
delta_input = [zeros(1,size(input_data,2)) ; input_data];
delta_input = delta_input(1:end-1,:);
delta_input = delta_input - input_data;
crop = max(find(delta_input(:,2),1,'last'), find(delta_input(:,3),1,'last'));
input_data = input_data(1:crop, :);
input_data(:,1) = input_data(:,1) - input_data(1,1);

angle = 0;
x = 0;
y = 0;

t = input_data(1,1);
left = input_data(1,2);
right = input_data(1,3);

TICKS_LINEAR = 1;
TICKS_ANGULAR = 0.025750759;

trajectory = input_data;

for i = 2:size(input_data,1)
    dist = TICKS_LINEAR*(input_data(i,2) - left) + (input_data(i,3) - right);
    
%    angle = TICKS_ANGULAR*(input_data(i,2) - input_data(i,3));
    
    x = x + dist*sin(angle);
    y = y + dist*cos(angle);
    
    trajectory(i,2) = x;
    trajectory(i,3) = y;
    
    angle = TICKS_ANGULAR*(input_data(i,2) - input_data(i,3));
    
    left = input_data(i,2);
    right = input_data(i,3);
end

[x y]

figure(1)
hold off
plot(trajectory(:,2), trajectory(:,3))

figure(2)
hold off
plot(TICKS_ANGULAR*(input_data(:,2) - input_data(:,3)), trajectory(:,2));