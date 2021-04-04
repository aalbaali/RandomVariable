function generateTextFile( file_name, time_array, value_array, cov_array, colname)
    % GENERATETEXTFILE( time_array, value_array, header_cell, file_name)
    % generates text files using the provided data.
    % The value_array should be column major (columns represent time steps)
    % Filename should include the directory.
    %
    % Notes: 
    %   - Length of time array should match number of columns of value_array
    %   - Number of rows of 'value_array' should match the number of header cells
    sz_val = size( value_array);
    if sz_val( 2) ~= length( time_array)
        error( 'Number of time steps do not match the time array');
    end
    
    if ~exist('colname', 'var')
        colname = 'y';
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
    fprintf( fh, "mean_size\t\t:\t%i,\t%i\n", size( value_array, 1), 1);
    fprintf( fh, "dof\t\t\t:\t%i\n", size( value_array, 1));
    fprintf( fh, "num_meas\t\t:\t%i\n", size( value_array, 2));
    fprintf( fh, repmat( repmat('=', 1, 16), 1, 1 + size(value_array, 1)+ size(cov_array, 1)^2));
    linebrk();
    header_string = sprintftab( 'Time');
    % Add measurement header column names
    for lv1 = 1 : size(value_array, 1)
        header_string = sprintf('%s%s', header_string, sprintftab( ...
            sprintf('%s_%i', colname, lv1)));
    end
    if exist( 'cov_array', 'var') && ~isempty( cov_array)
        for lv1 = 1 : size( cov_array, 1)
            for lv2 = 1 : size( cov_array, 1)
                header_string = sprintf('%s%s', header_string, sprintftab( ...
                    sprintf('cov_%i%i', lv1, lv2)));
            end
        end
    end
    % Write header
    fprintf( fh, header_string); 
    linebrk();
    fprintf( fh, repmat( repmat('=', 1, 16), 1, 1 + size(value_array, 1)+ size(cov_array, 1)^2));
    linebrk();
    
    % Go over each data point and print data
    for lv1 = 1 : length( time_array)
        line_str = vec2str( [time_array( lv1),  value_array( :, lv1)']);
        if exist( 'cov_array', 'var') && ~isempty( cov_array)
            cov_str = vec2str( reshape( cov_array( :, :, lv1), [], 1));
            line_str = sprintf('%s%s', line_str, cov_str);
        end
        fprintf( fh, line_str);
        linebrk();
    end
    fclose( fh);
end