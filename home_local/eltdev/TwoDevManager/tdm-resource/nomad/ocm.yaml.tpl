cfg:
    instrument_id: "TwoDevManager"
    log_properties: "config/daq/ocm-log.properties"
    oldb_uri_prefix: "cii.oldb:/elt/tdm/ocm"

    req_endpoint: "zpb.rr://{{ range service "ocm-req" }}{{ .Address }}:{{ .Port }}{{ end }}/"
    pub_endpoint: "zpb.ps://{{ range service "ocm-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/"
    dpm:
        req_endpoint: "zpb.rr://{{ range service "dpm-req" }}{{ .Address }}:{{ .Port }}{{ end }}/"
        pub_endpoint: "zpb.ps://{{ range service "dpm-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/"

