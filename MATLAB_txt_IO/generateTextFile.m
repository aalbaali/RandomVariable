function generateTextFile( file_name, time_array, value_array, cov_3d, colname, time_dim)
    % GENERATETEXTFILE( time_array, value_array, header_cell, file_name)
    % generates text files using the provided data.
    % The value_array should be column major (columns represent time steps)
    % Filename should include the directory.
    %
    % Notes: 
    %   - Length of time array should match number of columns of value_array
    %   - Number of rows of 'value_array' should match the number of header cells    
        
    if ~exist('time_dim', 'var')
        % Time dimension of measurements (3D or 2D array)
        time_dim = 2;
    end    
    if size(value_array, time_dim) ~= length( time_array)            
        error( 'Number of time steps do not match the time array');
    end
    
    if ~exist('colname', 'var')
        colname = 'y';
    end
    
    % Measurement size and number of measurements
    if time_dim == 2
        meas_size = [ size( value_array, 1), 1];
        num_meas  = size( value_array, 2);
    elseif time_dim == 3
        meas_size = size( value_array( :, :, 1));
        num_meas  = size( value_array, 3);
    end
    % Generate a text file. fh = file_handle
    fh = fopen( file_name, 'w');
    if fh < 0
        error('Error opening file');
    end
    % Lambda function that takes a string and converts to sprintf with a
    % tab after it
    sprintftab = @( str) sprintf( '%-16s', str);
    
    % Lambda function that inserts a line break
    linebrk = @() fprintf( fh, '\n');
    
    % Lambda function that outputs a column matrix as a string with tabs
    % between the data points
%     vec2str = @( vec) cell2mat( arrayfun( @(kk) sprintftab(sprintf('%f', kk)), ...
%         vec, 'UniformOutput', false));
%     vec2str = @( vec) sprintf( repmat('%-16f', 1, length( vec)), vec);
    vec2str = @( vec) strjoin( arrayfun(@(kk) sprintf('%-16s', ...
        sprintf('%f,', kk)), vec, 'UniformOutput', false));
    % Header string
    %   Mean size
    fprintf( fh, "mean_size\t\t:\t%i,\t%i\n", meas_size);
    fprintf( fh, "dof\t\t\t:\t%i\n", size( value_array, 1));
    fprintf( fh, "num_meas\t\t:\t%i\n", num_meas);
    fprintf( fh, repmat( repmat('=', 1, 16), 1, 1 + size(value_array, 1)+ size(cov_3d, 1)^2));
    linebrk();
    header_string = sprintftab( 'Time');
    % Add measurement header column names
    for lv1 = 1 : meas_size( 1)
        for lv2 = 1 : meas_size( 2)
            header_string = sprintf('%s%s', header_string, sprintftab( ...
                sprintf('%s_%i%i', colname, lv2, lv1)));
        end
    end
    if exist( 'cov_3d', 'var') && ~isempty( cov_3d)
        for lv1 = 1 : size( cov_3d, 1)
            for lv2 = 1 : size( cov_3d, 1)
                header_string = sprintf('%s%s', header_string, sprintftab( ...
                    sprintf('cov_%i%i', lv1, lv2)));
            end
        end
    end
    % Write header
    fprintf( fh, header_string); 
    linebrk();
    fprintf( fh, repmat( repmat('=', 1, 16), 1, 1 + size(value_array, 1)+ size(cov_3d, 1)^2));
    linebrk();
    
    % Go over each data point and print data
    for lv1 = 1 : num_meas
        if time_dim == 2
            line_str = vec2str( [time_array( lv1),  value_array( :, lv1)']);
        elseif time_dim == 3
            value_k = value_array( :, :, lv1);
            line_str = vec2str( [time_array( lv1),  value_k(:)']);
        end
        if exist( 'cov_3d', 'var') && ~isempty( cov_3d)
            cov_str = vec2str( reshape( cov_3d( :, :, lv1), [], 1));
            line_str = sprintf('%s%s', line_str, cov_str);
        end
        fprintf( fh, line_str);
        linebrk();
    end
    fclose( fh);
end