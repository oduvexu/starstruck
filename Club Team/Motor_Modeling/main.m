
TICKS_PER_REVOLUTION_TORQUE = 627.2;
TICKS_PER_REVOLUTION_SPEED = 392;
TICKS_PER_REVOLUTION_TURBO = 261.333;

TICKS_PER_REVOLUTION_INTERNAL = 240.448;

Experiment = csvread('constant_effort50.csv', 1, 1);

Exp_Time = Experiment(:,1);
Exp_Effort = Experiment(:,2);
Exp_Ticks = Experiment(:,3);
Exp_RPM = Experiment(:,4);

time_lag = 0.1;

Exp_RPM_ = @(x)interp1(Exp_Time,Exp_RPM,x);
Exp_RPM = arrayfun(Exp_RPM_, Exp_Time + time_lag);

Exp_RPM(isnan(Exp_RPM)) = 0;

Exp_RadiansPerSecond = Exp_RPM * (TICKS_PER_REVOLUTION_SPEED / TICKS_PER_REVOLUTION_INTERNAL) / (60) * 2 * pi * 3.8;

Exp_Radians = Exp_Ticks * (TICKS_PER_REVOLUTION_INTERNAL / TICKS_PER_REVOLUTION_SPEED) / (2*pi);

Exp_RPM = Exp_RadiansPerSecond;
Exp_Ticks = Exp_Radians;

foobar = [Exp_RadiansPerSecond   Exp_Radians];

% Normalize time to zero.
Exp_Time = Exp_Time - Exp_Time(1);

initial_speed = Exp_RPM(1);
initial_pos = Exp_Ticks(1);

max_simulation_time = max(Exp_Time);

exp_data = [Exp_Time, Exp_Effort, Exp_Ticks, Exp_RPM];

effort = @(t) interp1(Exp_Time,Exp_Effort,t);

% effort = @(t) 0

format long;

diff = @(t,Z)deriv(t,Z,motor,effort);

model_exec = @(t_end,inputs)ode45(@(t,Z)deriv(t,Z,inputs,effort),[0 max_simulation_time],[initial_pos; initial_speed; 0]);

optim = @(m) difference(model_exec, m, exp_data, true);

guess = [0.000010410671586   0.000208870528766   0.391386588375573   0.950612618854886   0.014384541481722   2.362883808478149];

optim(guess);

opt = optimset('MaxIter',200*30,'MaxFunEvals',200*30);
optimal_motor = fminsearch(optim, guess ,opt)

difference(model_exec, guess, exp_data, true)

[t, model_data] = model_exec(max_simulation_time, optimal_motor);

figure(3)
plot(t,model_data(:,3))