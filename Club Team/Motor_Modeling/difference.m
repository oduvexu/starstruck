function [ area ] = difference( model_exec, inputs, exp_data, draw )

    experiment = @(t)interp1(exp_data(:,1),exp_data(:,4),t);
    
    t_end = exp_data(end,1);
    
    [t, model_data] = model_exec(t_end,inputs);
    model_data = cat(2,t,model_data);
    
    model = @(t)interp1(model_data(:,1),model_data(:,3),t);
    
    delta = .01;
    
    diff_data_original = (0:delta:t_end).';
    
    diff_func = @(t)abs(model(t)-experiment(t));
    
    experiment_area_func = @(t)abs(experiment(t));
    
    diff_data = arrayfun(diff_func,diff_data_original)*delta;
    
    experiment_area_data = arrayfun(experiment_area_func,diff_data_original)*delta;
    
    area = sum(diff_data);
    exp_area = sum(experiment_area_data);
    
    percentage_error = area/exp_area;
    
    area = percentage_error;
    
    inputs
    
    if (draw)
        figure(1);
        hold off;
        plot(exp_data(:,1),exp_data(:,4)); % blue
        hold on;
        plot(model_data(:,1), model_data(:,3)); % orange
        
        figure(2);
        hold off;
        plot(exp_data(:,1),exp_data(:,3)); % blue
        hold on;
        plot(model_data(:,1), model_data(:,2));
        drawnow;
    end
    
end