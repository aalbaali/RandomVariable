function [ measurements, struct] = importTextFile( file_name)
    % IMPORTTEXTFILE( file_name) imports random variable realizations and the
    % associated covariance matrix. The text file must follow a specific
    % format.
    
    %% Read header
    fid = fopen( file_name, 'r');

    % Line 1 : mean_size : [ rows, cols]
    mean_size_str  = fgetl(fid);
    ch = strfind( mean_size_str, ':');
    mean_size = cellfun(@str2num, strsplit( mean_size_str( ch+1 : end), ','));
    
    % Line 2 : dof
    dof_str = fgetl(fid);
    ch = strfind( dof_str, ':');
    dof = cellfun(@str2num, strsplit( dof_str( ch+1 : end), ','));
    
    % Line 3 : num_meas
    num_meas_str = fgetl(fid);
    ch = strfind( num_meas_str, ':');
    num_meas = str2num( num_meas_str( ch+1 : end));
    
    % Skip the splitter (===========)
    fgetl( fid);
    header_str = fgetl( fid);
    col_titles = strsplit(header_str);    
    fgetl( fid);
    % Remove last header if it's empty
    if isempty( col_titles{ end})
        col_titles(end) = [];
    end

    % Get total number of columns: 1 (time) + dim_var (mean) + dim_var^2
    % (covariance (column majro))
    num_cols = length( col_titles);
    % Solve the quadratic equation to get the number of variables
    
    if num_cols > (2 + prod( mean_size))
        % Then ignore covariances
        import_covariances = true;
    else
        import_covariances = false;
    end

    fclose( fid);

    %% Import data
    % If dataLines is not specified, define defaults
    dataLines = [7, Inf];

    %% Setup the Import Options
    opts = delimitedTextImportOptions("NumVariables", num_cols);

    % Specify range and delimiter
    opts.DataLines = dataLines;
%     opts.Delimiter = ",";

    % Specify column names and types
    variable_names = strings(1, num_cols);
    for lv1 = 1 : num_cols
        variable_names( lv1) = col_titles{ lv1};
    end
    variable_types = repelem("double", 1, num_cols);

    opts.VariableNames = variable_names;
    opts.VariableTypes = variable_types;
    % opts = setvaropts(opts, num_cols, "WhitespaceRule", "preserve");
    opts = setvaropts(opts, num_cols, "EmptyFieldRule", "auto");
    opts.ExtraColumnsRule = "ignore";
    opts.EmptyLineRule = "read";

    % Import the data
    tbl = readtable(file_name, opts);

    % Number of data points
    num_points = size( tbl, 1);

    %% Convert to output type
    Time = tbl.Time;
    % Matrix of mean values
    values_mat   = nan( num_points, prod( mean_size));    
    if import_covariances
        cov_matrices = nan( num_points, dof^2);
    end
    for lv1 = 1 : prod( mean_size)
        values_mat(:, lv1) = tbl.(variable_names( lv1 + 1));
    end
    if import_covariances
        for lv1 = 1 : dof^2
            cov_matrices( :, lv1) = tbl.(variable_names( 1 + prod(mean_size) + lv1));
        end
    end

    % Columnize the measurements (transpose, do not reshape!)
    values_mat = values_mat';
    values3d = reshape( values_mat, [ mean_size, num_meas]);
    
    if import_covariances
        % Covariances
        cov_3d = reshape(cov_matrices', dof, dof, []);

        % Go over each matrix and ensure symmetry
        for lv1 = 1 : size( cov_3d, 3)
            cov_3d(:, :, lv1) = (1/2) * (cov_3d(:, :, lv1) + cov_3d(:, :, lv1)');
        end
    else
        cov_3d = [];
    end

    % Store into struct
    measurements.values   = values_mat;
    measurements.values3d = values3d;
    measurements.time     = Time;
    measurements.cov      = cov_3d;
end