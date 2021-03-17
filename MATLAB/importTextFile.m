function measurements = importTextFile( file_name, dim_var)
    % IMPORTTEXTFILE( file_name) imports random variable realizations and the
    % associated covariance matrix. The text file must follow a specific
    % format.
    if nargin < 2
        dim_var = -1;
    end
    
    import_covariances = true;
    %% Read header
    fid = fopen( file_name, 'r');

    % Scan header (first line)
    header_str = fgetl(fid);
    col_titles = strsplit(header_str, '\t');
    % Remove last header if it's empty
    if isempty( col_titles{ end})
        col_titles(end) = [];
    end

    % Get total number of columns: 1 (time) + dim_var (mean) + dim_var^2
    % (covariance (column majro))
    num_cols = length( col_titles);
    % Solve the quadratic equation to get the number of variables
    if dim_var < 0
        dim_var = (-1 + sqrt( 1 + 4 * (num_cols - 1)))/ 2;
    elseif dim_var ~= (-1 + sqrt( 1 + 4 * (num_cols - 1)))/ 2
        % Then ignore covariances
        import_covariances = false;
    end
    % Check if it's an integer
    if ( dim_var / ceil( dim_var)) ~= 1
        error('Imported dimension invalid');    
    end

    fclose( fid);

    %% Import data
    % If dataLines is not specified, define defaults
    dataLines = [2, Inf];

    %% Setup the Import Options
    opts = delimitedTextImportOptions("NumVariables", num_cols);

    % Specify range and delimiter
    opts.DataLines = dataLines;
    opts.Delimiter = "\t";

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
    values_mat   = nan( num_points, dim_var);    
    if import_covariances
        cov_matrices = nan( num_points, dim_var^2);
    end
    for lv1 = 2 : num_cols
        if ( lv1 - 1) <= dim_var
            values_mat(:, lv1 - 1) = tbl.(variable_names( lv1));    
        elseif import_covariances
            cov_matrices(:, lv1 - 1 - dim_var) = tbl.(variable_names( lv1));
        end
    end
    % Columnize the measurements (transpose, do not reshape!)
    values_mat = values_mat';
        
    if import_covariances
        % Covariances
        cov_3d = reshape(cov_matrices', dim_var, dim_var, []);

        % Go over each matrix and ensure symmetry
        for lv1 = 1 : size( cov_3d, 3)
            cov_3d(:, :, lv1) = (1/2) * (cov_3d(:, :, lv1) + cov_3d(:, :, lv1)');
        end
    else
        cov_3d = [];
    end

    % Store into struct
    measurements.values = values_mat;
    measurements.time   = Time;
    measurements.cov    = cov_3d;
end