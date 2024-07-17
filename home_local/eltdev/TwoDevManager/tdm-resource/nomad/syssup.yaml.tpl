!cfg.include config/sup/syssup/server/definitions.yaml:

server: !cfg.type:SysSup
    server_id       : 'sup'

    req_endpoint    : "zpb.rr://{{ range service "syssup-req" }}{{ .Address }}:{{ .Port }}{{ end }}/"
    pub_endpoint    : "zpb.ps://{{ range service "syssup-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/"

    db_timeout      : 2000
    scxml           : "config/sup/syssup/server/sm.xml"
    log_properties  : "config/sup/syssup/server/log_cii_properties.cfg"
    req_timeout     : 120000
    dictionaries    : []
    mon_timeout     : 2000
    oldb_prefix     : 'elt/tdm'
    ob_modes        : [
    {
    name: Imaging,
    subsystems: ['subsim2', 'subsim3']
    }
    ]
    subsystems      : [
    {
    name: fcs,
    scope: internal,
    type: sup::syssup::common::Generic,
    
    rr_endpoint: "zpb.rr://{{ range service "fcs-req" }}{{ .Address }}:{{ .Port }}{{ end }}/StdCmds",
    ps_endpoint: "zpb.ps://{{ range service "fcs-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/",
    
    access: true
    },
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
    name: TestSim1,
    scope: internal,
    type: sup::syssup::common::Generic,
    
    rr_endpoint: "zpb.rr://{{ range service "tdmccf-req" }}{{ .Address }}:{{ .Port }}{{ end }}/StdCmds",
    ps_endpoint: "zpb.ps://{{ range service "tdmccf-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/",
    
    access: true
    },
    {
    name: ocm,
    scope: internal,
    type: sup::syssup::common::Generic,
    
    rr_endpoint: "zpb.rr://{{ range service "ocm-req" }}{{ .Address }}:{{ .Port }}{{ end }}/std",
    ps_endpoint: "zpb.ps://{{ range service "ocm-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/",
    
    access: true
    }
    ]
