

Experiment = csvread('friction_data.csv', 1, 1);

Exp_Time = Experiment(:,1);
Exp_Effort = Experiment(:,2);
Exp_Ticks = Experiment(:,3);
Exp_RPM = Experiment(:,4);

% Convert from ticks to rotations
TICKS_PER_ROTATION = 261.333;
%Exp_Ticks = Exp_Ticks / TICKS_PER_ROTATION;

Exp_Ticks = Exp_Ticks / 2 / pi;

% Normalize time to zero.
Exp_Time = Exp_Time - Exp_Time(1);

initial_speed = Exp_RPM(1);
initial_pos = Exp_Ticks(1);

max_simulation_time = max(Exp_Time);

exp_data = [Exp_Time, Exp_Effort, Exp_Ticks, Exp_RPM];

%effort = @(t) interp1(Exp_Time,Exp_Effort,t);

effort = @(t) 0;

diff = @(t,Z)deriv(t,Z,motor,effort);

model_exec = @(t_end,inputs)ode45(@(t,Z)deriv(t,Z,inputs,effort),[0 max_simulation_time],[initial_pos; initial_speed; 10]);

optim = @(m) difference(model_exec, m, exp_data, true);

guess = [0.0121    0    0.2750];

optim(guess)

opt = optimset('MaxIter',200*30,'MaxFunEvals',200*30);
optimal_motor = fminsearch(optim, guess ,opt)