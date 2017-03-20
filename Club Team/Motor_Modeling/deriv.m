function [ Z_d ] = deriv(t, Z, motor, effort)
% This function represents the motor state-space equation.  It takes as
% constants 6 scalars and 1 constant function of time.  The function of
% time should be defined as an interpolated anonymous function.

    % "motor" is a 6-tuple representing the characteristics of the motor.
    J = motor(1);
 %   K = motor(2);
    B = motor(2);
    L = motor(3);
%    R = motor(4);
    
%    K = .0621;
    
%    R = 2.48;
 
    K = motor(5);
    
    R = motor(6);

    V = motor(4);
    
    % 'effort' is a function handle that returns the effort given as a 
    % function of time.
    battery = 7.8;
    
    %voltage = battery / 20 * (29 - 88/sqrt(effort(t)+.001));
    %voltage = max(0, V*voltage);

    voltage = battery / 7.8 * (0.0905*effort(t) - 0.1976);
    voltage = max(0, V*voltage);
    
    ang = Z(1);
    vang = Z(2);
    i = Z(3);
    
    ang_accel = 1/J*(K*i-B*vang);
    i_dot = 1/L*(voltage-R*i-K*vang);
    
    if (J < 0 || K < 0 || B < 0 || L < 0 || R < 0)
        ang_accel = 0;
    end
    
    Z_d = [vang; ang_accel; i_dot];
end

