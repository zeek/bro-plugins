refine flow SSDP_Flow += {
	function proc_ssdp_method(m: Method): bool
		%{
		if ( ssdp_method ) 
			{
	                connection()->bro_analyzer()->ProtocolConfirmation();

			BifEvent::generate_ssdp_method(connection()->bro_analyzer(), connection()->bro_analyzer()->Conn(), bytestring_to_val(${m.method}));
			}

		return true;
		%}

        function proc_ssdp_header(hdr: Header): bool
                %{
		if ( ssdp_header )
			{
	                BifEvent::generate_ssdp_header(connection()->bro_analyzer(), connection()->bro_analyzer()->Conn(), bytestring_to_val(${hdr.name}), bytestring_to_val(${hdr.value}));
			}

		return true;
               %}

};

refine typeattr Method += &let {
	proc: bool = $context.flow.proc_ssdp_method(this);
};

refine typeattr Header += &let {
        proc: bool = $context.flow.proc_ssdp_header(this);
};
