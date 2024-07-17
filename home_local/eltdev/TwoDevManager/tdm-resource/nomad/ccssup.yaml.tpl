!cfg.include config/sup/syssup/server/definitions.yaml:

server: !cfg.type:SysSup
    server_id       : 'ccssup'

    req_endpoint    : "zpb.rr://{{ range service "ccssup-req" }}{{ .Address }}:{{ .Port }}{{ end }}/"
    pub_endpoint    : "zpb.ps://{{ range service "ccssup-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/"

    db_timeout      : 2000
    scxml           : "config/sup/syssup/server/sm.xml"
    log_properties  : "config/sup/syssup/server/log_properties.cfg"
    req_timeout     : 120000
    dictionaries    : []
    mon_timeout     : 2000
    oldb_prefix     : 'tdm'
    ob_modes        : []
    subsystems      : [
    {
    name: subsim2,
    scope: internal,
    type: sup::syssup::common::Generic,
    
    rr_endpoint: "zpb.rr://{{ range service "subsim2-req" }}{{ .Address }}:{{ .Port }}{{ end }}/StdCmds",
    ps_endpoint: "zpb.ps://{{ range service "subsim2-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/",
    
    access: true
    },
    {
    name: subsim3,
    scope: internal,
    type: sup::syssup::common::Generic,
    
    rr_endpoint: "zpb.rr://{{ range service "subsim3-req" }}{{ .Address }}:{{ .Port }}{{ end }}/StdCmds",
    ps_endpoint: "zpb.ps://{{ range service "subsim3-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/",
    
    access: true
    },
    {
    name: subsim3,
    scope: internal,
    type: sup::syssup::common::Generic,
    
    rr_endpoint: "zpb.rr://{{ range service "subsim3-req" }}{{ .Address }}:{{ .Port }}{{ end }}/StdCmds",
    ps_endpoint: "zpb.ps://{{ range service "subsim3-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/",
    
    access: true
    }
    ]
