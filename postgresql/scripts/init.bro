module LogPostgres;

export {
	## Default hostname if none specified
	const default_hostname = "localhost" &redef;

	## default database name. Only used if different from the empty string
	const default_dbname = "" &redef;

	## default port. Only used if zero or greater
	const default_port = -1 &redef;
}
