function generateTextFile( time_array, value_array, header_cell, file_name)
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
    if sz_val( 1) ~= length( header_cell)
        error( 'Number of header cell does not match the number of rows of data');
    end
    
    % Generate a text file. fh = file_handle
    fh = fopen( file_name, 'w');
    if fh < 0
        error('Error opening file');
    end
    % Lambda function that takes a string and converts to sprintf with a
    % tab after it
    sprintftab = @( str) sprintf( '%s\\t', str);
    
    % Lambda function that inserts a line break
    linebrk = @() fprintf( fh, '\n');
    
    % Lambda function that outputs a column matrix as a string with tabs
    % between the data points
    vec2str = @( vec) cell2mat( arrayfun( @(kk) sprintftab( num2str( kk)), ...
        vec, 'UniformOutput', false));
    
    % Header string
    header_string = sprintf( 'Time\\t');
    for lv1 = 1 : length( header_cell)
        header_string = strcat( header_string, sprintftab( header_cell{ lv1}));
    end
    % Write header
    fprintf( fh, header_string); 
    linebrk();
    
    % Go over each data point and print data
    for lv1 = 1 : length( time_array)
        line_str = vec2str( [time_array( lv1),  value_array( :, lv1)']);
        fprintf( fh, line_str);
        linebrk();
    end
    fclose( fh);
end