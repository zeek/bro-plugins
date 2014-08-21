##! Interface for the DataSeries log writer.

module LogDataSeries;

# Default function to postprocess a rotated DataSeries log file. It moves the
# rotated file to a new name that includes a timestamp with the opening time,
# and then runs the writer's default postprocessor command on it.
function default_rotation_postprocessor_func(info: Log::RotationInfo) : bool
	{
	# Move file to name including both opening and closing time.
	local dst = fmt("%s.%s.ds", info$path,
			strftime(Log::default_rotation_date_format, info$open));

	system(fmt("/bin/mv %s %s", info$fname, dst));

	# Run default postprocessor.
	return Log::run_rotation_postprocessor_cmd(info, dst);
	}

redef Log::default_rotation_postprocessors += { [Log::WRITER_DATASERIES] = default_rotation_postprocessor_func };
